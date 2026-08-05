#!/usr/bin/python3

import sys
import os
from subprocess import TimeoutExpired
import time
from logging import getLogger

import harmonic_bios_autoupdate.config as config
from .bios_update_tools import *
from .prepare_for_update import write_preparation_summary

#Based on https://www.intel.com/content/dam/support/us/en/documents/server-products/server-boards/intel-ofu-userguide-v1-03.pdf
ErrorCodesDict = {
    0 : "Successful termination",
    1 : "Invalid invocation or unknown command line argument",
    2 : "File was not found",
    3 : "Unable to read a file",
    4 : "The file update package is incompatible with the target server",
    5 : "A file in the update package is invalid or unsupported",
    6 : "Firmware interface failure (an error occurred when reading or writing to the BMC, setting the update notification, or updating the BMC, FRU, HSC, Intel®Local Control Panel, or SDR)",
    7 : "BIOS interface failure (an error occurred when reading the BIOS ID, setting the update notification, or updating the System BIOS)",
    8 : "Insufficient rights (the user must have Administrator or root rights)",
    9 : "Instance of another utility already running. If so, wait for the instance to finish and then start again",
    10: "Unknown error"
    }

# BIOS password rules:
#        An printable ASCII string containing, Lower case letters
#        or Upper case letters or Digits or special characters mentioned below
#        !  @  #  $  %  ^  &  * (  )  -  _  +  =  ?
#        A minimum of 1 characters and maximum 14 characters in length.
#        To clear the password, new password should be specified as "".

# Unfortunately, I see no option other than provide it as plain text
# revisit when we move to redfish
defaultBiosPassword = "'D31h20!6'"
blankBiosPassword = "''"
powerRestorePolicy = "on"
syscfgToolPath = "/usr/bin/syscfg/syscfg"
installationTraceFile="/var/lib/harmonic-bios-autoupdate/installation_started"

args = None
customBiosPassword = None
biosPasswordInUse = None
logger = None


def parse_arguments():
    parser = argparse.ArgumentParser(description='Process input arguments')
    parser.add_argument('-f', '--force', dest='force_update', action='store_true')
    parser.add_argument('-d', '--debug', dest='debug', action='store_true')
    parser.add_argument('--no-reboot', dest='no_reboot', action='store_true')
    parser.add_argument('-p', '--password', dest='password', default = None,
                        help = 'Provide custom BIOS password. Password must be encapsulated with an apostrophe (\'PASSWORD\')')
    return parser.parse_args()


def load_kernel_module(moduleName):
    exec_command("/sbin/modprobe {}".format(moduleName))


def get_bios_info():
    if get_board_name() == "M50FCP2SBSTD":
        exec_command("/usr/bin/sysfwupdt/sysfwupdt -i", logLevel = "info")
    else:
        exec_command("/usr/bin/flashupdt/flashupdt -i", logLevel = "info")


def format_password(password):
    return "'" + password.strip("'").strip('"').strip('`') + "'"


def set_bios_password(currentPassword, newPassword):
    cmd=" ".join([syscfgToolPath, "/bap", format_password(currentPassword), format_password(newPassword)])
    execResult = exec_command_no_logging(cmd, False)
    if execResult.returncode == 0:
        return True
    logger.error("BIOS set password failed with RC {}".format(execResult.returncode))
    return False


def check_bios_password():
    global biosPasswordInUse
    try:
        if get_board_name() == "M50FCP2SBSTD":
            #new bios versions on Fox Creek require a Blank password when upgrading
            if load_default_settings(defaultBiosPassword):
                if set_bios_password(defaultBiosPassword, blankBiosPassword):
                    biosPasswordInUse = blankBiosPassword
                    logger.debug("Blank BIOS password in use")
                    return True
                else:
                    logger.error("could not set a Blank password")
                    return False
            if load_default_settings(blankBiosPassword):
                biosPasswordInUse = blankBiosPassword
                logger.debug("Blank BIOS password in use")
                return True
            logger.error("Unknown BIOS password in use. aborting")
            return False
        if set_bios_password(defaultBiosPassword, defaultBiosPassword):
            biosPasswordInUse = defaultBiosPassword
            logger.debug("Default BIOS password in use")
            return True
        if set_bios_password(blankBiosPassword, blankBiosPassword):
            biosPasswordInUse = blankBiosPassword
            logger.debug("Blank BIOS password in use")
            return True
        if customBiosPassword is not None:
            if set_bios_password(customBiosPassword, customBiosPassword):
                biosPasswordInUse = customBiosPassword
                logger.debug("Custom BIOS password in use")
                return True

        logger.error("Unknown BIOS password in use. aborting")
        return False
    except:
        logger.error("Unknown error occurred")
        raise


