#!/usr/bin/env python3

import collections
import itertools
import sys
import json
import re
import ipaddress
import os
import math
from copy import deepcopy
from collections import defaultdict, namedtuple
from dataclasses import dataclass
from sortedcontainers import SortedSet

from common import (
    k8s_create,
    k8s_update_swctl_spec,
    k8s_delete_swctl,
    k8s_list_custom_object,
    k8s_get_software_controls,
    k8s_update_vswitch_ctrl_mac_addrs,
    k8s_update_vswitch_mac_addrs,
    k8s_update_vswitch_gw_addrs,
    k8s_update_bngpg_spec,
    k8s_delete_bngpg,
    k8s_update_bngwl_spec,
    k8s_delete_bngwl,

    ns_filter,

    get,
    merge,
    psql_exec,
    ipv4tomdma,
    ipv6tomdma,
    APOLLO_XI
)

import asyncio
from gwtovcm import VcmCtlAsyncClient
from gwtoolt import OltMgrAsyncClient


from foundation.logger import loadLoggerConfig, getLogger

logger = getLogger('interface_cfg')

DEFAULT_NSI_POOL_COUNT = 64

def psql_delete_control(olt_control):
    match = re.match('olt-control-(\d+)-(\d+)-(\d+).*', olt_control)
    if match is not None:
        chassis = int(match.group(1))
        slot    = int(match.group(2))
        port    = int(match.group(3))
        psql_exec(f'TRUNCATE olt{chassis}_{slot}_{port};')
        psql_exec(f'TRUNCATE olt_counters{chassis}_{slot}_{port};')
        psql_exec(f'TRUNCATE pon_port{chassis}_{slot}_{port};')
        psql_exec(f'TRUNCATE pon_port_counters{chassis}_{slot}_{port};')
        psql_exec(f'TRUNCATE onu{chassis}_{slot}_{port};')

def psql_delete_module_mgmt(module_mgmt):
    # For now only one module-mgmt supported. So it deletion will cause trancation of all related tables.
    psql_exec('TRUNCATE sztp_modules;')
    psql_exec('TRUNCATE sztp_module_transceivers;')
    psql_exec('TRUNCATE sztp_module_interfaces;')

class OltDistributionData():
    def __init__(self, k8s_olt_controls_old, olt_old, delete_olt, k8s_svc_prov_apps_old, spa_cfg):
        self.k8s_olt_controls_old = k8s_olt_controls_old
        self.olt_old = olt_old
        self.delete_olt = delete_olt
        self.k8s_svc_prov_apps_old = k8s_svc_prov_apps_old
        #self.spa_old = spa_old
        self.spa_cfg = spa_cfg

def psql_delete_spa(spa_to_delete):
    match = re.match('svc-prov-app-(\d+)', spa_to_delete)
    spa_id = int(match.group(1))
    psql_exec(f'TRUNCATE vcm_{spa_id};')
    psql_exec(f'TRUNCATE vcm_dhcp_{spa_id};')

def apply_olt_control(olt_control_specs, k8s_software_controls, module_mgmt_specs, olt_control_global_spec, bng_data_global_spec):
    logger.info('Applying olt-control')

    k8s_olt_controls = {w['metadata']['name']: w for w in
                        k8s_software_controls
                        if w['metadata']['name'].startswith('olt-control-')}
    old = set(k8s_olt_controls.keys())
    new = set(olt_control_specs.keys())
    delete = old - new
    create = new - old
    update = new & old

    module_stag_pool_meta, module_vni_pool_meta, tunnel_mgmt_vxlan_src_ip_pool_meta, tunnel_data_vxlan_src_ip_pool_meta \
            = gather_pool_meta(olt_control_specs, module_mgmt_specs)

    module_stag_pools, module_vni_pools = init_module_pools(module_stag_pool_meta, module_vni_pool_meta)
    tunnel_mgmt_vxlan_src_ip_pool, tunnel_data_vxlan_src_ip_pool = \
            init_global_pools(tunnel_mgmt_vxlan_src_ip_pool_meta, tunnel_data_vxlan_src_ip_pool_meta, olt_control_global_spec, bng_data_global_spec)

    # ========== Helper functions =======================
    def get_free_value_from_pool(pool):
        """ Return first unused value from pool
            :type pool: dict[T, Bool]
            :return: T or None if no unused values
        """
        for val, used in pool.items():
            if not used:
                return val
        return None

    def assign_vni_from_pool(tunnel_spec, vni_pool):
        pool_vni = get_free_value_from_pool(vni_pool)
        if pool_vni is None:
            raise ValueError("Unable to find unused VNI in the VNI pool")
        tunnel_spec['vxlan']['vni'] = pool_vni
        vni_pool[pool_vni] = True
        logger.debug(f'Vni {pool_vni} assigned from pool')

    def assign_stag_from_pool(pon_spec, stag_pool):
        pool_stag = get_free_value_from_pool(stag_pool)
        if pool_stag is None:
            raise ValueError("Unable to find unused stag in the stag pool")
        pon_spec['s-tag'] = pool_stag
        stag_pool[pool_stag] = True
        logger.debug(f'Stag {pool_stag} assigned from pool')

    def assign_vxlan_tunnel_src_ip_from_pool(tunnel_spec, vxlan_tunnel_src_ip_pool):
        pool_src_ip = get_free_value_from_pool(vxlan_tunnel_src_ip_pool)
        if pool_src_ip is None:
            raise ValueError("Unable to find unused vxlan tunnel src IP in the pool")
        tunnel_spec['vxlan']['src-ip'] = pool_src_ip
        vxlan_tunnel_src_ip_pool[pool_src_ip] = True
        logger.debug(f'Vxlan tunnel src-ip {pool_src_ip} assigned from pool')

    def update_tunnel_from_global_vxlan_cfg(tunnel_spec, global_vxlan_cfg):
        if not is_global_vxlan_tunnel_configured(global_vxlan_cfg):
            raise ValueError("Global vxlan config for managable empty tunnel does not set")
        mark_tunnel_dynamic(tunnel_spec)
        vxlan_tunnel_spec = tunnel_spec['vxlan']

        vxlan_tunnel_spec['dsi-vlan'] = global_vxlan_cfg['dsi-vlan-id']
        vxlan_tunnel_spec['peer-ip'] = 'dynamic'
        # Note: For now either v4 or v6 global_vxlan_cfg supported
        if is_global_vxlan_tunnel_v4(global_vxlan_cfg):
            gw, prefix_len = global_vxlan_cfg['cre-gw-ip-address'].split('/')
            vxlan_tunnel_spec['gateway'] = gw
            return
        if is_global_vxlan_tunnel_v6(global_vxlan_cfg):
            gw, prefix_len = global_vxlan_cfg['cre-gw-ipv6-address'].split('/')
            vxlan_tunnel_spec['gateway'] = gw
            return
        raise ValueError('Either Ipv4 or Ipv6 global vxlan configuration should be set for empty managable vxlan tunnels')

    def mark_tunnel_dynamic(tunnel_spec):
        vxlan_tunnel_spec = tunnel_spec['vxlan']
        vxlan_tunnel_spec['dynamic-configured'] = True

    # ===================================================
    for olt_control in delete:
        # do not delete protected from deletion olt-control swctls
        spec = k8s_olt_controls[olt_control].get('spec')
        if 'protected-from-deletion' in spec and spec['protected-from-deletion'] is True:
            logger.debug(f'Ignoring: {olt_control} is protected from deletion')
            continue
        logger.debug(f'Deleting: {olt_control}')
        oltClient = OltMgrAsyncClient()
        oltDnsName = f"{olt_control}-0.{olt_control}.{os.environ.get('MY_POD_NAMESPACE', 'default')}.svc.cluster.local"
        loop = asyncio.get_event_loop()
        loop.run_until_complete(oltClient.connectToOltMgr(oltDnsName))
        loop.run_until_complete(oltClient.CleanupRedis())
        logger.debug(f'Deleting SWCTL for: {olt_control}')
        k8s_delete_swctl(k8s_olt_controls[olt_control])
        psql_delete_control(olt_control)

    # Update specs with already used values from pools,
    # If stag/vni value out of pool range it will be reasigned
    # After that itteration, all specs that already exists in K8s updated
    # with already pool assigned values
    for olt_control in update:
        k8s_olt_control_spec = k8s_olt_controls[olt_control].get('spec')
        olt_control_spec = olt_control_specs[olt_control]
        if is_olt_spec_managable(k8s_olt_control_spec, module_mgmt_specs):
            olt_module_spec = olt_control_spec['iface:interface']['module'][0]
            olt_spec = olt_control_spec['iface:interface']['olt'][0]
            olt_module_key = get_module_key(olt_module_spec)
            k8s_olt_spec = k8s_olt_control_spec['iface:interface']['olt'][0]
            if not is_vxlan_tunnel_configured(olt_spec['tunnel-mgmt']):
                # Check if k8s spec for that OLT contains values from pools
                if is_vxlan_tunnel_configured(k8s_olt_spec['tunnel-mgmt']):
                    k8s_olt_mgmt_vni = k8s_olt_spec['tunnel-mgmt']['vxlan']['vni']
                    # Check if VNI is used
                    if k8s_olt_mgmt_vni in module_vni_pools[olt_module_key] and \
                            module_vni_pools[olt_module_key][k8s_olt_mgmt_vni] == False:
                        logger.debug(f'Tunnel mgmt vxlan VNI value already configured for {olt_control} k8s swctl. Vni: {k8s_olt_mgmt_vni}. It will reused.')
                        # Updates current spec with vni value that was already assigned from pool
                        olt_spec['tunnel-mgmt']['vxlan']['vni'] = k8s_olt_mgmt_vni
                        module_vni_pools[olt_module_key][k8s_olt_mgmt_vni] = True
                    else:
                        logger.debug(f'Previous tunnel mgmt vxlan VNI value for {olt_control} k8s swctl: {k8s_olt_mgmt_vni} was out of VNI pool ranged or used by other swctl. Will assigned new VNI value from pool on next stage')
                    # Check if Tunnel-mgmt src IP is used
                    k8s_olt_mgmt_src_ip = k8s_olt_spec['tunnel-mgmt']['vxlan']['src-ip']
                    if k8s_olt_mgmt_src_ip in tunnel_mgmt_vxlan_src_ip_pool and \
                            tunnel_mgmt_vxlan_src_ip_pool[k8s_olt_mgmt_src_ip] == False:
                        logger.debug(f'Tunnel mgmt vxlan src-ip value already configured for {olt_control} k8s swctl. Src-ip {k8s_olt_mgmt_src_ip}. It will reused.')
                        # Updates current spec with src-ip value that was already assigned from pool
                        olt_spec['tunnel-mgmt']['vxlan']['src-ip'] = k8s_olt_mgmt_src_ip
                        tunnel_mgmt_vxlan_src_ip_pool[k8s_olt_mgmt_src_ip] = True
                    else:
                        logger.debug(f'Previous tunnel mgmt vxlan src-ip value for {olt_control} k8s swctl: {k8s_olt_mgmt_src_ip} was out of tunnel-mgmt src-ip pool ranged or used by other swctl. Will assigned new tunnel-mgmt src-ip value from pool on next stage')

                # Updates other tunnel data from global spec
                update_tunnel_from_global_vxlan_cfg(olt_spec['tunnel-mgmt'], olt_control_global_spec['tunnel-mgmt'])
            for pon_spec in olt_spec['pon']:
                pon_port = pon_spec['olt-pon-port']
                k8s_pon_spec = pon_spec_from_olt(k8s_olt_spec, pon_spec)
                if k8s_pon_spec is None:
                    # Pon dynamically added, values from pools will be assigned on next itteration
                    continue
                if not is_vxlan_tunnel_configured(pon_spec['tunnel-data']):
                    if is_vxlan_tunnel_configured(k8s_pon_spec['tunnel-data']):
                        k8s_pon_data_vni = k8s_pon_spec['tunnel-data']['vxlan']['vni']
                        # Check if VNI is used
                        if k8s_pon_data_vni in module_vni_pools[olt_module_key] \
                                and module_vni_pools[olt_module_key][k8s_pon_data_vni] == False:
                            logger.debug(f'Tunnel data vxlan VNI for pon {pon_port} value already configured for {olt_control} k8s swctl. Vni: {k8s_pon_data_vni}. It will reused.')
                            # Updates current spec with vni value that was already assigned from pool
                            pon_spec['tunnel-data']['vxlan']['vni'] = k8s_pon_data_vni
                            module_vni_pools[olt_module_key][k8s_pon_data_vni] = True
                        else:
                            logger.debug(f'Previous tunnel data vxlan VNI value for pon {pon_port} for {olt_control} k8s swctl: {k8s_pon_data_vni} was out of VNI pool range or used by other swctl. Will assigned new VNI value from pool on next stage')
                        # Check if Tunnel-mgmt src IP is used
                        k8s_tunnel_data_src_ip = k8s_pon_spec['tunnel-data']['vxlan']['src-ip']
                        if k8s_tunnel_data_src_ip in tunnel_data_vxlan_src_ip_pool and \
                                tunnel_data_vxlan_src_ip_pool[k8s_tunnel_data_src_ip] == False:
                            logger.debug(f'Tunnel data vxlan src-ip for pon {pon_port} value already configured for {olt_control} k8s swctl. Src-ip: {k8s_tunnel_data_src_ip}. It will reused.')
                            pon_spec['tunnel-data']['vxlan']['src-ip'] = k8s_tunnel_data_src_ip
                            tunnel_data_vxlan_src_ip_pool[k8s_tunnel_data_src_ip] = True
                        else:
                            logger.debug(f'Previous tunnel data vxlan src-ip value for pon {pon_port} for {olt_control} k8s swctl: {k8s_tunnel_data_src_ip} was out of tunnel-mgmt src-ip pool range or used by other swctl. Will assigned new tunnel-mgmt src-ip value from pool on next stage')
                    # Updates other tunnel data from global spec
                    if bng_data_global_spec and 'tunnel-data' in bng_data_global_spec:
                        update_tunnel_from_global_vxlan_cfg(pon_spec['tunnel-data'], bng_data_global_spec['tunnel-data'])
                if 's-tag' not in pon_spec:
                    if 's-tag' in k8s_pon_spec:
                        k8s_pon_stag = k8s_pon_spec['s-tag']
                        if k8s_pon_stag in module_stag_pools[olt_module_key] \
                                and module_stag_pools[olt_module_key][k8s_pon_stag] == False:
                            logger.debug(f'Pon {pon_port} Stag value already configured for {olt_control} k8s swctl. Stag: {k8s_pon_stag}. It will reused.')
                            # Updates current spec with vni value that was already assigned from pool
                            pon_spec['s-tag'] = k8s_pon_stag
                            module_stag_pools[olt_module_key][k8s_pon_stag] = True
                        else:
                            logger.debug(f'Previous pon {pon_port} Stag value for {olt_control} k8s swctl: {k8s_pon_stag} was out of stag pool range or used by other swctl. Will assigned new stag value from pool')
                    if pon_spec['tunnel-data'] is not None:
                        mark_tunnel_dynamic(pon_spec['tunnel-data'])


    # Set OLT's that already exists in k8s, but it can't be updated.
    # Ex. dynamic changes of pool sizes for tunnel src ip, all olts
    #   which tunnel is not fit in pool should be deleted.
    failed_olt_specs = set()

    # Assign values from pools for specs that was not presented in k8s
    # All tunnels that presented in k8s spec, already updated in current spec
    # At this stage all values that already exists in k8s already assigned for
    # spec, so additional checks does not needed
    for olt_control in update.union(create):
        try:
            olt_control_spec = olt_control_specs[olt_control]
            if is_olt_spec_managable(olt_control_spec, module_mgmt_specs):
                olt_module_spec = olt_control_spec['iface:interface']['module'][0]
                olt_spec = olt_control_spec['iface:interface']['olt'][0]
                olt_module_key = get_module_key(olt_module_spec)
                if not is_vxlan_tunnel_configured(olt_spec['tunnel-mgmt']):
                    # That condition won't passed for dynamic mgmt tunnels that already exists in k8s
                    logger.debug(f'Tunnel mgmt vxlan does not configured for existed k8s {olt_control} swctl. Assign new values from pool...')
                    if 'vxlan' not in olt_spec['tunnel-mgmt'] or 'vni' not in olt_spec['tunnel-mgmt']['vxlan']:
                        assign_vni_from_pool(olt_spec['tunnel-mgmt'], module_vni_pools[olt_module_key])
                    if 'vxlan' not in olt_spec['tunnel-mgmt'] or 'src-ip' not in olt_spec['tunnel-mgmt']['vxlan']:
                        assign_vxlan_tunnel_src_ip_from_pool(olt_spec['tunnel-mgmt'], tunnel_mgmt_vxlan_src_ip_pool)
                    # Updates other tunnel data from global spec
                    update_tunnel_from_global_vxlan_cfg(olt_spec['tunnel-mgmt'], olt_control_global_spec['tunnel-mgmt'])
                for pon_spec in olt_spec['pon']:
                    pon_port = pon_spec['olt-pon-port']
                    if not is_vxlan_tunnel_configured(pon_spec['tunnel-data']) and pon_spec['tunnel-data'] is not None:
                        logger.debug(f'Tunnel data vxlan for pon {pon_port} does not configured for existed k8s {olt_control} swctl. Assign new values from pool...')
                        # That condition won't passed for dynamic data tunnels that already exists in k8s
                        if 'vxlan' not in pon_spec['tunnel-data'] or 'vni' not in pon_spec['tunnel-data']['vxlan']:
                            assign_vni_from_pool(pon_spec['tunnel-data'], module_vni_pools[olt_module_key])
                        if 'vxlan' not in pon_spec['tunnel-data'] or 'src-ip' not in pon_spec['tunnel-data']['vxlan']:
                            assign_vxlan_tunnel_src_ip_from_pool(pon_spec['tunnel-data'], tunnel_data_vxlan_src_ip_pool)
                        # Updates other tunnel data from global spec
                        if bng_data_global_spec and 'tunnel-data' in bng_data_global_spec:
                            update_tunnel_from_global_vxlan_cfg(pon_spec['tunnel-data'], bng_data_global_spec['tunnel-data'])
                    if 's-tag' not in pon_spec and pon_spec['tunnel-data'] is not None:
                        logger.debug(f'Pon {pon_port} Stag does not configured for existed k8s {olt_control} swctl. Assign new values from pool...')
                        assign_stag_from_pool(pon_spec, module_stag_pools[olt_module_key])
                        mark_tunnel_dynamic(pon_spec['tunnel-data'])
        except Exception as e:
            logger.debug(f'Failed to create spec for {olt_control}. k8s swctl will removed/not created. Error: {e}')
            failed_olt_specs.add(olt_control)

    for olt_control in update:
        if olt_control in failed_olt_specs:
            k8s_delete_swctl(k8s_olt_controls[olt_control])
            psql_delete_control(olt_control)
        else:
            k8s_olt_control = k8s_olt_controls[olt_control]
            k8s_olt_control_spec = k8s_olt_control.get('spec')
            olt_control_spec = olt_control_specs[olt_control]
            # When current spec is updated with vni/stag values from pools we are able to
            # update k8s spec if it is changed
            if olt_control_spec != k8s_olt_control_spec:
                logger.debug(f'Updating: {olt_control}')
                k8s_update_swctl_spec(k8s_olt_control, olt_control_spec)
            else:
                logger.debug(f'Ignorring: {olt_control} spec does not changed')

    for olt_control in create:
        if olt_control not in failed_olt_specs:
            olt_control_spec = olt_control_specs[olt_control]
            logger.debug(f'Creating: {olt_control}')
            k8s_create('software-controls', 'SoftwareControl', olt_control, olt_control_spec)

    return list(failed_olt_specs)

