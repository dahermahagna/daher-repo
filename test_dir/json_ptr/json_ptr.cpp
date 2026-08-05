#include <iostream>
#include <array>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <nlohmann/json.hpp>

namespace {

uint32_t parseAdminState(const nlohmann::json& value)
{
    if (value.is_number_unsigned()) {
        return value.get<uint32_t>();
    }
    if (value.is_number_integer()) {
        const auto v = value.get<int64_t>();
        return v < 0 ? 0U : static_cast<uint32_t>(v);
    }
    if (value.is_string()) {
        std::string text = value.get<std::string>();
        for (char& c : text) {
            if (c >= 'A' && c <= 'Z') {
                c = static_cast<char>(c + ('a' - 'A'));
            }
        }

        if (text == "up" || text == "enabled" || text == "true") {
            return 1U;
        }
        if (text == "down" || text == "disabled" || text == "false") {
            return 0U;
        }

        try {
            return static_cast<uint32_t>(std::stoul(text));
        } catch (...) {
            std::cout << "Warning: unsupported admin-state value: " << text << std::endl;
            return 0U;
        }
    }

    std::cout << "Warning: unsupported admin-state type" << std::endl;
    return 0U;
}

bool parseBoolValue(const nlohmann::json& value)
{
    if (value.is_boolean()) {
        return value.get<bool>();
    }
    if (value.is_number_integer() || value.is_number_unsigned()) {
        return value.get<int64_t>() != 0;
    }
    if (value.is_string()) {
        std::string text = value.get<std::string>();
        for (char& c : text) {
            if (c >= 'A' && c <= 'Z') {
                c = static_cast<char>(c + ('a' - 'A'));
            }
        }

        if (text == "true" || text == "1" || text == "yes" || text == "enabled" || text == "up") {
            return true;
        }
        if (text == "false" || text == "0" || text == "no" || text == "disabled" || text == "down") {
            return false;
        }
    }

    std::cout << "Warning: unsupported bool value/type" << std::endl;
    return false;
}

} // namespace

class json_ptr
{
private:
    /* data */
public:
    json_ptr(/* args */){};
    ~json_ptr(){};
    void print_json_ptr(){std::cout << "json_ptr "<< std::endl;};

};

