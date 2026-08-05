#!/usr/bin/python3

BIOSVersionsDict = {
    #WolfPass board (Skylake, Cascadelake, Cascadelake-R)
     "S2600WFQR":{
                "COS"              : "SE5C620.86B.02.01.0013.C0003.121520200651",
                "APOLLO_PLATFORM"  : "SE5C620.86B.02.01.0017.C0013.110620230543"
                 },
    # Wildcat board (Haswell, Broadwell)
    "S2600WT2R":{
                "APOLLO_PLATFORM" : "SE5C610.86B.01.01.0028.C9.121720182203",
                },
    "M50FCP2SBSTD":{
                "APOLLO_PLATFORM" : "SE5C741.86B.01.02.0004.250610232",
                }

}

matchedBoardName = None

preinstallSummary = {
    "board" : None,
    "matched_board" : None,
    "platform" : None,
    "candidate_version" : None,
    "update_available" : False,
    "installation_pending" : False
    }

packageDataDir = "/var/lib/harmonic-bios-autoupdate/"

PAYLOAD_BASE_PATH = "/opt/harmonic/bios-update/intel"


# Single source of truth for planner jumps and execution metadata.
# Keyed by board → platform → list of jump objects.
STEP_RECIPES = {
    "M50FCP2SBSTD": {
        "APOLLO_PLATFORM": [
            {
                "from_version": "SE5C741.86B.01.00.0005",
                "to_version": "SE5C741.86B.01.01.0008",
                "payload_filename": "M50FCP2SBSTD_APOLLO_PLATFORM_01010008.zip",
                "name": "stack_upgrade_01010008",
                "recipe": "foxcreek_r01000005_to_r01010008",
                "actions": [
                    {"type": "run_shell", "command": "startup.nsh", "reset_after_action": False},
                ],
                "warnings": [],
            },
            {
                "from_version": "SE5C741.86B.01.01.0008",
                "to_version": "SE5C741.86B.01.02.0001",
                "payload_filename": "M50FCP2SBSTD_APOLLO_PLATFORM_01020001.zip",
                "name": "official_stack_upgrade",
                "recipe": "foxcreek_r01010008_to_r01020001",
                "actions": [
                    {"type": "update_component", "component": "cpld", "version": "V3P0", "command": "update_component --component cpld --version V3P0 --reset none", "reset_after_action": False},
                    {"type": "update_component", "component": "bmc", "version": "1.81", "command": "update_component --component bmc --version 1.81 --reset system_reset", "reset_after_action": True},
                    {"type": "run_shell", "command": "startup.nsh", "reset_after_action": False},
                ],
                "warnings": [
                    "First reboot may take around 13 minutes",
                    "A second reboot is expected",
                ],
            },
            {
                "from_version": "SE5C741.86B.01.02.0001",
                "to_version": "SE5C741.86B.01.02.0001.2401260138",
                "payload_filename": "M50FCP2SBSTD_APOLLO_PLATFORM_custom.zip",
                "name": "custom_cap_upgrade",
                "recipe": "foxcreek_official_to_custom_cap",
                "actions": [
                    {"type": "run_shell", "command": "UpdBIOS_FCP_CAP.nsh", "reset_after_action": False},
                ],
                "warnings": [
                    "Run only after startup.nsh completed successfully",
                ],
            },
        ]
    },
    "S2600WFQR": {
        "APOLLO_PLATFORM": [
            {
                "from_version": "SE5C620.86B.02.01.0010.C0010",
                "to_version": "SE5C620.86B.02.01.0013.C0013",
                "payload_filename": "S2600WFQR_APOLLO_PLATFORM_0013.zip",
                "name": "stack_upgrade_0013",
                "recipe": "wolfpass_0010_to_0013",
                "actions": [
                    {"type": "run_shell", "command": "startup.nsh", "reset_after_action": False},
                ],
                "warnings": [],
            },
            {
                "from_version": "SE5C620.86B.02.01.0013.C0013",
                "to_version": "SE5C620.86B.02.01.0013.C0013.121520200651",
                "payload_filename": "S2600WFQR_APOLLO_PLATFORM_custom.zip",
                "name": "custom_cap_upgrade",
                "recipe": "wolfpass_official_to_custom_cap",
                "actions": [
                    {"type": "run_shell", "command": "UpdBIOS_WFQ_CAP.nsh", "reset_after_action": False},
                ],
                "warnings": [],
            },
        ]
    },
}


def _build_step_recipes_map(step_recipes):
    """Convert recipe list into lookup map keyed by (from_version, to_version)."""
    recipes_map = {}
    for board, platforms in step_recipes.items():
        if board.startswith("_"):
            continue
        recipes_map[board] = {}
        for platform, recipes in platforms.items():
            if not isinstance(recipes, list):
                continue
            recipes_map[board][platform] = {
                (r["from_version"], r["to_version"]): r
                for r in recipes
            }
    return recipes_map


STEP_RECIPES_MAP = _build_step_recipes_map(STEP_RECIPES)


def _build_version_jumps_map_from_step_recipes(step_recipes):
    """Build VERSION_JUMPS lookup from STEP_RECIPES payload filenames."""
    version_jumps = {}
    for board, platforms in step_recipes.items():
        if board.startswith("_"):
            continue
        version_jumps[board] = {}
        for platform, jumps in platforms.items():
            if not isinstance(jumps, list):
                continue
            jump_map = {}
            for jump in jumps:
                from_version = jump["from_version"]
                to_version = jump["to_version"]
                payload_filename = jump["payload_filename"]
                jump_map[(from_version, to_version)] = payload_filename
            version_jumps[board][platform] = jump_map
    return version_jumps


# Planner uses this runtime map.
VERSION_JUMPS = _build_version_jumps_map_from_step_recipes(STEP_RECIPES)