def apply_module_mgmt(module_mgmt_specs, olt_control_specs, failed_olt_specs, k8s_software_controls):
    logger.info('Applying module-mgmt')

    k8s_module_mgmts = {w['metadata']['name']: w for w in
                        k8s_software_controls
                        if w['metadata']['name'].startswith('module-mgmt')}

    old = set(k8s_module_mgmts.keys())
    new = set(module_mgmt_specs.keys())
    delete = old - new
    create = new - old
    update = new & old

    # Update module-mgmt spec with pool based assigned vxlan tunnels OLTs
    for module_mgmt in module_mgmt_specs.keys():
        if 'olt' in module_mgmt_specs[module_mgmt]['iface:interface']:
            module_mgmt_olts = module_mgmt_specs[module_mgmt]['iface:interface']['olt']
            for i, mm_olt_spec in enumerate(module_mgmt_olts):
                for olt_control, olt_control_spec in olt_control_specs.items():
                    olt_spec = olt_control_spec['iface:interface']['olt'][0]
                    if mm_olt_spec['v-chassis'] == olt_spec['v-chassis'] and \
                    mm_olt_spec['v-slot'] == olt_spec['v-slot'] and \
                    mm_olt_spec['v-slot-port'] == olt_spec['v-slot-port']:
                        if olt_control in failed_olt_specs:
                            del module_mgmt_olts[i]
                        else:
                            module_mgmt_olts[i] = olt_spec

    for module_mgmt in delete:
        logger.info(f'Deleting {module_mgmt}')
        k8s_delete_swctl(k8s_module_mgmts[module_mgmt])
        psql_delete_module_mgmt(k8s_module_mgmts[module_mgmt])
    for module_mgmt in update:
        if module_mgmt_specs[module_mgmt] != k8s_module_mgmts[module_mgmt].get('spec'):
            logger.info(f'Updating {module_mgmt}')
            k8s_update_swctl_spec(k8s_module_mgmts[module_mgmt], module_mgmt_specs[module_mgmt])
    for module_mgmt in create:
        logger.info(f'Creating {module_mgmt}')
        k8s_create('software-controls', 'SoftwareControl', module_mgmt, module_mgmt_specs[module_mgmt])

def apply_bng_pgs(wls_created, wls_deleted):
    apollo_wl_per_pg_limit = 2
    redundancy = int(os.environ.get('DEFAULT_REDUNDANCY_POLICY', 1))
    if  "DEFAULT_PG_SIZE" in os.environ:
        apollo_wl_per_pg_limit = int(os.environ['DEFAULT_PG_SIZE'])

    def generate_pg_spec(wls):
        return {'redundancyPolicy': redundancy, 'release': 'default',
            'workloads': [{'name': f'bng-workload-{wl}'} for wl in sorted(wls)]}

    k8s_bng_pgs = {
        int(re.search(r'\d+', pg['metadata']['name']).group()): {int(re.search(r'\d+', wl['name']).group()) for wl in pg['spec']['workloads']}
        for pg in k8s_list_custom_object('bng-protectiongroups')
    }

    k8s_new_pgs = defaultdict(set, deepcopy(k8s_bng_pgs))

    # deleting wls and adding wls to existing pgs
    for pg, old_wls in k8s_bng_pgs.items():
        for wl in old_wls & wls_deleted:
            k8s_new_pgs[pg].remove(wl)
        for i in range(apollo_wl_per_pg_limit - len(k8s_new_pgs[pg])):
            if wls_created:
                k8s_new_pgs[pg].add(wls_created.pop(0))
        if len(k8s_new_pgs[pg]) == 0:
            logger.info(f'Deleting bng-pg-{pg}')
            k8s_delete_bngpg(f"bng-pg-{pg}")
        elif k8s_new_pgs[pg] != old_wls:
            logger.info(f'Updating bng-pg-{pg}')
            k8s_update_bngpg_spec(f"bng-pg-{pg}", generate_pg_spec(k8s_new_pgs[pg]))

    # adding wls to new pgs
    max_pg_idx = max(k8s_bng_pgs.keys(), default=0)
    inverleaved_empty_pg_idxs = SortedSet(range(0,max_pg_idx+1)) - k8s_bng_pgs.keys()
    min_idx_vacant_pg = inverleaved_empty_pg_idxs.pop(0) if inverleaved_empty_pg_idxs else max_pg_idx+1
    for i, wl in enumerate(wls_created):
        if len(k8s_new_pgs[min_idx_vacant_pg]) < apollo_wl_per_pg_limit:
            k8s_new_pgs[min_idx_vacant_pg].add(wl)
        if len(k8s_new_pgs[min_idx_vacant_pg]) >= apollo_wl_per_pg_limit or i == len(wls_created)-1:
            logger.info(f'Creating bng-pg-{min_idx_vacant_pg}')
            k8s_create('bng-protectiongroups', 'BngProtectiongroup', f"bng-pg-{min_idx_vacant_pg}", generate_pg_spec(k8s_new_pgs[min_idx_vacant_pg]))
            if inverleaved_empty_pg_idxs:
                min_idx_vacant_pg = inverleaved_empty_pg_idxs.pop(0)
            elif min_idx_vacant_pg <= max_pg_idx:
                min_idx_vacant_pg = max_pg_idx+1
            else:
                min_idx_vacant_pg += 1