char * json_file="{
    "data": {
        "ccap:ccap": {
            "management": {
                "fault-management": {
                    "diag-log-global-config": {
                        "aging": 10080,
                        "max-size": 100,
                        "notif-ctrl": "",
                        "notify-log-size-high-thrshld": 80,
                        "notify-log-size-low-thrshld": 60
                    },
                    "diag-log-triggers-config": {
                        "enable-aging-triggers": "",
                        "include-triggers": "registration",
                        "ranging-retry-station-maint-num": 90,
                        "ranging-retry-threshold": 6,
                        "ranging-retry-trigger": "consecutive-miss",
                        "reg-detail": "",
                        "reg-time-interval": 90
                    },
                    "event-reporting-config": [
                        {
                            "priority": "emergency",
                            "reporting": "local"
                        },
                        {
                            "priority": "alert",
                            "reporting": "local"
                        },
                        {
                            "priority": "critical",
                            "reporting": "local traps syslog"
                        },
                        {
                            "priority": "error",
                            "reporting": "local traps syslog"
                        },
                        {
                            "priority": "warning",
                            "reporting": "local traps syslog"
                        },
                        {
                            "priority": "notice",
                            "reporting": "local traps syslog"
                        },
                        {
                            "priority": "information",
                            "reporting": ""
                        },
                        {
                            "priority": "debug",
                            "reporting": ""
                        }
                    ],
                    "event-throttle-config": {
                        "interval": 1,
                        "threshold": 0,
                        "throttle-admin-state": "unconstrained"
                    },
                    "yang-ext": {
                        "ccap-harmonic:no-reboot-on-fault": false
                    }
                },
                "ipdr": {
                    "exporter-config": {
                        "enabled": false,
                        "yang-ext": {
                            "ccap-harmonic:ack-timeout": 60,
                            "ccap-harmonic:connection": "accept",
                            "ccap-harmonic:keep-alive": 300,
                            "ccap-harmonic:max-unacked": 200
                        }
                    }
                },
                "snmp": {
                    "access-config": [
                        {
                            "community": "ILKS07",
                            "ip-address": "0.0.0.0/0",
                            "type": "read-only"
                        },
                        {
                            "community": "public",
                            "ip-address": "0.0.0.0/0",
                            "type": "read-write"
                        }
                    ],
                    "ccap-harmonic-management:fast-counters": {
                        "enabled": false
                    },
                    "notification-receiver-config": [
                        {
                            "notification-receiver": {
                                "address": "10.40.29.117"
                            },
                            "notification-receiver-name": "10.40.29.117",
                            "notification-receiver-port": 162,
                            "retries": 3,
                            "timeout": 1,
                            "type": "snmpv2c-trap",
                            "view-config-ref": [
                                {
                                    "view-name": "trap_cable_cm-onoff"
                                },
                                {
                                    "view-name": "trap_user-mgmt_user-change"
                                }
                            ],
                            "yang-ext": {
                                "ccap-harmonic-management:auth": "priv",
                                "ccap-harmonic-management:community-string": "public",
                                "ccap-harmonic-management:trap-type": "snmpv3-trap"
                            }
                        },
                        {
                            "notification-receiver": {
                                "address": "10.71.14.22"
                            },
                            "notification-receiver-name": "10.71.14.22",
                            "notification-receiver-port": 162,
                            "retries": 3,
                            "timeout": 1,
                            "type": "snmpv2c-trap",
                            "view-config-ref": [
                                {
                                    "view-name": "trap_aaa-server_status"
                                },
                                {
                                    "view-name": "trap_cable_cm-onoff"
                                },
                                {
                                    "view-name": "trap_config_status"
                                },
                                {
                                    "view-name": "trap_config_system-clock-changed"
                                },
                                {
                                    "view-name": "trap_core_gm-sync-acquired"
                                },
                                {
                                    "view-name": "trap_core_gm-sync-lost"
                                },
                                {
                                    "view-name": "trap_core_ipsec-event"
                                },
                                {
                                    "view-name": "trap_core_md-map-advance-changed"
                                },
                                {
                                    "view-name": "trap_core_md-protection-changed"
                                },
                                {
                                    "view-name": "trap_core_node-joined"
                                },
                                {
                                    "view-name": "trap_core_node-left"
                                },
                                {
                                    "view-name": "trap_core_scep-event"
                                },
                                {
                                    "view-name": "trap_core_status"
                                },
                                {
                                    "view-name": "trap_docsis-events_bpkm"
                                },
                                {
                                    "view-name": "trap_docsis-events_dccack"
                                },
                                {
                                    "view-name": "trap_docsis-events_dccreq"
                                },
                                {
                                    "view-name": "trap_docsis-events_dccrsp"
                                },
                                {
                                    "view-name": "trap_docsis-events_regack"
                                },
                                {
                                    "view-name": "trap_docsis-events_regreq"
                                },
                                {
                                    "view-name": "trap_envmon_fan"
                                },
                                {
                                    "view-name": "trap_envmon_memory"
                                },
                                {
                                    "view-name": "trap_envmon_memory-corrected"
                                },
                                {
                                    "view-name": "trap_envmon_power"
                                },
                                {
                                    "view-name": "trap_envmon_security"
                                },
                                {
                                    "view-name": "trap_envmon_temperature"
                                },
                                {
                                    "view-name": "trap_envmon_voltage"
                                },
                                {
                                    "view-name": "trap_rpd-events_connectivity"
                                },
                                {
                                    "view-name": "trap_rpd-events_phy-redundancy"
                                },
                                {
                                    "view-name": "trap_rpd-events_ptp"
                                },
                                {
                                    "view-name": "trap_rpd_config"
                                },
                                {
                                    "view-name": "trap_rpd_crash-file-status"
                                },
                                {
                                    "view-name": "trap_rpd_no-traffic-on-channel"
                                },
                                {
                                    "view-name": "trap_rpd_ptp-sync-timeout"
                                },
                                {
                                    "view-name": "trap_rpd_resources"
                                },
                                {
                                    "view-name": "trap_rpd_rf-state"
                                },
                                {
                                    "view-name": "trap_rpd_ssh"
                                },
                                {
                                    "view-name": "trap_rpd_status"
                                },
                                {
                                    "view-name": "trap_rpn_power-alarm"
                                },
                                {
                                    "view-name": "trap_snmp_coldstart"
                                },
                                {
                                    "view-name": "trap_snmp_link-updown"
                                },
                                {
                                    "view-name": "trap_user-mgmt_user-change"
                                }
                            ],
                            "yang-ext": {
                                "ccap-harmonic-management:auth": "no-auth",
                                "ccap-harmonic-management:community-string": "ILKS07",
                                "ccap-harmonic-management:trap-type": "snmpv2c-trap"
                            }
                        }
                    ],
                    "view-config": [
                        {
                            "subtree": "1.3.6.1.4.1.1563.11.1.2.0",
                            "subtree-mask": "1.1.1.1.1.1.1.1.1.1.1",
                            "type": "included",
                            "view-name": "trap_aaa-server_status"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_cable_cm-onoff"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_config_status"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_config_system-clock-changed"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_core_gm-sync-acquired"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_core_gm-sync-lost"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_core_ipsec-event"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_core_md-map-advance-changed"
                        },
                        {
                            "subtree": "1.3.6.1.4.1.1563.10.1.4.1.4.0.25",
                            "subtree-mask": "1.1.1.1.1.1.1.1.1.1.1.1.1.1",
                            "type": "included",
                            "view-name": "trap_core_md-protection-changed"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_core_node-joined"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_core_node-left"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_core_scep-event"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_core_status"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_docsis-events_bpkm"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_docsis-events_dccack"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_docsis-events_dccreq"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_docsis-events_dccrsp"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_docsis-events_regack"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_docsis-events_regreq"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_envmon_fan"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_envmon_memory"
                        },
                        {
                            "subtree": "1.3.6.1.4.1.1563.10.1.4.1.4.0.23",
                            "subtree-mask": "1.1.1.1.1.1.1.1.1.1.1.1.1.1",
                            "type": "included",
                            "view-name": "trap_envmon_memory-corrected"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_envmon_power"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_envmon_security"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_envmon_temperature"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_envmon_voltage"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd-events_connectivity"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "excluded",
                            "view-name": "trap_rpd-events_phy-redundancy"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd-events_ptp"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd_config"
                        },
                        {
                            "subtree": "1.3.6.1.4.1.1563.10.1.4.1.4.0.26",
                            "subtree-mask": "1.1.1.1.1.1.1.1.1.1.1.1.1.1",
                            "type": "excluded",
                            "view-name": "trap_rpd_crash-file-status"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd_no-traffic-on-channel"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd_ptp-sync-timeout"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd_resources"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd_rf-state"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd_ssh"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpd_status"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "included",
                            "view-name": "trap_rpn_power-alarm"
                        },
                        {
                            "subtree": "1.3.6.1.6.3.1.1.5.1",
                            "subtree-mask": "1.1.1.1.1.1.1.1.1.1",
                            "type": "included",
                            "view-name": "trap_snmp_coldstart"
                        },
                        {
                            "subtree": "N/A",
                            "subtree-mask": "N/A",
                            "type": "excluded",
                            "view-name": "trap_snmp_link-updown"
                        },
                        {
                            "subtree": "1.3.6.1.4.1.1563.10.1.2.0",
                            "subtree-mask": "1.1.1.1.1.1.1.1.1.1.1",
                            "type": "included",
                            "view-name": "trap_user-mgmt_user-change"
                        }
                    ],
                    "yang-ext": {
                        "ccap-harmonic-management:enabler": "enabled",
                        "ccap-harmonic-management:engine": {
                            "engine-id": "8000061b80000000000000"
                        },
                        "ccap-harmonic-management:group": [
                            {
                                "name": "private",
                                "read-view-name": "all",
                                "security-level": "priv",
                                "version": "v3",
                                "write-view-name": "all"
                            },
                            {
                                "name": "ro-auth",
                                "read-view-name": "all",
                                "security-level": "auth",
                                "version": "v3"
                            },
                            {
                                "name": "ro-no-auth",
                                "read-view-name": "all",
                                "security-level": "no-auth",
                                "version": "v3"
                            },
                            {
                                "name": "ro-priv",
                                "read-view-name": "all",
                                "security-level": "priv",
                                "version": "v3"
                            },
                            {
                                "name": "rw-auth",
                                "read-view-name": "all",
                                "security-level": "auth",
                                "version": "v3",
                                "write-view-name": "all"
                            },
                            {
                                "name": "rw-no-auth",
                                "read-view-name": "all",
                                "security-level": "no-auth",
                                "version": "v3",
                                "write-view-name": "all"
                            },
                            {
                                "name": "rw-priv",
                                "read-view-name": "all",
                                "security-level": "priv",
                                "version": "v3",
                                "write-view-name": "all"
                            }
                        ],
                        "ccap-harmonic-management:informs": {
                            "pending": 25,
                            "retries": 3,
                            "timeout": 15
                        },
                        "ccap-harmonic-management:user": [
                            {
                                "auth-protocol": "md5",
                                "group-name": "private",
                                "name": "user-without",
                                "password": "7c4c5c4eb439b440722e5c36a25400ac",
                                "udp-port": 162,
                                "version": "v3"
                            },
                            {
                                "auth-protocol": "md5",
                                "group-name": "private",
                                "name": "userAES",
                                "password": "7c4c5c4eb439b440722e5c36a25400ac",
                                "privacy-protocol": "aes",
                                "udp-port": 162,
                                "version": "v3"
                            },
                            {
                                "auth-protocol": "md5",
                                "group-name": "private",
                                "name": "userDES",
                                "password": "7c4c5c4eb439b440722e5c36a25400ac",
                                "privacy-protocol": "des",
                                "udp-port": 162,
                                "version": "v3"
                            }
                        ],
                        "ccap-harmonic-management:view": [
                            {
                                "name": "all",
                                "subtree": [
                                    {
                                        "oid": "1",
                                        "type": "included"
                                    }
                                ]
                            },
                            {
                                "name": "none",
                                "subtree": [
                                    {
                                        "oid": "1",
                                        "type": "excluded"
                                    }
                                ]
                            }
                        ]
                    }
                },
                "yang-ext": {
                    "ccap-harmonic-management:api-bridge": {
                        "is-enabled": false
                    },
                    "ccap-harmonic-management:auto-copy-to-startup-interval-min": 0,
                    "ccap-harmonic-management:cli-devel-log": {
                        "is-enabled": true,
                        "severity": "info"
                    },
                    "ccap-harmonic-management:cluster": {
                        "protection-group": {
                            "lcce-count": 2,
                            "redundancy-policy": 1
                        },
                        "server-count": 1
                    },
                    "ccap-harmonic-management:is-sshd-unsecure-mode": false,
                    "ccap-harmonic-management:is-startup-config": false,
                    "ccap-harmonic-management:logging": {
                        "buffered": {
                            "size-mb": 10
                        },
                        "evt": [
                            {
                                "description": "Cold Start",
                                "evt-enabled": true,
                                "evt-id": 1024,
                                "name": "cold-start",
                                "priority": "notice"
                            },
                            {
                                "description": "Link Up",
                                "evt-enabled": true,
                                "evt-id": 1026,
                                "name": "link-up",
                                "priority": "notice"
                            },
                            {
                                "description": "Link Down",
                                "evt-enabled": true,
                                "evt-id": 1027,
                                "name": "link-down",
                                "priority": "notice"
                            },
                            {
                                "description": "Cable Modem Online",
                                "evt-enabled": true,
                                "evt-id": 1028,
                                "name": "cm-online",
                                "priority": "notice"
                            },
                            {
                                "description": "Cable Modem Offline",
                                "evt-enabled": true,
                                "evt-id": 1029,
                                "name": "cm-offline",
                                "priority": "notice"
                            },
                            {
                                "description": "User Added",
                                "evt-enabled": true,
                                "evt-id": 1030,
                                "name": "user-add",
                                "priority": "notice"
                            },
                            {
                                "description": "User Deleted",
                                "evt-enabled": true,
                                "evt-id": 1031,
                                "name": "user-delete",
                                "priority": "notice"
                            },
                            {
                                "description": "AAA Service Error",
                                "evt-enabled": true,
                                "evt-id": 1032,
                                "name": "aaa-srv-error",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD Online",
                                "evt-enabled": true,
                                "evt-id": 1033,
                                "name": "rpd-online",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD Offline",
                                "evt-enabled": true,
                                "evt-id": 1034,
                                "name": "rpd-offline",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD Connect Timeout",
                                "evt-enabled": true,
                                "evt-id": 1035,
                                "name": "rpd-reconn-to",
                                "priority": "notice"
                            },
                            {
                                "description": "IP address conflict detected on nsi provisioning interface",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1043,
                                "name": "np-ip-conflict",
                                "priority": "notice"
                            },
                            {
                                "description": "nsi provisioning interface failed to obtain an IP address",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1044,
                                "name": "np-failure",
                                "priority": "notice"
                            },
                            {
                                "description": "event triggered on core state change",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1045,
                                "name": "core-state",
                                "priority": "notice"
                            },
                            {
                                "description": "Node left cluster",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1046,
                                "name": "node-left",
                                "priority": "error"
                            },
                            {
                                "description": "Node joined cluster",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1047,
                                "name": "node-join",
                                "priority": "error"
                            },
                            {
                                "description": "RPD configuration/capabilities mismatch",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1060,
                                "name": "rpd-validation-failure",
                                "priority": "warning"
                            },
                            {
                                "description": "RPD configuration/capabilities become valid",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1061,
                                "name": "rpd-config-valid",
                                "priority": "warning"
                            },
                            {
                                "description": "PTP failed to sync before timeout",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1062,
                                "name": "ptp-sync-timeout",
                                "priority": "warning"
                            },
                            {
                                "description": "GCP Connected",
                                "evt-enabled": true,
                                "evt-id": 1063,
                                "name": "gcp-connect",
                                "priority": "notice"
                            },
                            {
                                "description": "GCP disconnected",
                                "evt-enabled": true,
                                "evt-id": 1064,
                                "name": "gcp-disconnect",
                                "priority": "notice"
                            },
                            {
                                "description": "GM synchronized",
                                "evt-enabled": true,
                                "evt-id": 1065,
                                "name": "grandmaster-sync",
                                "priority": "notice"
                            },
                            {
                                "description": "GM lost synchronization",
                                "evt-enabled": true,
                                "evt-id": 1066,
                                "name": "grandmaster-sync-lost",
                                "priority": "notice"
                            },
                            {
                                "description": "System Clock changed",
                                "evt-enabled": true,
                                "evt-id": 1067,
                                "name": "system-clock-changed",
                                "priority": "notice"
                            },
                            {
                                "description": "DOCSIS Event occured",
                                "evt-enabled": true,
                                "evt-id": 1068,
                                "name": "docsis-evt",
                                "priority": "notice"
                            },
                            {
                                "description": "System MTU change",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1069,
                                "name": "system-mtu",
                                "priority": "error"
                            },
                            {
                                "description": "MD address OUI change",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1070,
                                "name": "md-addr-oui",
                                "priority": "error"
                            },
                            {
                                "description": "SCEP Event",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1071,
                                "name": "scep",
                                "priority": "notice"
                            },
                            {
                                "description": "IPSEC Event",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1072,
                                "name": "ipsec",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD video type mismatch event",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1073,
                                "name": "rpd-video-type-mismatch",
                                "priority": "notice"
                            },
                            {
                                "description": "MD map-advance change event",
                                "evt-enabled": true,
                                "evt-id": 1075,
                                "name": "md-map-advance-changed",
                                "priority": "notice"
                            },
                            {
                                "description": "LCCE MD protection change event",
                                "evt-enabled": true,
                                "evt-id": 1076,
                                "name": "md-protection-changed",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD shared configuration mismatch event",
                                "destination": [
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1077,
                                "name": "rpd-shared-configuration-mismatch",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD crash status file event",
                                "destination": [
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 1078,
                                "name": "rpd-crash-status-file",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD SSH session opened",
                                "evt-enabled": true,
                                "evt-id": 2002,
                                "name": "rpd-ssh-login",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD SSH session closed",
                                "evt-enabled": true,
                                "evt-id": 2003,
                                "name": "rpd-ssh-logout",
                                "priority": "notice"
                            },
                            {
                                "description": "Total RPD CPU utilization exceeded threshold",
                                "evt-enabled": true,
                                "evt-id": 2005,
                                "name": "rpd-total-cpu-utilization",
                                "priority": "notice"
                            },
                            {
                                "description": "Total RPD memory utilization exceeded threshold",
                                "evt-enabled": true,
                                "evt-id": 2006,
                                "name": "rpd-total-memory-utilization",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD configuration changed",
                                "evt-enabled": true,
                                "evt-id": 2020,
                                "name": "rpd-config-changed",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD DS RF port state changed",
                                "evt-enabled": true,
                                "evt-id": 2024,
                                "name": "rpd-ds-port-state",
                                "priority": "notice"
                            },
                            {
                                "description": "Config topology changed",
                                "evt-enabled": true,
                                "evt-id": 2025,
                                "name": "cli-topology-change",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD DS channel state changed",
                                "evt-enabled": true,
                                "evt-id": 2026,
                                "name": "rpd-ds-channel-state",
                                "priority": "notice"
                            },
                            {
                                "description": "RPD DS OFDM channel state changed",
                                "evt-enabled": true,
                                "evt-id": 2028,
                                "name": "rpd-ofdm-channel-state",
                                "priority": "notice"
                            },
                            {
                                "description": "No traffic on video channel",
                                "evt-enabled": true,
                                "evt-id": 2030,
                                "name": "no-traffic-on-channel",
                                "priority": "notice"
                            },
                            {
                                "description": "RPN Power Status Changed",
                                "evt-enabled": true,
                                "evt-id": 2034,
                                "name": "power-status-changed",
                                "priority": "notice"
                            },
                            {
                                "description": "MSTP configuration Changed",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2046,
                                "name": "mstp-configuration-changed",
                                "priority": "notice"
                            },
                            {
                                "description": "Sfp link UP",
                                "evt-enabled": true,
                                "evt-id": 2050,
                                "name": "sfp-link-up",
                                "priority": "notice"
                            },
                            {
                                "description": "Sfp link DOWN",
                                "evt-enabled": true,
                                "evt-id": 2051,
                                "name": "sfp-link-down",
                                "priority": "notice"
                            },
                            {
                                "description": "Rpd image activation failure",
                                "destination": [
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2052,
                                "name": "rpd-image-activation-failure",
                                "priority": "notice"
                            },
                            {
                                "description": "phy redundancy ACTIVE state",
                                "destination": [
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2053,
                                "name": "rpd-phy-redundancy-active",
                                "priority": "notice"
                            },
                            {
                                "description": "phy redundancy STANDBY state",
                                "destination": [
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2054,
                                "name": "rpd-phy-redundancy-standby",
                                "priority": "notice"
                            },
                            {
                                "description": "phy redundancy FAILED state",
                                "destination": [
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2055,
                                "name": "rpd-phy-redundancy-failed",
                                "priority": "notice"
                            },
                            {
                                "description": "Node Configuration Changed",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2056,
                                "name": "node-config-enabled-change",
                                "priority": "notice"
                            },
                            {
                                "description": "Node Configuration Disabled",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2057,
                                "name": "node-config-disabled",
                                "priority": "notice"
                            },
                            {
                                "description": "Missing RPD Calibration",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2061,
                                "name": "missing-rpd-calibration",
                                "priority": "error"
                            },
                            {
                                "description": "Missing RFT Calibration",
                                "destination": [
                                    {
                                        "dest-type": "local-log"
                                    },
                                    {
                                        "dest-type": "syslog"
                                    },
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2062,
                                "name": "missing-rft-calibration",
                                "priority": "error"
                            },
                            {
                                "description": "phy redundancy enabled",
                                "destination": [
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2064,
                                "name": "rpd-phy-redundancy-enabled",
                                "priority": "notice"
                            },
                            {
                                "description": "phy redundancy disabled",
                                "destination": [
                                    {
                                        "dest-type": "console"
                                    }
                                ],
                                "evt-enabled": true,
                                "evt-id": 2065,
                                "name": "rpd-phy-redundancy-disabled",
                                "priority": "notice"
                            }
                        ],
                        "fluent-bit": {
                            "filter": {
                                "throttle-plugin": {
                                    "rate-limit": {
                                        "interval": 600,
                                        "match": "*",
                                        "msg-rate": 5,
                                        "window": 5
                                    }
                                }
                            },
                            "general": {
                                "enabler": false
                            }
                        },
                        "syslog": {
                            "control": "tcp",
                            "port": 514
                        }
                    },
                    "ccap-harmonic-management:netconf": {
                        "get-default": "disabled",
                        "is-enabled": false
                    },
                    "ccap-harmonic-management:sonar-service": {
                        "is-enabled": false,
                        "sonar-password": "$6$V0ZAZBowdxq.3W.x$RM9FRAxfzpNZi56xToxkN9KhbXDO/iEkUldqXYJOIjaXQ8a92tacp2nKqNpd.rjjbqtTktekmhrOljsyEBhot."
                    },
                    "ccap-harmonic:access": {
                        "ssh": true
                    },
                    "ccap-harmonic:cosm-ip-address": "7.8.7.100",
                    "ccap-harmonic:cosm-ip-gw-address": "7.8.7.1/24",
                    "ccap-harmonic:in-band-ssh-passward-enabled": false,
                    "ccap-harmonic:redis": {
                        "redis-number": 10
                    },
                    "ccap-harmonic:telemetry": {
                        "prometheus-optimization": true,
                        "prometheus-scrape": false,
                        "prometheus-scrape-interval": "30s"
                    }
                }
            }
        },
        "txid": "1782:918958:361535"
    }
}"

bool dParse(const nlohmann::json& config/* , CEventCache& cache */)
{
    static const nlohmann::json::json_pointer basePtr(
        "/data/ccap:ccap/chassis/slot");

    if (!config.contains(basePtr)) {
        std::cout << "InterfaceConfigParser: path not found in JSON" << std::endl;
        
        return false;
    }
    else
        std::cout << "InterfaceConfigParser: path found in JSON" << std::endl;

    const auto& slots = config.at(basePtr);
    for (const auto& slot : slots) {
        // DS RF Ports
        if (slot.contains("ds-rf-port")) {
            for (const auto& dsPort : slot.at("ds-rf-port")) {
                uint32_t dsPortId = 0;
                if (dsPort.contains("ds-rf-port-id"))
                    dsPortId = dsPort.at("ds-rf-port-id").get<uint32_t>();

                uint16_t rpdId = 0;
                if (slot.contains("slot-id"))
                    rpdId = slot.at("slot-id").get<uint16_t>();

                if (dsPort.contains("admin-state"))
                    /* cache.SetDsAdminState(
                        static_cast<InterfaceEntity_t::AdminState_e>(
                            dsPort.at("admin-state").get<uint32_t>()),
                        rpdId, dsPortId); */
                        std::cout << "dsPort admin-state: " << parseAdminState(dsPort.at("admin-state")) << std::endl;

                if (dsPort.contains("up-down-trap-enabled"))
                            /* cache.SetDsUpDownTrapEnabled(
                                dsPort.at("up-down-trap-enabled").get<bool>(),
                                rpdId, dsPortId); */
                    std::cout << "dsPort up-down-trap-enabled: " << parseBoolValue(dsPort.at("up-down-trap-enabled")) << std::endl;

                // Down channels
                if (dsPort.contains("down-channel")) {
                    for (const auto& ch : dsPort.at("down-channel")) {
                        uint32_t chId = 0;
                        if (ch.contains("down-channel-id"))
                            chId = ch.at("down-channel-id").get<uint32_t>();

                        if (ch.contains("admin-state"))
                            /* cache.SetDsAdminState(
                                static_cast<InterfaceEntity_t::AdminState_e>(
                                    ch.at("admin-state").get<uint32_t>()),
                                rpdId, dsPortId, chId); */
                        std::cout << "down-channel admin-state: " << parseAdminState(ch.at("admin-state")) << std::endl;

                        if (ch.contains("up-down-trap-enabled"))
                            /* cache.SetDsUpDownTrapEnabled(
                                ch.at("up-down-trap-enabled").get<bool>(),
                                rpdId, dsPortId, chId); */
                        std::cout << "down-channel up-down-trap-enabled: " << parseBoolValue(ch.at("up-down-trap-enabled")) << std::endl;
                    }
                }
            }
        }

        // US RF Ports
        if (slot.contains("us-rf-port")) {
            for (const auto& usPort : slot.at("us-rf-port")) {
                uint32_t usPortId = 0;
                if (usPort.contains("us-rf-port-id"))
                    usPortId = usPort.at("us-rf-port-id").get<uint32_t>();

                uint16_t rpdId = 0;
                if (slot.contains("slot-id"))
                    rpdId = slot.at("slot-id").get<uint16_t>();

                if (usPort.contains("admin-state"))
                    /* cache.SetUsAdminState(
                        static_cast<InterfaceEntity_t::AdminState_e>(
                            usPort.at("admin-state").get<uint32_t>()),
                        rpdId, usPortId); */
                    std::cout << "usPort admin-state: " << parseAdminState(usPort.at("admin-state")) << std::endl;

                if (usPort.contains("up-down-trap-enabled"))
                    /* cache.SetUsUpDownTrapEnabled(
                        usPort.at("up-down-trap-enabled").get<bool>(),
                        rpdId, usPortId); */
                    std::cout << "usPort up-down-trap-enabled: " << parseBoolValue(usPort.at("up-down-trap-enabled")) << std::endl;

                // Upstream physical channels
                if (usPort.contains("upstream-physical-channel")) {
                    for (const auto& phyCh : usPort.at("upstream-physical-channel")) {
                        uint32_t phyChId = 0;
                        if (phyCh.contains("upstream-physical-channel-id"))
                            phyChId = phyCh.at("upstream-physical-channel-id").get<uint32_t>();

                        if (phyCh.contains("admin-state"))
                            /* cache.SetUsAdminState(
                                static_cast<InterfaceEntity_t::AdminState_e>(
                                    phyCh.at("admin-state").get<uint32_t>()),
                                rpdId, usPortId, phyChId); */
                            std::cout << "upstream-physical-channel admin-state: " << parseAdminState(phyCh.at("admin-state")) << std::endl;

                        if (phyCh.contains("up-down-trap-enabled"))
                            /* cache.SetUsUpDownTrapEnabled(
                                phyCh.at("up-down-trap-enabled").get<bool>(),
                                rpdId, usPortId, phyChId); */
                            std::cout << "upstream-physical-channel up-down-trap-enabled: " << parseBoolValue(phyCh.at("up-down-trap-enabled")) << std::endl;

                        // Upstream logical channels
                        if (phyCh.contains("upstream-logical-channel")) {
                            for (const auto& logCh : phyCh.at("upstream-logical-channel")) {
                                uint32_t logChId = 0;
                                if (logCh.contains("upstream-logical-channel-id"))
                                    logChId = logCh.at("upstream-logical-channel-id").get<uint32_t>();

                                if (logCh.contains("admin-state"))
                                    /* cache.SetUsAdminState(
                                        static_cast<InterfaceEntity_t::AdminState_e>(
                                            logCh.at("admin-state").get<uint32_t>()),
                                        rpdId, usPortId, phyChId, logChId); */
                                    std::cout << "upstream-logical-channel admin-state: " << parseAdminState(logCh.at("admin-state")) << std::endl;

                                if (logCh.contains("up-down-trap-enabled"))
                                    /* cache.SetUsUpDownTrapEnabled(
                                        logCh.at("up-down-trap-enabled").get<bool>(),
                                        rpdId, usPortId, phyChId, logChId); */
                                    std::cout << "upstream-logical-channel up-down-trap-enabled: " << parseBoolValue(logCh.at("up-down-trap-enabled")) << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}
enum EventPriority_e {
		DEBUG	,
		INFO	,
		NOTICE	,
		WARNING	,
		ERROR	,
		ALERT	,
		CRITICAL,
		EMERGENCY
	};

	enum EventDestination_e {
		SYSLOG 		= 2,
        SNMP		= 1,
		CONSOLE		= 4,
		LOCAL_LOG	= 0
	};

std::string toLowerAscii(std::string text)
{
    for (char& c : text) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c + ('a' - 'A'));
        }
    }
    return text;
}

template <typename EnumT, std::size_t N>
std::optional<EnumT> parseEnumFromJson(
    const nlohmann::json& value,
    const std::array<std::pair<std::string_view, EnumT>, N>& map)
{
    if (value.is_string()) {
        const std::string text = toLowerAscii(value.get<std::string>());
        for (const auto& [name, enumValue] : map) {
            if (text == name) {
                return enumValue;
            }
        }
        return std::nullopt;
    }

    if (value.is_number_integer() || value.is_number_unsigned()) {
        const auto raw = value.get<int64_t>();
        if (raw < 0) {
            return std::nullopt;
        }
        return static_cast<EnumT>(raw);
    }

    return std::nullopt;
}

static constexpr std::array<std::pair<std::string_view, EventPriority_e>, 9> kEventPriorityByName{{
    {"debug", DEBUG},
    {"information", INFO},
    {"info", INFO},
    {"notice", NOTICE},
    {"warning", WARNING},
    {"error", ERROR},
    {"alert", ALERT},
    {"critical", CRITICAL},
    {"emergency", EMERGENCY},
}};

static constexpr std::array<std::pair<std::string_view, EventDestination_e>, 4> kEventDestinationByName{{
    {"local", LOCAL_LOG},
    {"traps", SNMP},
    {"syslog", SYSLOG},
    {"console", CONSOLE},
}};

EventPriority_e parseEventPriority(const nlohmann::json& value)
{
    // Generic parser example: one reusable function + tiny enum map.
    if (const auto parsed = parseEnumFromJson<EventPriority_e>(value, kEventPriorityByName)) {
        return *parsed;
    }

    std::cout << "Warning: unsupported priority value/type" << std::endl;
    return INFO;
}

uint32_t parseReportingMask(const nlohmann::json& value)
{
    if (value.is_number_unsigned()) {
        return value.get<uint32_t>();
    }
    if (value.is_number_integer()) {
        const auto v = value.get<int64_t>();
        return v < 0 ? 0U : static_cast<uint32_t>(v);
    }

    // Common text representation: "local traps syslog".
    if (value.is_string()) {
        uint32_t mask = 0;
        std::istringstream iss(value.get<std::string>());
        std::string token;
        while (iss >> token) {
            for (char& c : token) {
                if (c >= 'A' && c <= 'Z') {
                    c = static_cast<char>(c + ('a' - 'A'));
                }
            }

            if (token == "local") {
                mask |= (1U << 0);
            } else if (token == "traps") {
                mask |= (1U << 1);
            } else if (token == "syslog") {
                mask |= (1U << 2);
            }
        }
        return mask;
    }

    std::cout << "Warning: unsupported reporting value/type" << std::endl;
    return 0U;
}

bool cParse(const nlohmann::json& config/* , CEventCache& cache */)
{
    static const nlohmann::json::json_pointer basePtr(
        "/data/ccap:ccap/management/fault-management/event-reporting-config");

    if (!config.contains(basePtr)) {
        // COSM_EVT_DEBUG("EventReportingParser: path not found in JSON");
        std::cout << "EventReportingParser: path not found in JSON" << std::endl;
        return false;
    }
    else
        // COSM_EVT_DEBUG("EventReportingParser: path found in JSON");
        std::cout << "EventReportingParser: path found in JSON" << std::endl;

    const auto& reports = config.at(basePtr);
    for (const auto& item : reports) {
        if (!item.contains("priority"))
            continue;

        auto priority = parseEventPriority(item.at("priority"));

        std::vector<EventDestination_e> destinations;
        uint32_t reportingMask = 0;
        if (item.contains("reporting")) {
            // uint32_t val = parseReportingMask(item.at("reporting"));
            // reportingMask = val;
            // int pow = 0;
            // for (uint32_t i = 1; i <= val; i <<= 1, ++pow) {
            //     if (i & val) {
            //         // destinations.push_back(EventConfig_t::EventDestination_e(pow));
            //     }
            // }
            for (const auto& dest : item.at("reporting")) {
                if (const auto parsedDest = parseEnumFromJson<EventDestination_e>(dest, kEventDestinationByName)) {
                    const auto destination = *parsedDest;
                    destinations.push_back(destination);

                    if (destination == LOCAL_LOG) {
                        reportingMask |= (1U << 0);
                    } else if (destination == SNMP) {
                        reportingMask |= (1U << 1);
                    } else if (destination == SYSLOG) {
                        reportingMask |= (1U << 2);
                    } else if (destination == CONSOLE) {
                        reportingMask |= (1U << 3);
                    }
                }
            }
        }

        // cache.SetDocsisEvtConfig(priority, destinations);
        std::cout <<"line:" << __LINE__ << " EventReportingParser: priority = " << static_cast<uint32_t>(priority) << ", reportingMask = " << reportingMask << ", destinations = ";
        // for (const auto& dest : destinations) {
        //     std::cout << static_cast<uint32_t>(dest) << " ";
        // }
        std::cout << std::endl;
    }
    return true;
}


int main()
{
    
   nlohmann::json parsed_json = json_file;

    // print the parsed JSON object
    // std::cout << "Parsed JSON object:\n" << parsed_json.dump(4) << std::endl;

    nlohmann::json::json_pointer ptr("/data/ccap:ccap/management/fault-management/event-reporting-config");

    // if (parsed_json.contains(ptr)) {
    //     // std::cout << "Pointer exists in the JSON object." << std::endl;
    //     nlohmann::json slot_array = parsed_json[ptr][0]["ds-rf-port"][0]["admin-state"];
    //     std::cout << "Slot array:\n" << slot_array.dump(2) << std::endl;
    // } else {
    //     std::cout << "Pointer does not exist in the JSON object." << std::endl;
    // }

    cParse(parsed_json);
    std::string rawViewName = "trap_aaa-server_status";
    std::vector<std::string> strs;
        boost::split(strs, rawViewName, boost::is_any_of("_"));
        if (strs.size() != 3) {
            std::cout << "ViewConfigParser: unexpected view-name format: " << rawViewName << std::endl;
        }
        std::string viewName = strs[1] + "_" + strs[2];
    std::cout << "View name: " << viewName << std::endl;
    return 0;
}