def load_default_settings(biosPassword):
    cmd=" ".join([syscfgToolPath, "/bldfs", biosPassword])
    execResult = exec_command_no_logging(cmd, False)
    if execResult.returncode == 0:
        return True
    logger.error("Password Mismatch:entered password doesn't match with current password.")
    return False

def set_power_restore_policy():
    cmd=" ".join([syscfgToolPath, "/prp", powerRestorePolicy])
    execResult = exec_command_no_logging(cmd, False)
    if execResult.returncode == 0:
        return True
    logger.error("Password Mismatch:entered password doesn't match with current password.")
    return False

def extract_bios_files(boardName, platform):
    candidateFilename = "/opt/harmonic/bios-update/intel/{}_{}.zip".format(boardName, platform)
    if os.path.isfile(candidateFilename):
        zipFileName = candidateFilename
    else:
        logger.error("Matching zip file not found, aborting")
        return False
    if get_board_name() == "M50FCP2SBSTD":
        exec_command("unzip -j -o {} -d /tmp/intel_bios".format(zipFileName))
    else:
        exec_command("unzip -j -o {} -d /tmp/intel_bios".format(zipFileName))
        exec_command("sed -i 's/master\.cfg/sdr_update_noprompt\.cfg/' /tmp/intel_bios/flashupdt.cfg")
        if biosPasswordInUse != blankBiosPassword:
            biosPassToAdd = biosPasswordInUse.strip("'")
            exec_command_no_logging("sed -i 's/UpdateBackupBios/Password={}+UpdateBackupBios/' /tmp/intel_bios/flashupdt.cfg".format(biosPassToAdd))
    return True


def alert_users():
    exec_command("touch /run/bios-update-in-progress", False)
    exec_command("wall BIOS update in progress, do not touch setup until finished", False)


def update_bios():
    logger.info("Starting the update...")
    if get_board_name() == "M50FCP2SBSTD":
        exec_command("/usr/bin/sysfwupdt/sysfwupdt -u  /tmp/intel_bios/FCP_BB_v3p0_RP475p4_Q20p1_std_10M50_cfm1_auto_prd.bin", check_cmd_output = False, logLevel = "info")
        exec_command("/usr/bin/sysfwupdt/sysfwupdt -u  /tmp/intel_bios/fcp-signed-OBMC-egs-1.95-0-ga44f42-39e80000-pfr-oob.bin", check_cmd_output = False, logLevel = "info")
        exec_command("/usr/bin/sysfwupdt/sysfwupdt -u  /tmp/intel_bios/harmonic06_R01.02.0001_CAPSULE_Customized_signed.cap", check_cmd_output = False, logLevel = "info")
        return exec_command("/usr/bin/sysfwupdt/sysfwupdt -u  /tmp/intel_bios/R01.02.0001_FoxCreekPass_EBG_EMR_UpdateCapsule_prd.bin", check_cmd_output = False, logLevel = "info")
    else:
        return exec_command("/usr/bin/flashupdt/flashupdt -u /tmp/intel_bios/flashupdt.cfg", check_cmd_output = False, logLevel = "info")


def preparing_to_set_password():
    global biosPasswordInUse
    if biosPasswordInUse == blankBiosPassword:
        if set_bios_password(blankBiosPassword, defaultBiosPassword[:-2] + str(1) + '\''):
            biosPasswordInUse = defaultBiosPassword[:-2] + str(1) + '\''
    for i in [2, 3, 4, 5, 7]:
        if set_bios_password(biosPasswordInUse, biosPasswordInUse[:-2] + str(i) + '\''):
            biosPasswordInUse = biosPasswordInUse[:-2] + str(i) + '\''