def apply_bng_workload(bng_workload_specs):
    logger.info('Applying bng-workload')

    k8s_bng_workloads = {w['metadata']['name']: w for w in
                     k8s_list_custom_object('bng-workloads')
                     if w['metadata']['name'].startswith('bng-workload-')}

    old = set(k8s_bng_workloads.keys())
    new = set(bng_workload_specs.keys())
    delete = old - new
    create = new - old
    update = new & old

    for bng in delete:
        logger.info(f'Deleting {bng}')
        k8s_delete_bngwl(k8s_bng_workloads[bng]['metadata']['name'])
    for bng in update:
        if bng_workload_specs[bng] != k8s_bng_workloads[bng].get('spec'):
            logger.info(f'Updating {bng}')
            k8s_update_bngwl_spec(k8s_bng_workloads[bng]['metadata']['name'], bng_workload_specs[bng])
    for bng in create:
        logger.info(f'Creating {bng}')
        k8s_create('bng-workloads', 'BngWorkload', bng, bng_workload_specs[bng])

    apply_bng_pgs(
        SortedSet({int(re.search(r'\d+', wl).group()) for wl in create}),
        SortedSet({int(re.search(r'\d+', wl).group()) for wl in delete})
    )

def apply_svc_prov_app(svc_prov_app_specs, k8s_software_controls):
    logger.info('Applying service-provisioning-application')

    k8s_svc_prov_apps = {w['metadata']['name']: w for w in
                         k8s_software_controls
                         if w['metadata']['name'].startswith('svc-prov-app')}

    old = set(k8s_svc_prov_apps.keys())
    new = set(svc_prov_app_specs.keys())
    delete = old - new
    for spa_key,spa in svc_prov_app_specs.items():
        non_lb_spa = ((spa['iface:interface']['service-provisioning'].get('interface') is None or
                       spa['iface:interface']['service-provisioning']['interface'].get('all') is not None) and
                       spa['iface:interface']['service-provisioning'].get('service-provisioning-id') == 1)
        if (spa['iface:interface']['service-provisioning'].get('handled-olts') is None and not non_lb_spa):
            delete.add(spa_key)

    create = new - old
    update = (new & old) - delete

    for spa_to_delete in delete:
        client = VcmCtlAsyncClient()
        spaDnsName = f"{spa_to_delete}.{os.environ.get('MY_POD_NAMESPACE', 'default')}.svc.cluster.local"
        loop = asyncio.get_event_loop()
        loop.run_until_complete(client.connectToVcmCtl(spaDnsName))
        loop.run_until_complete(client.CleanupVcmCtl())
        logger.info(f'Deleting {spa_to_delete}')
        k8s_delete_swctl(k8s_svc_prov_apps[spa_to_delete])
        psql_delete_spa(spa_to_delete)
    for spa_to_update in update:
        if svc_prov_app_specs[spa_to_update] != k8s_svc_prov_apps[spa_to_update].get('spec'):
            logger.info(f'Updating {spa_to_update}')
            k8s_update_swctl_spec(k8s_svc_prov_apps[spa_to_update], svc_prov_app_specs[spa_to_update])
    for spa_to_create in create:
        logger.info(f'Creating {spa_to_create}')
        k8s_create('software-controls', 'SoftwareControl', spa_to_create, svc_prov_app_specs[spa_to_create])

def get_data_for_olt_distribution(iface, modules, spa_list, olt_list, k8s_software_controls):

    spa_key_list = ['svc-prov-app-{}'.format(spa['service-provisioning-id']) for spa in spa_list]
    k8s_svc_prov_apps_old = {w['metadata']['name']: w for w in
                             k8s_software_controls
                             if w['metadata']['name'].startswith('svc-prov-app')}
    spa_old = set(k8s_svc_prov_apps_old.keys())
    spa_cfg = set(spa_key_list)

    olt_key_list = ['olt-control-{}-{}-{}'.format(olt['v-chassis'], olt['v-slot'], olt['v-slot-port']) for olt in olt_list]

    for module in modules:
        olt_key = get_olt_key_for_module(module, iface)
        if olt_key is not None:
            olt_key_list.append(f"olt-control-{olt_key['v-chassis']}-{olt_key['v-slot']}-{olt_key['v-slot-port']}")

    k8s_olt_controls_old = {w['metadata']['name']: w for w in
                            k8s_software_controls
                            if w['metadata']['name'].startswith('olt-control-')}
    olt_old = set(k8s_olt_controls_old.keys())
    olt_new = set(olt_key_list)
    delete_olt = olt_old - olt_new

    return OltDistributionData(k8s_olt_controls_old, olt_old, delete_olt, k8s_svc_prov_apps_old, spa_cfg)

def get_olt_max_pon_port_count(type):
    max_pier_pon_port_count = 16
    max_pearl_pon_port_count = 8

    if type == "harmonic-pier":
        return max_pier_pon_port_count
    elif type == "harmonic-pearl":
        return max_pearl_pon_port_count
    # default value for other types
    return 1

def distribute_olts_between_spa(olt_control_specs, svc_prov_app_specs, olts_to_assign_spa):
    def get_pon_port_count_from_olt_key(olt_key):
        return get_olt_max_pon_port_count(olt_control_specs[olt_key]['iface:interface']['olt'][0]['type']) if olt_key in olt_control_specs.keys() else 1

    def get_pon_port_count_from_olt(olt):
        return get_pon_port_count_from_olt_key('olt-control-{}-{}-{}'.format(olt['v-chassis'], olt['v-slot'], olt['v-slot-port']))

    def get_spa_key_with_less_pon_ports(svc_prov_app_pon_port_count):
        return min(svc_prov_app_pon_port_count, key=svc_prov_app_pon_port_count.get) if svc_prov_app_pon_port_count else None

    # fit large first
    olts_to_assign_spa.sort(key=lambda olt: get_pon_port_count_from_olt_key(olt), reverse=True)

    svc_prov_app_pon_port_count = {}
    for spa_key in svc_prov_app_specs:
        if svc_prov_app_specs[spa_key]['iface:interface']['service-provisioning'].get('handled-olts') is None:
            svc_prov_app_pon_port_count[spa_key] = 0
        else:
            svc_prov_app_pon_port_count[spa_key] = sum(map(get_pon_port_count_from_olt, svc_prov_app_specs[spa_key]['iface:interface']['service-provisioning']['handled-olts']))

    for olt_key in olts_to_assign_spa:
        spa_key_with_less_pon_ports = get_spa_key_with_less_pon_ports(svc_prov_app_pon_port_count)
        if spa_key_with_less_pon_ports:
            if svc_prov_app_specs[spa_key_with_less_pon_ports]['iface:interface']['service-provisioning'].get('handled-olts') is None:
                svc_prov_app_specs[spa_key_with_less_pon_ports]['iface:interface']['service-provisioning']['handled-olts'] = [{'v-chassis' : olt_key.split('-')[2], 'v-slot' : olt_key.split('-')[3], 'v-slot-port' : olt_key.split('-')[4]}]
            else:
                svc_prov_app_specs[spa_key_with_less_pon_ports]['iface:interface']['service-provisioning']['handled-olts'].append(
                    {'v-chassis' : olt_key.split('-')[2], 'v-slot' : olt_key.split('-')[3], 'v-slot-port' : olt_key.split('-')[4]})
            olt_control_specs[olt_key]['iface:interface']['olt'][0]['svc-prov-app-name'] = f"{spa_key_with_less_pon_ports}.{os.environ.get('MY_POD_NAMESPACE', 'default')}.svc.cluster.local"
            olt_control_specs[olt_key]['iface:interface']['olt'][0]['svc-prov-app-id'] = svc_prov_app_specs[spa_key_with_less_pon_ports]['iface:interface']['service-provisioning'].get('service-provisioning-id')
            svc_prov_app_pon_port_count[spa_key_with_less_pon_ports] += get_pon_port_count_from_olt_key(olt_key)
        else:
            olt_control_specs[olt_key]['iface:interface']['olt'][0]['svc-prov-app-name'] = None
            olt_control_specs[olt_key]['iface:interface']['olt'][0]['svc-prov-app-id'] = None

def store_ip(ip_address, bundle_nsi_pool, pool_tag, address_tag, is_static):
    if ip_address is not None:
        ip_pool = bundle_nsi_pool.get(pool_tag)
        if ip_pool is not None:
            if ip_address in ip_pool:
                bundle_nsi_pool[address_tag] = ip_address
                ip_pool.remove(ip_address)
                logger.debug('IP for {}({}) stored, value: {}'.format(address_tag, bundle_nsi_pool['pool_id'], ip_address))
            elif is_static:
                bundle_nsi_pool[address_tag] = ip_address
                logger.debug('IP for {}({}) stored (static, is not in pool), value: {}'
                    .format(address_tag, bundle_nsi_pool['pool_id'], ip_address))
            else:
                logger.debug('IP for {}({}) ignored (is not in pool), value: {}'
                    .format(address_tag, bundle_nsi_pool['pool_id'], ip_address))

# returns assigned IP address and IP pool existence
def get_ip_address(bundle_nsi_pool, pool_tag, address_tag):
    ip_address = bundle_nsi_pool.get(address_tag)
    if ip_address is not None:
        logger.debug('IP for {}({}) found, value: {}'.format(address_tag, bundle_nsi_pool['pool_id'], ip_address))
        return ip_address, False

    ip_pool = bundle_nsi_pool.get(pool_tag)
    if ip_pool is None:
        # no start-ip-address - no pool, IP assignment doesn't needed
        logger.debug('Pool {}({}) not found'.format(pool_tag, bundle_nsi_pool['pool_id']))
        return None, False

    if ip_pool:
        ip_address = ip_pool.pop()
        bundle_nsi_pool[address_tag] = ip_address

    logger.debug('IP for {} assigned from {}({}) pool, value: {}'.format(address_tag, pool_tag, bundle_nsi_pool['pool_id'], ip_address))
    return ip_address, True

def allocate_nsi_ips(bng_workload_specs, svc_prov_app_specs, bundle_nsi_pools, k8s_software_controls):
    logger.debug(f'NSI pools, config IPs: {bundle_nsi_pools}')

    k8s_bng_workloads = k8s_list_custom_object('bng-workloads')
    for k8s_bng_workload in k8s_bng_workloads:
        # we'll check only CRDs that remain after new specs applying
        if k8s_bng_workload['metadata']['name'] in bng_workload_specs:
            logger.debug('Processing CRD: {}'.format(k8s_bng_workload['metadata']['name']))
            bng = k8s_bng_workload['spec']['iface:interface']['bng'][0]
            bundle = k8s_bng_workload['spec']['iface:interface']['bundle'][0]

            bng_v4 = 'bng_{}_v4'.format(bng['bng-id'])
            bng_v6 = 'bng_{}_v6'.format(bng['bng-id'])

            ip_pool_id = bundle['cre-vlan-id']
            if ip_pool_id in bundle_nsi_pools:
                bundle_nsi_pool = bundle_nsi_pools[ip_pool_id]
                # if some of adresses already stored - they configured statically, CRD IPs should be ignored
                if bng_v4 not in bundle_nsi_pool and bng_v6 not in bundle_nsi_pool:
                    store_ip(bng.get('nsi-ip-address'), bundle_nsi_pool, 'v4_pool', bng_v4, False)
                    store_ip(bng.get('nsi-ipv6-address'), bundle_nsi_pool, 'v6_pool', bng_v6, False)

            if 'sub-bundle' in k8s_bng_workload['spec']['iface:interface'] and 'sub-interface' in bng:
                sub_bundles = k8s_bng_workload['spec']['iface:interface']['sub-bundle']
                sub_interfaces = bng['sub-interface']
                for sub_bundle in sub_bundles:
                    ip_pool_id = sub_bundle['cre-vlan-id']
                    if ip_pool_id in bundle_nsi_pools:
                        bundle_nsi_pool = bundle_nsi_pools[ip_pool_id]
                        if bng_v4 not in bundle_nsi_pool and bng_v6 not in bundle_nsi_pool:
                            for sub_interface in sub_interfaces:
                                if sub_bundle['sub-bundle-index'] == sub_interface['sub-bundle-index-ref']:
                                    store_ip(sub_interface.get('nsi-ip-address'), bundle_nsi_pool, 'v4_pool', bng_v4, False)
                                    store_ip(sub_interface.get('nsi-ipv6-address'), bundle_nsi_pool, 'v6_pool', bng_v6, False)

    for k8s_software_control in k8s_software_controls:
        if k8s_software_control['metadata']['name'] in svc_prov_app_specs:
            logger.debug('Processing CRD: {}'.format(k8s_software_control['metadata']['name']))
            spa = k8s_software_control['spec']['iface:interface']['service-provisioning']
            bundle = k8s_software_control['spec']['iface:interface']['bundle'][0]

            spa_v4 = 'spa_{}_v4'.format(spa['service-provisioning-id'])
            spa_v6 = 'spa_{}_v6'.format(spa['service-provisioning-id'])

            ip_pool_id = bundle['cre-vlan-id']
            if ip_pool_id in bundle_nsi_pools:
                bundle_nsi_pool = bundle_nsi_pools[ip_pool_id]
                if spa_v4 not in bundle_nsi_pool and spa_v6 not in bundle_nsi_pool:
                    store_ip(spa.get('nsi-ip-address'), bundle_nsi_pool, 'v4_pool', spa_v4, False)
                    store_ip(spa.get('nsi-ipv6-address'), bundle_nsi_pool, 'v6_pool', spa_v6, False)

    logger.debug(f'NSI pools, config IPs, CRD IPs: {bundle_nsi_pools}')

    for bng_workload_spec in bng_workload_specs.values():
        bng = bng_workload_spec['iface:interface']['bng'][0]
        bundle = bng_workload_spec['iface:interface']['bundle'][0]

        logger.debug('Processing spec for BNG {}'.format(bng['bng-id']))
        bng_v4 = 'bng_{}_v4'.format(bng['bng-id'])
        bng_v6 = 'bng_{}_v6'.format(bng['bng-id'])
        ip_pool_id = bundle['cre-vlan-id']

        if bng.get('nsi-ip-address') is not None or bng.get('nsi-ipv6-address') is not None:
            logger.debug('Static IP(s) configured for BNG {} bundle {}'.format(bng['bng-id'], bundle['bundle-index']))
        else:
            # process only if no static addresses set
            ip_assign_success = False
            if ip_pool_id in bundle_nsi_pools:
                v4_ip, pool_exists =  get_ip_address(bundle_nsi_pools[ip_pool_id], 'v4_pool', bng_v4)
                if v4_ip is None:
                    ip_assign_success = not pool_exists
                else:
                    ip_assign_success = True
                    bng['nsi-ip-address'] = v4_ip

            if not ip_assign_success:
                raise ValueError("Can't allocate NSI IPv4 address for BNG {} bundle {}".format(bng['bng-id'], bundle['bundle-index']))

            ip_assign_success = False
            if ip_pool_id in bundle_nsi_pools:
                v6_ip, pool_exists =  get_ip_address(bundle_nsi_pools[ip_pool_id], 'v6_pool', bng_v6)
                if v6_ip is None:
                    ip_assign_success = not pool_exists
                else:
                    ip_assign_success = True
                    bng['nsi-ipv6-address'] = v6_ip

            if not ip_assign_success:
                raise ValueError("Can't allocate NSI IPv6 address for BNG {} bundle {}".format(bng['bng-id'], bundle['bundle-index']))

        if 'sub-bundle' in bng_workload_spec['iface:interface'] and 'sub-interface' in bng:
            sub_bundles = bng_workload_spec['iface:interface']['sub-bundle']
            sub_interfaces = bng['sub-interface']
            for sub_interface in sub_interfaces:
                if sub_interface.get('nsi-ip-address') is not None or sub_interface.get('nsi-ipv6-address') is not None:
                    logger.debug('Static IP(s) configured for BNG {} sub-bundle {}.{}'
                        .format(bng['bng-id'], bundle['bundle-index'], sub_interface['sub-bundle-index-ref']))
                else:
                    ip_assign_success = False
                    for sub_bundle in sub_bundles:
                        if sub_bundle['sub-bundle-index'] == sub_interface['sub-bundle-index-ref']:
                            ip_pool_id = sub_bundle['cre-vlan-id']
                            if ip_pool_id in bundle_nsi_pools:
                                v4_ip, pool_exists =  get_ip_address(bundle_nsi_pools[ip_pool_id], 'v4_pool', bng_v4)
                                if v4_ip is None:
                                    ip_assign_success = not pool_exists
                                else:
                                    ip_assign_success = True
                                    sub_interface['nsi-ip-address'] = v4_ip

                    if not ip_assign_success:
                        raise ValueError("Can't allocate NSI IPv4 address for BNG {} sub-bundle {}.{}"
                            .format(bng['bng-id'], bundle['bundle-index'], sub_interface['sub-bundle-index-ref']))

                    ip_assign_success = False
                    for sub_bundle in sub_bundles:
                        if sub_bundle['sub-bundle-index'] == sub_interface['sub-bundle-index-ref']:
                            ip_pool_id = sub_bundle['cre-vlan-id']
                            if ip_pool_id in bundle_nsi_pools:
                                v6_ip, pool_exists =  get_ip_address(bundle_nsi_pools[ip_pool_id], 'v6_pool', bng_v6)
                                if v6_ip is None:
                                    ip_assign_success = not pool_exists
                                else:
                                    ip_assign_success = True
                                    sub_interface['nsi-ipv6-address'] = v6_ip

                    if not ip_assign_success:
                        raise ValueError("Can't allocate NSI IPv6 address for BNG {} sub-bundle {}.{}"
                            .format(bng['bng-id'], bundle['bundle-index'], sub_interface['sub-bundle-index-ref']))

    for svc_prov_app_spec in svc_prov_app_specs.values():
        spa = svc_prov_app_spec['iface:interface']['service-provisioning']
        logger.debug('spec_value: {spa}')
        bundle = svc_prov_app_spec['iface:interface']['bundle'][0]

        logger.debug('Processing spec for SPA {}'.format(spa['service-provisioning-id']))
        spa_v4 = 'spa_{}_v4'.format(spa['service-provisioning-id'])
        spa_v6 = 'spa_{}_v6'.format(spa['service-provisioning-id'])
        ip_pool_id = bundle['cre-vlan-id']

        if spa.get('nsi-ip-address') is not None or spa.get('nsi-ipv6-address') is not None:
            logger.debug('Static IP(s) configured for SPA {} bundle {}'.format(spa['service-provisioning-id'], bundle['bundle-index']))
        else:
            ip_provisioning_mode = spa.get('ip-provisioning-mode', 'ipv4-only')
            logger.debug(f'Provisioning mode: {ip_provisioning_mode}')

            if ip_provisioning_mode == 'ipv4-only':
                ip_assign_success = False
                if ip_pool_id in bundle_nsi_pools:
                    v4_ip, pool_exists =  get_ip_address(bundle_nsi_pools[ip_pool_id], 'v4_pool', spa_v4)
                    if v4_ip is None:
                        ip_assign_success = not pool_exists
                    else:
                        ip_assign_success = True
                        spa['nsi-ip-address'] = v4_ip

                if not ip_assign_success:
                    raise ValueError("Can't allocate NSI IPv4 address for SPA {} bundle {}"
                        .format(spa['service-provisioning-id'], bundle['bundle-index']))

            if ip_provisioning_mode == 'ipv6-only':
                ip_assign_success = False
                if ip_pool_id in bundle_nsi_pools:
                    v6_ip, pool_exists =  get_ip_address(bundle_nsi_pools[ip_pool_id], 'v6_pool', spa_v6)
                    if v6_ip is None:
                        ip_assign_success = not pool_exists
                    else:
                        ip_assign_success = True
                        spa['nsi-ipv6-address'] = v6_ip

                if not ip_assign_success:
                    raise ValueError("Can't allocate NSI IPv6 address for SPA {} bundle {}"
                        .format(spa['service-provisioning-id'], bundle['bundle-index']))

# DOCSIS and PON use shared IP pool
# DOCSIS use addresses from pool start
# we start allocate addresses for PON from pool end to minimize DOCSIS/PON parts overlap
# and avoid reconfiguration when pods count changed
def create_nsi_ip_pool(ip_pool_id, bundle_id, sub_bundle_id, nsi, docsis_ip_pool_size):
    full_ip_pool_id = ''
    if sub_bundle_id:
        full_ip_pool_id = f'v{ip_pool_id}-sb{bundle_id}.{sub_bundle_id}'
    else:
        full_ip_pool_id = f'v{ip_pool_id}-b{bundle_id}'
    ip_pool = { 'pool_id' : full_ip_pool_id }

    if nsi is None:
        return ip_pool

    ip_pool_size = nsi.get('pool-count', DEFAULT_NSI_POOL_COUNT)
    if ip_pool_size is None:
        return ip_pool

    start_ipv4_address = nsi.get('start-ipv4-address')
    if start_ipv4_address is not None:
        ip_pool['v4_pool'] = [str(ipaddress.IPv4Address(start_ipv4_address) + i) for i in range(docsis_ip_pool_size, ip_pool_size)]

    start_ipv6_address = nsi.get('start-ipv6-address')
    if start_ipv6_address is not None:
        ip_pool['v6_pool'] = [str(ipaddress.IPv6Address(start_ipv6_address) + i) for i in range(docsis_ip_pool_size, ip_pool_size)]

    return ip_pool

def get_olt_key_for_module(module, iface):
    if 'type' not in module:
        return None

    if not is_module_cfg_managable(module, iface):
        return None

    pon_ifaces = list(filter(lambda pon_iface: pon_iface['v-chassis'] == module['v-chassis'] and pon_iface['v-slot'] == module['v-slot'],
                             deepcopy(get(iface, 'cosm-cli-iface-config:interface/pon'))))
    if not pon_ifaces:
        # At least one 'interface pon' must be present
        return None

    iface_olt_exists = any((olt['v-chassis'] == module['v-chassis'] and
                            olt['v-slot'] == module['v-slot'] and
                            olt['v-slot-port'] == pon_ifaces[0]['v-slot-port']) for olt in deepcopy(get(iface, 'cosm-cli-iface-config:interface/olt')))
    if iface_olt_exists:
        return None

    is_fin = module['type'] not in ['harmonic-pier', 'harmonic-pearl']

    return {'v-chassis': module['v-chassis'],
            'v-slot': module['v-slot'],
            'v-slot-port': pon_ifaces[0]['v-slot-port'] if is_fin else 0 }

def transform_module_and_pon_into_olt(olt_key, module, iface):
    olt = {}
    olt['v-chassis'] = olt_key['v-chassis']
    olt['v-slot'] = olt_key['v-slot']
    olt['v-slot-port'] = olt_key['v-slot-port']

    # from module spec
    olt['admin-state'] = module['admin-state']
    olt['type'] = 'harmonic-fin' if module['type'] not in ['harmonic-pier', 'harmonic-pearl'] else module['type']
    if 'name' in module:
        olt['name'] = module['name']

    # defaults
    olt['fw-version'] = 'default'
    olt['relay-state'] = 'disable'
    olt['tunnel-mgmt'] = {'vxlan': {}}

    olt['pon'] = []

    # pon
    pon_ifaces = list(filter(lambda pon_iface: pon_iface['v-chassis'] == module['v-chassis'] and pon_iface['v-slot'] == module['v-slot'],
                             deepcopy(get(iface, 'cosm-cli-iface-config:interface/pon'))))
    for pon_id, pon in enumerate(pon_ifaces, start=1):
        olt_pon = deepcopy(pon)
        olt_pon.pop('v-slot-port', None)

        # from pon spec
        olt_pon['sfp-port-num'] = pon['v-slot-port']
        olt_pon['pon-index'] = pon['olt-pon-port']

        # defaults
        olt_pon['provisioning-template'] = 'default-template'
        if 'traffic-mode' in module and module['traffic-mode'] == 'external':
            olt_pon['tunnel-data'] = None
            logger.debug('skipping default tunnel-data initialization for external traffic mode')
        else:
            olt_pon['tunnel-data'] = {'vxlan': {}}

        olt_pon['olt-pon-port'] = pon_id
        olt['pon'].append(olt_pon)

    return olt