def perform_update(boardName, platform):
    if not check_bios_password():
        return
    if not extract_bios_files(boardName, platform):
        return
    updateResult = update_bios()

    if updateResult.returncode != 0:
        logger.error("BIOS update terminated with an error.")
        logger.error("Installation RC: {} - {}".format(
            updateResult.returncode,
            ErrorCodesDict[updateResult.returncode] if updateResult.returncode < len(ErrorCodesDict) else "Unknown return code"
            )
        )
        if os.path.isfile(installationTraceFile): # Second failure in a row
            logger.error("BIOS automatic update service failed to update multiple times, disabling it.")
        else:
            exec_command("touch " + installationTraceFile)
            logger.error("Power cycling the machine for retry")

            # the BMC is a separate machine that exists inside the server.
            # once the command below executes, it sends a command to this machine to reboot. the call returns immediately.
            # the sleep is to prevent the server from rebooting during this period of time.
            # I tested empirically the mc reset times of the servers. We have no way to know it has finished (I asked intel...).
            # So I set this time with some buffer to be on the safe side.
            exec_command("ipmitool mc reset cold") # hard reset BMC to increase chances upon retry
            bufferSeconds = 30
            logger.info("Sleep until BMC completes reset")
            if boardName == "S2600WFQR":
                time.sleep(60 + bufferSeconds)
            elif boardName == "S2600WT2R":
                time.sleep(360 + bufferSeconds)
            elif boardName == "M50FCP2SBSTD":
                time.sleep(360 + bufferSeconds)

            exec_command("sync")
            if not args.no_reboot:
                exec_command("ipmitool power cycle")

    else:
        logger.info("BIOS update completed successfully")

        logger.info("Loading BIOS defaults")
        if customBiosPassword is not None:
            load_default_settings(customBiosPassword)
        else:
            load_default_settings(biosPasswordInUse)
            if boardName == "M50FCP2SBSTD":
                preparing_to_set_password()
                set_power_restore_policy()
            set_bios_password(biosPasswordInUse, defaultBiosPassword)


def disable_service():
    set_update_service_mode("disable")


def power_cycle_server():
    exec_command("ipmitool power cycle")


def main():
    global args, logger, customBiosPassword
    MyLogger()
    logger = logging.getLogger('bios-update')
    logger.info("lights on - perform_update")
    args = parse_arguments()
    if args.debug:
        MyLogger.set_console_log_level(logging.DEBUG)
    if args.password is not None:
        customBiosPassword = format_password(args.password)
    try:
        load_kernel_module("ipmi_devintf")
        boardName = get_board_name()
        platform = get_platform()
        get_bios_info() # debug only

        plans_dir = UPGRADE_PLANS_DIR
        state_file = PLAN_STATE_FILE_PATH

        # New approach first: only depends on plan files existing
        has_transition_plans = (
            os.path.isdir(plans_dir)
            and any(
                name.endswith(".json") and os.path.isfile(os.path.join(plans_dir, name))
                for name in os.listdir(plans_dir)
            )
        )

        if has_transition_plans:
            logger.info("Transition plans detected -> using new plan-based flow")
            alert_users()

            # Keep password handling consistent with the legacy update path.
            if not check_bios_password():
                logger.error("Unknown BIOS password in use. aborting")
                return

            result_msg, plan = auto_execute_bios_upgrade(
                plans_dir,
                state_file,
                current_bios_version=get_current_bios_version(),
                current_board_name=boardName,
                current_platform=platform,
                bios_password_in_use=biosPasswordInUse,
            )
            logger.info(result_msg)
            if plan:
                logger.info("Plan state: %s", plan.get("state"))
        else:
            if is_update_available(boardName, platform) or args.force_update:
                alert_users()
                perform_update(config.matchedBoardName, platform)

    except subprocess.TimeoutExpired:
        logger.error("aborting the upgrade due to command execution timeout")

    finally:
        config.preinstallSummary["installation_pending"] = False
        write_preparation_summary(config)

        disable_service()
        exec_command("sync")
        if not args.no_reboot:
            exec_command("ipmitool power cycle")

if __name__=="__main__":
    main()