def create_single_spa_spec(oltDistributionData, cli_spa_config, olts_to_assign_spa):
    # create single spa spec from cli_spa_config
    svc_prov_app_specs = {}
    cfg_spa_id = cli_spa_config['service-provisioning-id']
    cfg_spa_key = 'svc-prov-app-{}'.format(cfg_spa_id)
    svc_prov_app_specs[cfg_spa_key] = {
    'iface:interface': {'service-provisioning': deepcopy(cli_spa_config)}
    }
    if cfg_spa_key in oltDistributionData.k8s_svc_prov_apps_old:
        old_handled_olts = oltDistributionData.k8s_svc_prov_apps_old[cfg_spa_key]['spec']['iface:interface']['service-provisioning'].get('handled-olts')
        if old_handled_olts is not None:
            new_handled_olts = [olt for olt in old_handled_olts if 'olt-control-{}-{}-{}'.format(olt['v-chassis'], olt['v-slot'], olt['v-slot-port']) not in oltDistributionData.delete_olt]
            if len(new_handled_olts) != 0:
                svc_prov_app_specs[cfg_spa_key]['iface:interface']['service-provisioning']['handled-olts'] = new_handled_olts
    return svc_prov_app_specs, olts_to_assign_spa

def create_lb_spa_specs(oltDistributionData, cli_spa_config, olts_to_assign_spa, module_types):
    def get_pon_port_count_from_olt_key(olt_key):
        return get_olt_max_pon_port_count(module_types[olt_key]) if olt_key in module_types.keys() else 1

    def get_pon_port_count_from_olt(olt):
        return get_pon_port_count_from_olt_key('olt-control-{}-{}-{}'.format(olt['v-chassis'], olt['v-slot'], olt['v-slot-port']))

    def is_transition_to_load_balance():
        if oltDistributionData.k8s_svc_prov_apps_old:
            old_spa_spec = oltDistributionData.k8s_svc_prov_apps_old[next(iter(oltDistributionData.k8s_svc_prov_apps_old))]
            if 'interface' in old_spa_spec['spec']['iface:interface']['service-provisioning'] and \
               old_spa_spec['spec']['iface:interface']['service-provisioning']['interface'].get('load-balance') is not None:
                return False
        return True

    def get_max_pon_ports_per_pod(new_pon_ports_count):
        all_pon_ports_count = new_pon_ports_count if new_pon_ports_count else 0
        # count average pon ports per spa
        for spa_key, spa in oltDistributionData.k8s_svc_prov_apps_old.items():
            old_handled_olts = spa['spec']['iface:interface']['service-provisioning'].get('handled-olts')
            if old_handled_olts is not None:
                for olt in old_handled_olts:
                    if 'olt-control-{}-{}-{}'.format(olt['v-chassis'], olt['v-slot'], olt['v-slot-port']) not in oltDistributionData.delete_olt:
                        all_pon_ports_count += get_pon_port_count_from_olt(olt)

        if all_pon_ports_count > max_pon_ports_per_pod*max_spa_pods:
            return math.ceil(all_pon_ports_count/max_spa_pods)
        return max_pon_ports_per_pod

    def get_new_spa_pods_for_olts_distribution(new_pods_count, aviable_spa_pods):
        if new_pods_count != aviable_spa_pods:
            # try to distribute olt pon ports between SPA pods
            olts_to_assign_spa.sort(key=lambda olt: get_pon_port_count_from_olt_key(olt), reverse=True)

            for aditional_pods in range(aviable_spa_pods - new_pods_count + 1):
                if new_pods_count + aditional_pods == aviable_spa_pods:
                    new_pods_count += aditional_pods
                    break
                for pod_count in range(new_pods_count + aditional_pods):
                    svc_prov_app_pon_port_count[allowed_ids[pod_count]] = 0

                can_distribute = True
                for olt_key in olts_to_assign_spa:
                    spa_key_with_less_pon_ports = min(svc_prov_app_pon_port_count, key=svc_prov_app_pon_port_count.get)
                    if svc_prov_app_pon_port_count[spa_key_with_less_pon_ports] + get_pon_port_count_from_olt_key(olt_key) > max_pon_ports_per_pod:
                        can_distribute = False
                        break
                    else:
                        svc_prov_app_pon_port_count[spa_key_with_less_pon_ports] += get_pon_port_count_from_olt_key(olt_key)
                if can_distribute:
                    new_pods_count += aditional_pods
                    break
        return new_pods_count


    # this should be equal to 'max_spa_pods' constant in cosm-cli:/views/validation/validation-handlers/NsiPoolCountValidation.cpp
    max_spa_pods = 6
    max_pon_ports_per_pod = 48
    new_pon_ports_count = 0
    available_pon_port_slots = 0
    svc_prov_app_pon_port_count = {}

    # balance OLTs if the transition to load balance
    need_to_balance = is_transition_to_load_balance()

    used_ids = set()
    svc_prov_app_specs = {}
    cfg_spa_id = cli_spa_config['service-provisioning-id']

    new_pon_ports_count += sum(map(get_pon_port_count_from_olt_key, olts_to_assign_spa))
    if need_to_balance:
        # When there will be a maximum number of SPA pods, need to define a new maximum to prevent rebalancing of the assigned OLTs if the SPA pod has more than the existing limit
        # for instance: max spa pods - 6, max pon ports per pod - 48, spa1 = 300, ports to assign - 900;
        #               correct distribution: max ports per pod = 200, should rebalance only 100 ports from spa1
        max_pon_ports_per_pod = get_max_pon_ports_per_pod(new_pon_ports_count)

    logger.debug(f'Maximum PON port per SPA = {max_pon_ports_per_pod}')

    # load-balance-enaled
    for spa_key, spa in oltDistributionData.k8s_svc_prov_apps_old.items():
        logger.debug(f'processing existing spa: {spa}')
        svc_prov_app_pon_port_count[spa_key] = 0
        used_ids.add(spa['spec']['iface:interface']['service-provisioning']['service-provisioning-id'])
        svc_prov_app_specs[spa_key] = {'iface:interface' : { 'service-provisioning' : deepcopy(cli_spa_config)}}
        svc_prov_app_specs[spa_key]['iface:interface']['service-provisioning']['service-provisioning-id'] = spa['spec']['iface:interface']['service-provisioning']['service-provisioning-id']
        old_handled_olts = spa['spec']['iface:interface']['service-provisioning'].get('handled-olts')
        if old_handled_olts is not None:
            new_handled_olts = [olt for olt in old_handled_olts if 'olt-control-{}-{}-{}'.format(olt['v-chassis'], olt['v-slot'], olt['v-slot-port']) not in oltDistributionData.delete_olt]
            new_handled_olts_pon_ports_count =  sum(map(get_pon_port_count_from_olt, new_handled_olts))

            if new_handled_olts_pon_ports_count <= max_pon_ports_per_pod:
                available_pon_port_slots += max_pon_ports_per_pod - new_handled_olts_pon_ports_count
            elif need_to_balance:
                new_handled_olts.sort(key=lambda olt: get_pon_port_count_from_olt(olt))
                for i in range(len(new_handled_olts) - 1, -1, -1):
                    port_count = get_pon_port_count_from_olt(new_handled_olts[i])
                    if new_handled_olts_pon_ports_count - port_count >= max_pon_ports_per_pod:
                        new_handled_olts_pon_ports_count -= port_count
                        olts_to_assign_spa.append('olt-control-{}-{}-{}'.format(new_handled_olts[i]['v-chassis'], new_handled_olts[i]['v-slot'], new_handled_olts[i]['v-slot-port']))
                        new_pon_ports_count += port_count
                        del new_handled_olts[i]
            svc_prov_app_pon_port_count[spa_key] = new_handled_olts_pon_ports_count
            if len(new_handled_olts) != 0:
                svc_prov_app_specs[spa_key]['iface:interface']['service-provisioning']['handled-olts'] = new_handled_olts

    allowed_ids = [i for i in range(cfg_spa_id, cfg_spa_id + max_spa_pods) if i not in used_ids]

    if (available_pon_port_slots < new_pon_ports_count):
        new_pods_count = math.ceil((new_pon_ports_count - available_pon_port_slots) / max_pon_ports_per_pod)
        if new_pods_count > max_spa_pods - len(used_ids):
            new_pods_count = max_spa_pods - len(used_ids)
    else:
        new_pods_count = 0

    # could be a case when we have enough available pon ports to distribute all olts but can't fit them all without new spa pods
    # for instance: max pon ports per pod - 48, spa1 - 33, spa2 - 33, new olt - pearl(16 pon ports), available slots - 34, but can't fit this in existing spa pods
    new_pods_count = get_new_spa_pods_for_olts_distribution(new_pods_count, max_spa_pods - len(used_ids))
    print(f'Need to create {new_pods_count} new pods')

    for pod_count in range(new_pods_count):
        new_id = allowed_ids[pod_count]
        spa_key = 'svc-prov-app-{}'.format(new_id)
        svc_prov_app_specs[spa_key] = {
        'iface:interface': {'service-provisioning': deepcopy(cli_spa_config)}
        }

        svc_prov_app_specs[spa_key]['iface:interface']['service-provisioning']['service-provisioning-id'] = new_id

    return svc_prov_app_specs, olts_to_assign_spa

def get_profiles(profile_names, all_profiles, profile_name_tag):
    if not all_profiles:
        return []

    profiles = []
    for profile in all_profiles:
        if profile[profile_name_tag] in profile_names:
            profiles.append(deepcopy(profile))

    return profiles

def get_multicast_profile_names(olt_spec):
    profile_names = set()
    for pon_spec in olt_spec:
        if 'multicast-profile' in pon_spec:
            profile_refs = pon_spec['multicast-profile']
            if len(profile_refs) > 0:
                profile_ref = profile_refs[0]
                if 'multicast-profile-name' in profile_ref:
                    profile_names.add(profile_ref['multicast-profile-name'])

    return profile_names

def get_broadcast_profile_names(olt_spec):
    profile_names = set()
    for pon_spec in olt_spec:
        if 'broadcast-profile' in pon_spec:
            profile_names.add(pon_spec['broadcast-profile'])

    return profile_names

def split(iface, cable, docsis_ip_pool_size, base_docs_subscriber_management, k8s_software_controls):
    """
    :type iface: dict[str, dict] interface config
    :type cable: dict[str, dict] cable config
    :return: crd_name2crd_spec tuple
    """


    def convert_onu_firmware_update_to_crd(fttx):
        fttx['onu-firmware-update']['secure-software-download']['enable'] = fttx['onu-firmware-update']['secure-software-download']['check-digital-signature']
        del fttx['onu-firmware-update']['secure-software-download']['check-digital-signature']

    rip = deepcopy(get(cable, 'cosm-cli-config:cable/rip'))
    system_proto_throttle = deepcopy(get(cable, 'cosm-cli-config:system/proto-throttle'))
    cable_proto_throttle = deepcopy(get(cable, 'cosm-cli-config:cable/proto-throttle'))
    mcast = deepcopy(get(cable, 'cosm-cli-config:cable/mcast'))
    lawful_intercept = deepcopy(get(cable, 'cosm-cli-config:cable/lawful-intercept'))
    ipsec = deepcopy(get(cable, 'cosm-cli-config:system/ipsec'))
    fttx = deepcopy(get(iface, 'cosm-cli-iface-config:interface/fttx'))
    all_broadcast_profiles = deepcopy(get(iface, 'cosm-cli-iface-config:interface/broadcast-profile'))
    all_multicast_profiles = deepcopy(get(iface, 'cosm-cli-iface-config:interface/multicast-profile'))
    modules = deepcopy(get(iface, 'cosm-cli-iface-config:interface/module'))
    spa_list = deepcopy(get(iface, 'cosm-cli-iface-config:interface/service-provisioning'))
    olts_iface = deepcopy(get(iface, 'cosm-cli-iface-config:interface/olt'))
    ethernet_ifaces = deepcopy(get(iface, 'cosm-cli-iface-config:interface/ethernet'))
    port_channel_ifaces = deepcopy(get(iface, 'cosm-cli-iface-config:interface/port-channel'))

    olt_control_specs = {}
    module_mgmt_specs = {}
    module_mgmt_iface = {
        'module' : [],
    }
    module_mgmt_olts = []
    module_types = {}

    if 'onu-firmware-update' in fttx:
        convert_onu_firmware_update_to_crd(fttx)

    olts_to_assign_spa = []
    olts = {}

    # OLTs configured using interface/olt
    for olt in olts_iface:
        if olt['admin-state'] == 'down':
            continue
        olt_key = f"olt-control-{olt['v-chassis']}-{olt['v-slot']}-{olt['v-slot-port']}"
        olts[olt_key] = olt

    spa_templ = deepcopy(get(iface, 'cosm-cli-iface-config:interface/service-provisioning'))
    spa_present = (len(spa_templ) != 0)
    if spa_present:
        templ_spa_id = spa_templ[0]['service-provisioning-id']
        templ_spa_key = 'svc-prov-app-{}'.format(templ_spa_id)
        load_balance_enabled = False

        if 'interface' in spa_templ[0]:
            load_balance_enabled = spa_templ[0]['interface'].get('load-balance') is not None

        logger.debug(f'Load_balancing enabled? : {load_balance_enabled}')

        olt_distribution_data = get_data_for_olt_distribution(iface, modules, spa_list, list(olts.values()), k8s_software_controls)

    # OLTs configured using interface/pon
    for module in modules:
        olt_key = get_olt_key_for_module(module, iface)
        if olt_key is None:
            continue

        olt = transform_module_and_pon_into_olt(olt_key, module, iface)
        olt_key =f"olt-control-{olt['v-chassis']}-{olt['v-slot']}-{olt['v-slot-port']}"
        olts[olt_key] = olt
        module_types[olt_key] = module['type']

    for olt_key, olt in olts.items():
        if spa_present:
            if olt_key not in olt_distribution_data.olt_old:
                olts_to_assign_spa.append(olt_key)
            elif olt_distribution_data.k8s_olt_controls_old[olt_key]['spec']['iface:interface']['olt'][0].get('svc-prov-app-name') is None:
                olts_to_assign_spa.append(olt_key)
            elif (not load_balance_enabled and (olt_distribution_data.k8s_olt_controls_old[olt_key]['spec']['iface:interface']['olt'][0]['svc-prov-app-name'].split('.'))[0] != templ_spa_key):
                olts_to_assign_spa.append(olt_key)
            else:
                # olt was present in a previous config and is present in a current one
                # and correspondent spa is present in a current config - no rebalance should be done
                olt['svc-prov-app-name'] = olt_distribution_data.k8s_olt_controls_old[olt_key]['spec']['iface:interface']['olt'][0]['svc-prov-app-name']

                spa_id = olt_distribution_data.k8s_olt_controls_old[olt_key]['spec']['iface:interface']['olt'][0].get('svc-prov-app-id')
                # in the old olt config svc-prov-app-id does not exist so get it from svc-prov-app-name
                if spa_id is None:
                    spa_id = next(iter(re.findall('\d+', olt['svc-prov-app-name'])), None)
                olt['svc-prov-app-id'] = spa_id

        module_cfg = get_olt_module(olt, iface)

        # Adds to module-mgmt spec only OLT's module behind them are managable
        if module_cfg is not None and is_module_cfg_managable(module_cfg, iface):
            module_mgmt_olts.append(deepcopy(olt))

        olt_resource = f'olt-control-{olt["v-chassis"]}-{olt["v-slot"]}-{olt["v-slot-port"]}'
        olt_control_specs[olt_resource] = {
            'iface:interface' : {
                'module' : [module_cfg],
                'olt' : [olt],
                'mcast' : mcast,
                'fttx' : fttx,
            }
        }
        # Add bundle and "dummy" BNG configuration which is required during
        # slowpath configuration appling
        if module_cfg is not None and "traffic-mode" in module_cfg and module_cfg['traffic-mode'] == "external":
            # Currently only one bundle supported
            olt_control_specs[olt_resource]['iface:interface']['bundle'] = get(cable, 'cosm-cli-config:cable/bundle')  
            olt_control_specs[olt_resource]['iface:interface']['bng'] = [{
                "bng-id": 1,
                "cable": {
                    "bundle": 1,
                }
            }]

        broadcast_profiles = get_profiles(get_broadcast_profile_names(olt['pon']), all_broadcast_profiles, 'broadcast-profile-name')
        if len(broadcast_profiles) > 0:
            olt_control_specs[f"olt-control-{olt['v-chassis']}-{olt['v-slot']}-{olt['v-slot-port']}"]['iface:interface']['broadcast_profile'] = broadcast_profiles

        multicast_profiles = get_profiles(get_multicast_profile_names(olt['pon']), all_multicast_profiles, 'multicast-profile-name')
        if len(multicast_profiles) > 0:
            olt_control_specs[f"olt-control-{olt['v-chassis']}-{olt['v-slot']}-{olt['v-slot-port']}"]['iface:interface']['multicast_profile'] = multicast_profiles

    if module_mgmt_olts:
        module_mgmt_iface['olt'] = module_mgmt_olts

    for module in modules:
        if is_module_cfg_managable(module, iface):
            module_mgmt_iface['module'].append(deepcopy(module))

    # For now supported only one module-mgmt instance
    # Thats not enough ONLY managable modules to create module-mgmt CRD. Should
    # also exists module-mgmt config
    for module_mgmt in deepcopy(get(iface, 'cosm-cli-iface-config:interface/module-management')):
        module_mgmt_key = 'module-mgmt-{}'.format(module_mgmt['module-management-id'])
        if module_mgmt_iface['module']:
            module_mgmt_specs[module_mgmt_key] = {
                'iface:interface' : module_mgmt_iface
            }
            module_mgmt_specs[module_mgmt_key]['iface:interface']['module-management'] = module_mgmt
        if module_mgmt_key in module_mgmt_specs:
            if len(ethernet_ifaces) > 0:
                module_mgmt_specs[module_mgmt_key]['iface:interface']['ethernet'] = ethernet_ifaces
            if len(port_channel_ifaces) > 0:
                module_mgmt_specs[module_mgmt_key]['iface:interface']['port-channel'] = port_channel_ifaces

    bundle_nsi_pools = {}

    bng_workload_specs = {}
    for bng in deepcopy(get(iface, 'cosm-cli-iface-config:interface/bng')):
        # transform interface pon into interface olt pon
        if 'interface' in bng:
            for pon in bng['interface'].pop('pon', []):
                if 'olt' not in bng['interface']:
                    bng['interface']['olt'] = []

                olt_exists = next(
                    (
                        olt
                        for olt in bng["interface"]["olt"]
                        if (
                            olt["v-chassis"] == pon["v-chassis"]
                            and olt["v-slot"] == pon["v-slot"]
                            and olt["v-slot-port"] == pon["v-slot-port"]
                        )
                    ),
                    None
                )
                if olt_exists is not None:
                    olt_exists["pon"].append({"olt-pon-port": pon["olt-pon-port"]})
                else:
                    bng["interface"]["olt"].append(
                        {
                            "v-chassis": pon["v-chassis"],
                            "v-slot": pon["v-slot"],
                            "v-slot-port": pon["v-slot-port"],
                            "pon": [{"olt-pon-port": pon["olt-pon-port"]}],
                        }
                    )

        multicast_profile_names = set()
        if 'interface' in bng and 'olt' in bng['interface']:
            for bng_olt in bng['interface']['olt']:
                for olt_key, olt in olts.items():
                    if olt["v-chassis"] == bng_olt["v-chassis"] and olt["v-slot"] == bng_olt["v-slot"] and olt["v-slot-port"] == bng_olt["v-slot-port"]:
                        multicast_profile_names.update(get_multicast_profile_names(olt['pon']))
        multicast_profiles = get_profiles(multicast_profile_names, all_multicast_profiles, 'multicast-profile-name')

        bng_workload_key = 'bng-workload-{}'.format(bng['bng-id'])
        bng_workload_specs[bng_workload_key] = {
            'iface:interface' : {
                'bng' : [bng]
            }
        }

        bundles = {}
        if 'cable' in bng:
            bundle_id = bng['cable']['bundle']
            bundles_ = get(cable, 'cosm-cli-config:cable/bundle')

            try:
                bundle_ = next((b for b in bundles_ if b['bundle-index'] == bundle_id))
            except StopIteration: # just in case. CLI prevents this
                raise RuntimeError(f"bng's cable bundle ({bundle_id}) references unknown cable bundle")

            ip_pool_id = bundle_['cre-vlan-id']
            if ip_pool_id not in bundle_nsi_pools:
                bundle_nsi_pools[ip_pool_id] = create_nsi_ip_pool(ip_pool_id, bundle_id, None, bundle_.get('nsi'), docsis_ip_pool_size)

            bng_v4 = 'bng_{}_v4'.format(bng['bng-id'])
            bng_v6 = 'bng_{}_v6'.format(bng['bng-id'])
            bundle_nsi_pool = bundle_nsi_pools[ip_pool_id]
            store_ip(bng.get('nsi-ip-address'), bundle_nsi_pool, 'v4_pool', bng_v4, True)
            store_ip(bng.get('nsi-ipv6-address'), bundle_nsi_pool, 'v6_pool', bng_v6, True)

            sub_bundles = []
            for sub_bundle in get(cable, 'cosm-cli-config:cable/sub-bundle'):
                if sub_bundle['bundle-index-ref'] == bundle_['bundle-index']:
                    sub_bundles.append(sub_bundle)

                    ip_pool_id = sub_bundle['cre-vlan-id']
                    if ip_pool_id not in bundle_nsi_pools:
                        bundle_nsi_pools[ip_pool_id] = create_nsi_ip_pool(ip_pool_id, bundle_id, sub_bundle['sub-bundle-index'],
                            sub_bundle.get('nsi'), docsis_ip_pool_size)

                    if 'sub-interface' in bng_workload_specs[bng_workload_key]['iface:interface']['bng'][0]:
                        sub_interface_found = False
                        for sub_interface in bng_workload_specs[bng_workload_key]['iface:interface']['bng'][0]['sub-interface']:
                            if sub_bundle['sub-bundle-index'] == sub_interface['sub-bundle-index-ref']:
                                sub_interface_found = True

                                bundle_nsi_pool = bundle_nsi_pools[ip_pool_id]
                                store_ip(sub_interface.get('nsi-ip-address'), bundle_nsi_pool, 'v4_pool', bng_v4, True)
                                store_ip(sub_interface.get('nsi-ipv6-address'), bundle_nsi_pool, 'v6_pool', bng_v6, True)
                                break

                        if not sub_interface_found:
                            sub_interface = { 'bundle-index-ref' : sub_bundle['bundle-index-ref'],
                                'sub-bundle-index-ref' : sub_bundle['sub-bundle-index'] }
                            bng_workload_specs[bng_workload_key]['iface:interface']['bng'][0]['sub-interface'].append(sub_interface)
                            logger.debug('Sub-interface {}.{} added for BNG {}'.format(sub_bundle['bundle-index-ref'],
                                sub_bundle['sub-bundle-index'], bng_workload_specs[bng_workload_key]['iface:interface']['bng'][0]['bng-id']))
                    else:
                        sub_interface = { 'bundle-index-ref' : sub_bundle['bundle-index-ref'],
                            'sub-bundle-index-ref' : sub_bundle['sub-bundle-index'] }
                        bng_workload_specs[bng_workload_key]['iface:interface']['bng'][0]['sub-interface'] = [ sub_interface ]
                        logger.debug('Sub-interfaces created, sub-interface {}.{} added for BNG {}'.format(sub_bundle['bundle-index-ref'],
                            sub_bundle['sub-bundle-index'], bng_workload_specs[bng_workload_key]['iface:interface']['bng'][0]['bng-id']))

            bundles = { 'bundle': [bundle_] }
            if sub_bundles:
                bundles['sub-bundle'] = sub_bundles
        bng_workload_specs[bng_workload_key]['iface:interface'].update(bundles)

        if base_docs_subscriber_management:
            bng_workload_specs[bng_workload_key]['base'] = base_docs_subscriber_management

        bng_workload_specs[bng_workload_key]['iface:interface']['rip'] = rip
        bng_workload_specs[bng_workload_key]['iface:interface']['proto-throttle'] = {}
        if len(system_proto_throttle) != 0 :
            bng_workload_specs[bng_workload_key]['iface:interface']['proto-throttle']['system'] = system_proto_throttle
        if len(cable_proto_throttle) != 0 :
            bng_workload_specs[bng_workload_key]['iface:interface']['proto-throttle']['cable']= cable_proto_throttle
        bng_workload_specs[bng_workload_key]['iface:interface']['mcast'] = mcast
        bng_workload_specs[bng_workload_key]['iface:interface']['lawful-intercept'] = lawful_intercept
        bng_workload_specs[bng_workload_key]['iface:interface']['ipsec'] = ipsec
        if len(multicast_profiles) > 0:
            bng_workload_specs[bng_workload_key]['iface:interface']['multicast-profile'] = multicast_profiles

    service_class = deepcopy(get(iface, 'cosm-cli-iface-config:interface/service-class'))
    service_descriptor = deepcopy(get(iface, 'cosm-cli-iface-config:interface/service-descriptor'))

    svc_prov_app_specs = {}
    if spa_present:
        if load_balance_enabled:
            svc_prov_app_specs, olts_to_assign_spa = create_lb_spa_specs(olt_distribution_data, spa_templ[0], olts_to_assign_spa, module_types)
        else:
            svc_prov_app_specs, olts_to_assign_spa = create_single_spa_spec(olt_distribution_data, spa_templ[0], olts_to_assign_spa)

    # Apply config changes from template and delete_olts to get pod weight for new olts assignment
    for spa_key in svc_prov_app_specs:
        if bool(service_class):
            svc_prov_app_specs[spa_key]['iface:interface']['service-class'] = service_class
        if bool(service_descriptor):
            svc_prov_app_specs[spa_key]['iface:interface']['service-descriptor'] = service_descriptor

        bundle = {}
        if 'cable' in svc_prov_app_specs[spa_key]['iface:interface']['service-provisioning']:
            bundle_id = svc_prov_app_specs[spa_key]['iface:interface']['service-provisioning']['cable']['bundle']
            bundles = get(cable, 'cosm-cli-config:cable/bundle')

            try:
                bundle_ = next((b for b in bundles if b['bundle-index'] == bundle_id))
            except StopIteration: # just in case. CLI prevents this
                raise RuntimeError(f"service-provisioning's cable bundle ({bundle_id}) references unknown cable bundle")

            ip_pool_id = bundle_['cre-vlan-id']
            if ip_pool_id not in bundle_nsi_pools:
                bundle_nsi_pools[ip_pool_id] = create_nsi_ip_pool(ip_pool_id, bundle_id, None, bundle_.get('nsi'), docsis_ip_pool_size)
            spa = svc_prov_app_specs[spa_key]['iface:interface']['service-provisioning']
            spa_v4 = 'spa_{}_v4'.format(spa['service-provisioning-id'])
            spa_v6 = 'spa_{}_v6'.format(spa['service-provisioning-id'])
            bundle_nsi_pool = bundle_nsi_pools[ip_pool_id]
            store_ip(spa.get('nsi-ip-address'), bundle_nsi_pool, 'v4_pool', spa_v4, True)
            store_ip(spa.get('nsi-ipv6-address'), bundle_nsi_pool, 'v6_pool', spa_v6, True)

            bundle = {
                'bundle': [bundle_]
            }

        svc_prov_app_specs[spa_key]['iface:interface'].update(bundle)

    if 'onu-firmware-update' in fttx:
        for spa_spec in svc_prov_app_specs.values():
            spa_spec['iface:interface']['service-provisioning']['onu-firmware-update'] = fttx['onu-firmware-update']

    allocate_nsi_ips(bng_workload_specs, svc_prov_app_specs, bundle_nsi_pools, k8s_software_controls)

    olt_control_global_spec = deepcopy(get(iface, 'cosm-cli-iface-config:interface/olt-control'))
    bng_data_global_spec = deepcopy(get(iface, 'cosm-cli-iface-config:interface/bng-data'))

    distribute_olts_between_spa(olt_control_specs, svc_prov_app_specs, olts_to_assign_spa)

    return olt_control_specs, bng_workload_specs, svc_prov_app_specs, module_mgmt_specs, olt_control_global_spec, bng_data_global_spec


ModuleStagPoolMeta = collections.namedtuple('ModuleStagPoolMeta', ['module_sizes', 'module_reserved'])
ModuleVniPoolMeta = collections.namedtuple('ModuleVniPoolMeta', ['module_sizes', 'module_reserved'])
TunnelMgmtVxlanSrcIpPoolMeta = collections.namedtuple('TunnelMgmtVxlanSrcIpPoolMeta', ['reserved', 'limit_size'])
TunnelDataVxlanSrcIpPoolMeta = collections.namedtuple('TunnelDataVxlanSrcIpPoolMeta', ['reserved', 'limit_size'])

def gather_pool_meta(olt_control_specs, module_mgmt_specs):
    managable_olt_control_specs = get_managable_olt_control_specs(olt_control_specs, module_mgmt_specs)
    def get_module_spec(olt_control_spec):
        return olt_control_spec['iface:interface']['module'][0]

    module_stag_pool_sizes = {get_module_key(get_module_spec(olt_control_spec)):100 for olt_control_spec in managable_olt_control_specs}
    module_vni_pool_sizes = {get_module_key(get_module_spec(olt_control_spec)):100 for olt_control_spec in managable_olt_control_specs}
    module_reserved_stag = {get_module_key(get_module_spec(olt_control_spec)):[] for olt_control_spec in managable_olt_control_specs}
    module_reserved_vni = {get_module_key(get_module_spec(olt_control_spec)):[] for olt_control_spec in managable_olt_control_specs}

    tunnel_mgmt_vxlan_src_ip_reserved = []
    tunnel_data_vxlan_src_ip_reserved = []

    for olt_control_name, olt_control_spec in olt_control_specs.items():
        is_olt_managable = is_olt_spec_managable(olt_control_spec, module_mgmt_specs)
        olt_spec = olt_control_spec['iface:interface']['olt'][0]
        module_spec = olt_control_spec['iface:interface']['module'][0]
        module_key = (module_spec['v-chassis'], module_spec['v-slot'])
        # If tunnel mgmt/data configuration for MANAGABLE olt is defined:
        # Add vni/stag to the reserved. To not overlap static and dynamic (from pool) vnis/stags

        # Add to the reserved tunnel source ip, src ips from all tunnels (also from non managable olts)
        if not is_vxlan_tunnel_configured(olt_spec['tunnel-mgmt']):
            if not is_olt_managable:
                raise ValueError(f"tunnel-mgmt not configured for not managable olt spec {olt_control_name}")
        else:
            if is_olt_managable:
                module_reserved_vni[module_key].append(olt_spec['tunnel-mgmt']['vxlan']['vni'])
            tunnel_mgmt_vxlan_src_ip_reserved.append(olt_spec['tunnel-mgmt']['vxlan']['src-ip'])
        for pon_spec in olt_spec['pon']:
            if not is_vxlan_tunnel_configured(pon_spec['tunnel-data']):
                if not is_olt_managable:
                    raise ValueError(f"tunnel-data not configured for not managable olt spec {olt_control_name}")
            else:
                if is_olt_managable:
                    module_reserved_vni[module_key].append(pon_spec['tunnel-data']['vxlan']['vni'])
                tunnel_data_vxlan_src_ip_reserved.append(pon_spec['tunnel-data']['vxlan']['src-ip'])
            if 's-tag' not in pon_spec:
                if not is_olt_managable:
                    raise ValueError(f"stag not configured for not managable olt spec {olt_control_name}")
            else:
                if is_olt_managable:
                    module_reserved_stag[module_key].append(pon_spec['s-tag'])

    for module_mgmt_spec in module_mgmt_specs.values():
        if 'module-management' in module_mgmt_spec['iface:interface']:
            if 'network' in module_mgmt_spec['iface:interface']['module-management']:
                mm_network = module_mgmt_spec['iface:interface']['module-management']['network']
                if 'core-ip-address' in mm_network:
                    tunnel_mgmt_vxlan_src_ip_reserved.append(str(mm_network['core-ip-address']) + '/32')
                if 'core-ipv6-address' in mm_network:
                    tunnel_mgmt_vxlan_src_ip_reserved.append(str(mm_network['core-ipv6-address']) + '/128')

    module_stag_meta = ModuleStagPoolMeta(module_stag_pool_sizes, module_reserved_stag)
    module_vni_meta = ModuleVniPoolMeta(module_vni_pool_sizes, module_reserved_vni)
    tunnel_mgmt_vxlan_meta = TunnelMgmtVxlanSrcIpPoolMeta(tunnel_mgmt_vxlan_src_ip_reserved, 2048)
    tunnel_data_vxlan_meta = TunnelDataVxlanSrcIpPoolMeta(tunnel_data_vxlan_src_ip_reserved, 2048)
    return module_stag_meta, module_vni_meta, tunnel_mgmt_vxlan_meta, tunnel_data_vxlan_meta

def init_module_pools(module_stag_pool_meta: ModuleStagPoolMeta, module_vni_pool_meta: ModuleVniPoolMeta):
    module_stag_pool_sizes, module_reserved_stag = module_stag_pool_meta
    module_vni_pool_sizes, module_reserved_vni = module_vni_pool_meta
    # Does we need to add olt-control/bng-data dsi-vlans to the reserved vni pools??
    # Init pools
    # Pool based vni/stag configuration rules
    # 1. all tunnels belonging to the same module and BNG must have unique VNIs
    #   To simplify vNI pool logic vni should be unique per module
    # 2. all s-tags should be unique within a module
    # :type pool_type: dict[(v-chassis, v-slot), dict[int val, bool is_used]]
    module_stag_pool_from = 100
    module_vni_pool_from = 11000
    module_stag_pools = {module: {
        stag: False for stag in create_integer_pool(module_stag_pool_from, size, module_reserved_stag[module])} for module, size in module_stag_pool_sizes.items()
    }
    module_vni_pools = {module: {
        vni: False for vni in create_integer_pool(module_vni_pool_from, size, module_reserved_vni[module])} for module, size in module_vni_pool_sizes.items()
    }
    return module_stag_pools, module_vni_pools

def init_global_pools(tunnel_mgmt_vxlan_src_ip_pool_meta: TunnelMgmtVxlanSrcIpPoolMeta,
                      tunnel_data_vxlan_src_ip_pool_meta: TunnelDataVxlanSrcIpPoolMeta,
                      olt_control_global_spec,
                      bng_data_global_spec):
    tunnel_mgmt_vxlan_src_ip_pool = {}
    tunnel_data_vxlan_src_ip_pool = {}

    def init_tunnel_vxlan_src_ip_pool(tunnel_global_spec, tunnel_src_ip_pool_meta):
        if is_global_vxlan_tunnel_v4(tunnel_global_spec):
            return create_ip_pool(
                tunnel_global_spec['ip-address-pool']['start-ipv4-addr'],
                tunnel_global_spec['cre-gw-ip-address'],
                tunnel_src_ip_pool_meta.reserved,
                None, # Not limit size for ipv4
            )
        if is_global_vxlan_tunnel_v6(tunnel_global_spec):
            return create_ip_pool(
                tunnel_global_spec['ip-address-pool']['start-ipv6-addr'],
                tunnel_global_spec['cre-gw-ipv6-address'],
                tunnel_src_ip_pool_meta.reserved,
                tunnel_src_ip_pool_meta.limit_size,
            )
        raise ValueError('Either v4 or v6 global mgmt tunnel vxlan should be configured for pool-based ip address assignment')

    if olt_control_global_spec and 'tunnel-mgmt' in olt_control_global_spec:
        tunnel_mgmt_vxlan_src_ip_pool = {ip: False for ip in init_tunnel_vxlan_src_ip_pool(olt_control_global_spec['tunnel-mgmt'], tunnel_mgmt_vxlan_src_ip_pool_meta)}
    if bng_data_global_spec and 'tunnel-data' in bng_data_global_spec:
        tunnel_data_vxlan_src_ip_pool = {ip: False for ip in init_tunnel_vxlan_src_ip_pool(bng_data_global_spec['tunnel-data'], tunnel_data_vxlan_src_ip_pool_meta)}

    return tunnel_mgmt_vxlan_src_ip_pool, tunnel_data_vxlan_src_ip_pool

def get_managable_olt_control_specs(olt_control_specs, module_mgmt_specs):
    return list(filter(
        lambda olt_control_spec: is_olt_spec_managable(olt_control_spec, module_mgmt_specs),
        olt_control_specs.values()
        ))

def is_vxlan_tunnel_configured(tunnel_cfg):
    if tunnel_cfg is None or 'vxlan' not in tunnel_cfg:
        return False
    vxlan_tunnel = tunnel_cfg['vxlan']
    # If tunnel does not configured, only encapsulate-inner-vlan default value will be set.
    return 'src-ip' in vxlan_tunnel \
       and 'gateway' in vxlan_tunnel \
       and 'peer-ip' in vxlan_tunnel \
       and 'vni' in vxlan_tunnel \
       and 'dsi-vlan' in vxlan_tunnel

def pon_spec_from_olt(olt_spec, pon_spec):
    """ Returns pon spec from OLT spec

    :return: pon dict[str,dict] pon specification or None
    """
    for pon in olt_spec['pon']:
        if 'sfp-port-num' in pon_spec and 'pon-index' in pon_spec and \
            'sfp-port-num' in pon and 'pon-index' in pon:
            if pon['sfp-port-num'] == pon_spec['sfp-port-num'] and pon['pon-index'] == pon_spec['pon-index']:
                return pon
        elif pon['olt-pon-port'] == pon_spec['olt-pon-port']:
            return pon
    return None

def is_olt_spec_managable(olt_control_spec, module_mgmt_specs):
    """ Check if olt related module are managable. (Exists in module-mgmt spec).
    """
    olt_control_iface_spec = olt_control_spec['iface:interface']
    if len(olt_control_iface_spec['module']) < 1 or len(olt_control_iface_spec['olt']) < 1:
        return False
    # Now suppots only one module/olt per spec
    olt_module = olt_control_iface_spec['module'][0]
    olt = olt_control_iface_spec['olt'][0]

    # For now supported only one module-mgmt
    for module_mgmt_spec in module_mgmt_specs.values():
        if olt_module is not None and olt is not None:
            match_module = False
            for module_mgmt_module in module_mgmt_spec['iface:interface']['module']:
                if olt_module['v-chassis'] == module_mgmt_module['v-chassis'] and \
                   olt_module['v-slot'] == module_mgmt_module['v-slot']:
                    match_module = True
                    break
            if not match_module:
                continue

            match_olt = False
            if 'olt' in module_mgmt_spec['iface:interface']:
                for module_mgmt_olt in module_mgmt_spec['iface:interface']['olt']:
                    if olt['v-chassis'] == module_mgmt_olt['v-chassis'] and \
                    olt['v-slot'] == module_mgmt_olt['v-slot'] and \
                    olt['v-slot-port'] == module_mgmt_olt['v-slot-port']:
                        match_olt = True
                        break
            if not match_olt:
                continue
            return True
        return False

def is_module_cfg_managable(module_cfg, iface_cfg):
    """ Check if module is managable by module-mgmt application.
        Module is managable if it's tunnels-config equeal sztp or gcp and
        exists at least one (for now only one) module-mgmt config.

    :type module_cfg: dict[str, dict] module config
    :type iface_cfg: dict[str, dict] iface config

    :return: Bool
    """
    module_mgmt_instances = get(iface_cfg, 'cosm-cli-iface-config:interface/module-management')
    if module_cfg.get('tunnel-config') in ['sztp', 'gcp'] \
            and module_mgmt_instances is not None and len(module_mgmt_instances) != 0:
        return True
    return False

def is_global_vxlan_tunnel_configured(global_vxlan_cfg):
    return False if global_vxlan_cfg is None or 'ip-address-pool' not in global_vxlan_cfg else True

def is_global_vxlan_tunnel_v4(global_vxlan_cfg):
    return is_global_vxlan_tunnel_configured(global_vxlan_cfg) and 'cre-gw-ip-address' in global_vxlan_cfg and 'start-ipv4-addr' in global_vxlan_cfg['ip-address-pool']

def is_global_vxlan_tunnel_v6(global_vxlan_cfg):
    return is_global_vxlan_tunnel_configured(global_vxlan_cfg) and 'cre-gw-ipv6-address' in global_vxlan_cfg and 'start-ipv6-addr' in global_vxlan_cfg['ip-address-pool']

def create_integer_pool(value_from, size, reserved=[]):
    """Create new integer pool in range [value_from..value_from + size] without reserved elements
    :type value_from: int
    :type size: int
    :type reserved: [int]

    :return: int_pool [int]
    """
    # Create a set of reserved elements for faster lookup
    reserved_set = set(reserved)
    # Initialize an empty integer pool
    int_pool = []
    # Generate integers starting from 'value_from' and add them to the pool until the desired size is reached
    i = value_from
    while len(int_pool) < size:
        if i not in reserved_set:
            int_pool.append(i)
        i += 1
    # Check if the pool size is less than the desired size
    if len(int_pool) < size:
        raise ValueError("Unable to create an integer pool of the desired size.")
    return int_pool

def create_ip_pool(start_ip, network, reserved=[], limit_size=None):
    ip_net = ipaddress.ip_network(network, False)
    start_ip_addr = ipaddress.ip_address(start_ip)
    reserved_ips = list(map(lambda ip_str: ipaddress.ip_address(ip_str.split('/')[0]), reserved))

    if start_ip_addr not in ip_net:
        raise ValueError(f'Start IP address {start_ip} not in the network {network}')

    # All the evaluation is "lazy". Ex. for ipv6 /64 subnet with limit 1000 it won't allocate 2^64 addresses and than
    # drop, it will just allocates 1000
    available_hosts = filter(
        lambda ip: ip not in reserved_ips,
        itertools.dropwhile(
            lambda ip: ip != start_ip_addr,
            ip_net.hosts()
        )
    )

    if limit_size is not None:
        available_hosts = itertools.islice(available_hosts, limit_size)

    return list(map(lambda host: f'{host.compressed}/{ip_net.prefixlen}', available_hosts))

def get_olt_module(olt, iface_cfg):
    """Get related to OlT module from configuration
    :type olt: dict[str, dict] olt config
    :type iface_cfg: dict[str, dict] iface config

    :return: module dict[str, dict] or None
    """
    module_cfg = {}
    for module in get(iface_cfg, 'cosm-cli-iface-config:interface/module'):
        if module['v-chassis'] == olt['v-chassis'] \
                and module['v-slot'] == olt['v-slot']:
            module_cfg = deepcopy(module)
            break
    return module_cfg

def get_module_key(olt_or_module_spec):
    return (olt_or_module_spec['v-chassis'], olt_or_module_spec['v-slot'])

def merge_config(target, *args):
    """Merge all config parts into one
    This is done because reading whole cable config is highly inefficient, especially on scale systems

    :type target: dict[str, dict] config
    :type *args: list[dict[str, str]] list of parts to be merged into target

    :return: target dict[str, dict]
    """

    for arg in args:
        target = merge(target, arg)

    return target

def apply_vswitch_cfg(olt_control_specs, failed_olt_specs, bng_workload_specs, svc_prov_app_specs):
    apply_vswitch_mac_addrs(bng_workload_specs, svc_prov_app_specs)
    apply_vswitch_ctrl_mac_addrs(svc_prov_app_specs)
    apply_vswitch_gw_addrs(olt_control_specs, failed_olt_specs)

def apply_vswitch_mac_addrs(bng_workload_specs, svc_prov_app_specs):
    mac_addrs = list()
    for key, bng in bng_workload_specs.items():
        bngs = get(bng, 'iface:interface/bng', None)
        nsi_ip = get(bngs[0], 'nsi-ip-address', None)
        nsi_ipv6 = get(bngs[0], 'nsi-ipv6-address', None)
        if nsi_ip:
            mac_addrs.append(ipv4tomdma(ipaddress.IPv4Address(nsi_ip)))
        elif nsi_ipv6:
            mac_addrs.append(ipv6tomdma(ipaddress.IPv6Address(nsi_ipv6)))
    for key, spa in svc_prov_app_specs.items():
        nsi_ip = get(spa, 'iface:interface/service-provisioning/nsi-ip-address', None)
        nsi_ipv6 = get(spa, 'iface:interface/service-provisioning/nsi-ipv6-address', None)
        if nsi_ip:
            mac_addrs.append(ipv4tomdma(ipaddress.IPv4Address(nsi_ip)))
        elif nsi_ipv6:
            mac_addrs.append(ipv6tomdma(ipaddress.IPv6Address(nsi_ipv6)))
    if mac_addrs:
        k8s_update_vswitch_mac_addrs(mac_addrs)

def apply_vswitch_ctrl_mac_addrs( svc_prov_app_specs):
    mac_addrs = list()
    for key, spa in svc_prov_app_specs.items():
        nsi_ip = get(spa, 'iface:interface/service-provisioning/nsi-ip-address', None)
        nsi_ipv6 = get(spa, 'iface:interface/service-provisioning/nsi-ipv6-address', None)
        if nsi_ip:
            mac_addrs.append(ipv4tomdma(ipaddress.IPv4Address(nsi_ip)))
        elif nsi_ipv6:
            mac_addrs.append(ipv6tomdma(ipaddress.IPv6Address(nsi_ipv6)))
    if mac_addrs:
        k8s_update_vswitch_ctrl_mac_addrs(mac_addrs)

def apply_vswitch_gw_addrs(olt_control_specs, failed_olt_specs):
    # NSI GWs are beeing handles by cable, adding mgmttunnel gw here
    vswitch_gws = list()
    for key, olt in olt_control_specs.items():
        if key in failed_olt_specs:
            continue
        olt = get(olt, 'iface:interface/olt')[0]
        ipv4 = str(get(olt, 'tunnel-mgmt/vxlan/gateway', '0.0.0.0'))
        netmask = str(get(olt, 'tunnel-mgmt/vxlan/src-ip', '0.0.0.0/8')).split('/')[1]
        vswitch_gws.append({
            'vlan': str(get(olt, 'tunnel-mgmt/vxlan/dsi-vlan/vid', 0)),
            'gateway': (ipv4 + '/' + netmask)
        })
    #remove duplicates, same gw can be used for many OLTs
    vswitch_gws = list({v['gateway']:v for v in vswitch_gws}.values())
    if vswitch_gws:
        k8s_update_vswitch_gw_addrs(vswitch_gws)

def main():
    loadLoggerConfig('/usr/share/cosm-os-gateway/etc/logger.conf')
    logger.info('interface_cfg.py: lights on')

    logger.info('Starting...')
    in_json = None
    raw_json = sys.stdin.read()
    try:
        in_json = json.loads(raw_json)
        with open('/tmp/output', 'a') as f:
            f.write(json.dumps(in_json, indent=4))
            f.write("\n")
    except json.JSONDecodeError as e:
        logger.info('DAHER 1')
        logger.info('Failed to parse input JSON, error: {}'.format(e))
        logger.info('Failed to parse input JSON, error: {}'.format(raw_json))
        return
    logger.info('DAHER 2')
    iface, cable_bundle, cable_subbundle, base_docs_subscriber_management, \
        rip, system_proto_throttle, cable_proto_throttle, mcast, lawful_intercept, ipsec, docsis_ip_pool_size = in_json

    logger.info('DAHER 3')
    for base in base_docs_subscriber_management:
        base['type'] = "remotephies"
    logger.info('stdin parsed')

    logger.info('DAHER 4')
    iface = iface['data']
    cable_bundle = cable_bundle['data']
    cable_subbundle = cable_subbundle['data']
    rip = rip['data']
    system_proto_throttle = system_proto_throttle['data']
    cable_proto_throttle = cable_proto_throttle['data']
    mcast = mcast['data']
    lawful_intercept = lawful_intercept['data']
    ipsec = ipsec['data']
    ns_filter(iface, ['cosm-cli-iface-config'])
    ns_filter(cable_bundle, ['cosm-cli-config'])
    ns_filter(cable_subbundle, ['cosm-cli-config'])

    logger.info('DAHER 5')
    cable = merge_config(cable_bundle,
                         cable_subbundle,
                         rip,
                         system_proto_throttle,
                         cable_proto_throttle,
                         mcast,
                         lawful_intercept,
                         ipsec)

    k8s_software_controls = k8s_get_software_controls()

    olt_control_specs, bng_workload_specs, svc_prov_app_specs, module_mgmt_specs, olt_control_global_spec, bng_data_global_spec = split(iface, cable, docsis_ip_pool_size, base_docs_subscriber_management, k8s_software_controls)

    failed_olt_specs = apply_olt_control(olt_control_specs, k8s_software_controls, module_mgmt_specs, olt_control_global_spec, bng_data_global_spec)
    apply_bng_workload(bng_workload_specs)
    apply_svc_prov_app(svc_prov_app_specs, k8s_software_controls)
    apply_module_mgmt(module_mgmt_specs, olt_control_specs, failed_olt_specs, k8s_software_controls)

    if APOLLO_XI:
        apply_vswitch_cfg(olt_control_specs, failed_olt_specs, bng_workload_specs, svc_prov_app_specs)

    logger.info('Finished')
    logger.info('interface_cfg.py: lights off')



if __name__ == '__main__':
    main()
