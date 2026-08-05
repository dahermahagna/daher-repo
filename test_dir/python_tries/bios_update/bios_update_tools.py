#!/usr/bin/python3

import subprocess
import logging
import logging.handlers
import os
import sys
import argparse
import re
import json
import copy

import harmonic_bios_autoupdate.config as config

global logDirPath
logDirPath = '/var/log'
global logName
logName = 'bios-update.log'

logger = None



PLAN_STATE_FILE_PATH = "/var/lib/harmonic-bios-autoupdate/upgrade_plan_state.json"
UPGRADE_PLANS_DIR = "/var/lib/harmonic-bios-autoupdate/upgrade_plans"
RUNTIME_STATE_FILE_VERSION = 1.00
STATE_VALUES = {
    "allowed_states": {"pending", "in_progress", "completed", "failed"},
    "active_state": {"pending", "in_progress"},
    "final_state": {"completed", "failed"},
}

class PlanValidationError(ValueError):
    """Structured validation error carrying a stable error code."""

    def __init__(self, code, message):
        super().__init__(message)
        self.code = code

class UpgradeStepExecutor:
    """Execution layer for loading state, merging plans, and running actions."""

    def __init__(self, manager):
        self.manager = manager

    def loadBasePlan(self, plan_file_path):
        with open(plan_file_path, 'r') as f:
            base_plan = json.load(f)
        self.manager.validate_base_upgrade_plan(base_plan)
        return base_plan

    def loadOrInitRuntimeState(self, base_plan, state_file_path):
        if os.path.exists(state_file_path):
            with open(state_file_path, 'r') as f:
                state = json.load(f)
            try:
                self.manager.validate_upgrade_state(state, len(base_plan["actions"]))
                if not self.manager.runtime_state_matches_base_plan(state, base_plan):
                    logger.info(
                        "Runtime state at %s belongs to a different plan; reinitializing state",
                        state_file_path,
                    )
                    state = self.manager.default_state_from_base_plan(base_plan)
            except PlanValidationError:
                # State can belong to a different/older plan. Reinitialize for selected plan.
                logger.info(
                    "Runtime state at %s does not match selected plan; reinitializing state",
                    state_file_path,
                )
                state = self.manager.default_state_from_base_plan(base_plan)
        else:
            state = self.manager.default_state_from_base_plan(base_plan)
        self.manager.validate_upgrade_state(state, len(base_plan["actions"]))
        return state

    def mergePlanAndState(self, base_plan, runtime_state):
        merged_plan = copy.deepcopy(base_plan)
        merged_plan["max_attempts_per_action"] = runtime_state["max_attempts_per_action"]
        merged_plan["current_action_index"] = runtime_state["current_action_index"]
        merged_plan["current_action_id"] = runtime_state["current_action_id"]
        merged_plan["last_completed_action_id"] = runtime_state["last_completed_action_id"]
        merged_plan["state"] = runtime_state["state"]
        merged_plan["last_error"] = runtime_state["last_error"]
        for i in range(len(merged_plan["actions"])):
            merged_plan["actions"][i]["action_id"] = runtime_state["actions"][i]["action_id"]
            merged_plan["actions"][i]["status"] = runtime_state["actions"][i]["status"]
            merged_plan["actions"][i]["attempts"] = runtime_state["actions"][i]["attempts"]

        if merged_plan.get("current_action_index") is None:
            merged_plan["current_action_index"] = self.manager._first_pending_action_index_from_state(runtime_state)
        if not merged_plan.get("last_completed_action_id"):
            merged_plan["last_completed_action_id"] = self.manager._last_completed_action_id_from_state(runtime_state)

        self.manager.validate_upgrade_plan(merged_plan)
        return merged_plan

    def saveRuntimeState(self, plan, state_file_path):
        self.manager.validate_upgrade_plan(plan)
        runtime_state = self.manager.extract_runtime_state_from_plan(plan)
        state_dir = os.path.dirname(os.path.abspath(state_file_path))
        if state_dir:
            os.makedirs(state_dir, exist_ok=True)
        with open(state_file_path, 'w') as f:
            json.dump(runtime_state, f, indent=2)
        logger.info(f"Upgrade state saved to {state_file_path}")
        return True

    def _find_next_pending_action(self, plan):
        for action_index, action in enumerate(plan["actions"]):
            if action.get("status") != "completed":
                return action_index
        return None

    def executeNextAction(self, plan):
        self.manager.validate_upgrade_plan(plan)

        if plan["state"] in STATE_VALUES["final_state"]:
            return plan

        while True:
            action_index = self._find_next_pending_action(plan)
            if action_index is None:
                plan["state"] = "completed"
                plan["current_action_index"] = len(plan["actions"])
                plan["current_action_id"] = None
                plan["last_error"] = None
                return plan

            action = plan["actions"][action_index]
            plan["current_action_index"] = action_index
            plan["current_action_id"] = action.get("action_id")
            plan["state"] = "in_progress"
            action["status"] = "in_progress"
            action["attempts"] = int(action.get("attempts", 0)) + 1

            try:
                exec_command(action["command"])
                action["status"] = "completed"
                plan["last_completed_action_id"] = action.get("action_id")
                plan["last_error"] = None

                next_action_index = self._find_next_pending_action(plan)
                if next_action_index is None:
                    plan["state"] = "completed"
                    plan["current_action_index"] = len(plan["actions"])
                    plan["current_action_id"] = None
                else:
                    plan["current_action_index"] = next_action_index
                    plan["current_action_id"] = plan["actions"][next_action_index].get("action_id")
                if action.get("reset_after_action", False):
                    return plan  # Exit for reboot
            except Exception as exc:
                max_attempts = plan["max_attempts_per_action"]
                plan["last_error"] = str(exc)
                if action["attempts"] >= max_attempts:
                    action["status"] = "failed"
                    plan["state"] = "failed"
                else:
                    action["status"] = "pending"
                    plan["state"] = "in_progress"
                return plan

class UpgradePlanManager:
    """Validation and orchestration layer for upgrade plans and runtime state."""

    def __init__(self, state_file_path=PLAN_STATE_FILE_PATH):
        self.state_file_path = state_file_path
        self.executor = UpgradeStepExecutor(self)

    def is_non_empty_string(self, value):
        return isinstance(value, str) and value.strip() != ""

    def plan_target_matches(self, current_value, plan_value):
        # Single-target plan matching.
        return current_value == plan_value

    def validate_upgrade_plan(self, plan):
        required_keys = [
            "from_version",
            "to_version",
            "board_name",
            "platform",
            "payload_path",
            "actions",
            "current_action_index",
            "max_attempts_per_action",
            "state",
            "last_error",
        ]
        if not isinstance(plan, dict):
            raise PlanValidationError("INVALID_PLAN_TYPE", "plan must be a dict")

        if not all(key in plan.keys() for key in required_keys):
            missing_keys = [key for key in required_keys if key not in plan]
            raise PlanValidationError("MISSING_PLAN_FIELD", f"missing plan fields: {', '.join(missing_keys)}")

        if not self.is_non_empty_string(plan["from_version"]):
            raise PlanValidationError("INVALID_PLAN_FROM_VERSION", "plan.from_version must be a non-empty string")
        if not self.is_non_empty_string(plan["to_version"]):
            raise PlanValidationError("INVALID_PLAN_TO_VERSION", "plan.to_version must be a non-empty string")
        if not self.is_non_empty_string(plan["payload_path"]):
            raise PlanValidationError("INVALID_PLAN_PAYLOAD_PATH", "plan.payload_path must be a non-empty string")

        board_name = plan.get("board_name")
        if not self.is_non_empty_string(board_name):
            raise PlanValidationError(
                "INVALID_PLAN_BOARD_NAME",
                "plan.board_name must be a non-empty string",
            )

        platform = plan.get("platform")
        if not self.is_non_empty_string(platform):
            raise PlanValidationError(
                "INVALID_PLAN_PLATFORM",
                "plan.platform must be a non-empty string",
            )

        if not isinstance(plan["actions"], list) or len(plan["actions"]) == 0:
            raise PlanValidationError("INVALID_PLAN_ACTIONS", "plan.actions must be a non-empty list")
        if not isinstance(plan["current_action_index"], int) or plan["current_action_index"] < 0:
            raise PlanValidationError("INVALID_ACTION_INDEX", "current_action_index must be a non-negative int")
        if not isinstance(plan["max_attempts_per_action"], int) or plan["max_attempts_per_action"] < 1:
            raise PlanValidationError("INVALID_MAX_ATTEMPTS", "max_attempts_per_action must be >= 1")

        if plan["state"] not in STATE_VALUES["allowed_states"]:
            raise PlanValidationError(
                "INVALID_PLAN_STATE",
                f"state must be one of {sorted(STATE_VALUES['allowed_states'])}",
            )

        for i, action in enumerate(plan["actions"]):
            if not isinstance(action, dict):
                raise PlanValidationError("INVALID_ACTION_TYPE", f"action at index {i} must be a dict")
            for action_key in ["action_id", "command", "status", "attempts"]:
                if action_key not in action:
                    raise PlanValidationError("MISSING_ACTION_FIELD", f"action[{i}] missing field: {action_key}")
            if not self.is_non_empty_string(action["action_id"]):
                raise PlanValidationError("INVALID_ACTION_ID", f"action[{i}] action_id must be non-empty string")
            if not self.is_non_empty_string(action["command"]):
                raise PlanValidationError("INVALID_ACTION_COMMAND", f"action[{i}] command must be non-empty string")
            if action["status"] not in STATE_VALUES["allowed_states"]:
                raise PlanValidationError("INVALID_ACTION_STATUS", f"action[{i}].status is invalid")
            if not isinstance(action["attempts"], int) or action["attempts"] < 0:
                raise PlanValidationError("INVALID_ACTION_ATTEMPTS", f"action[{i}].attempts must be non-negative int")

        if plan["current_action_index"] > len(plan["actions"]):
            raise PlanValidationError("ACTION_INDEX_OUT_OF_RANGE", "current_action_index out of actions bounds")

        return True

    def validate_base_upgrade_plan(self, base_plan):
        required_keys = [
            "from_version",
            "to_version",
            "board_name",
            "platform",
            "payload_path",
            "actions",
        ]
        if not isinstance(base_plan, dict):
            raise PlanValidationError("INVALID_BASE_PLAN_TYPE", "base plan must be a dict")

        if not all(key in base_plan.keys() for key in required_keys):
            missing_keys = [key for key in required_keys if key not in base_plan]
            raise PlanValidationError("MISSING_BASE_PLAN_FIELD", f"missing base plan fields: {', '.join(missing_keys)}")

        if not self.is_non_empty_string(base_plan["from_version"]):
            raise PlanValidationError("INVALID_BASE_PLAN_FROM_VERSION", "base plan from_version must be a non-empty string")
        if not self.is_non_empty_string(base_plan["to_version"]):
            raise PlanValidationError("INVALID_BASE_PLAN_TO_VERSION", "base plan to_version must be a non-empty string")
        if not self.is_non_empty_string(base_plan["payload_path"]):
            raise PlanValidationError("INVALID_BASE_PLAN_PAYLOAD_PATH", "base plan payload_path must be a non-empty string")

        board_name = base_plan.get("board_name")
        if not self.is_non_empty_string(board_name):
            raise PlanValidationError(
                "INVALID_BASE_PLAN_BOARD_NAME",
                "base plan board_name must be a non-empty string",
            )

        platform = base_plan.get("platform")
        if not self.is_non_empty_string(platform):
            raise PlanValidationError(
                "INVALID_BASE_PLAN_PLATFORM",
                "base plan platform must be a non-empty string",
            )

        if not isinstance(base_plan["actions"], list) or len(base_plan["actions"]) == 0:
            raise PlanValidationError("INVALID_BASE_PLAN_ACTIONS", "base plan actions must be a non-empty list")

        for i, action in enumerate(base_plan["actions"]):
            if not isinstance(action, dict):
                raise PlanValidationError("INVALID_BASE_ACTION_TYPE", f"action[{i}] must be a dict")
            for action_key in ["action_id", "command"]:
                if action_key not in action:
                    raise PlanValidationError(
                        "MISSING_BASE_ACTION_FIELD",
                        f"action[{i}] missing field: {action_key}",
                    )
            if not self.is_non_empty_string(action["action_id"]):
                raise PlanValidationError("INVALID_BASE_ACTION_ID", f"action[{i}] has invalid action_id")
            if not self.is_non_empty_string(action["command"]):
                raise PlanValidationError("INVALID_BASE_ACTION_COMMAND", f"action[{i}] has invalid command")
            if "reset_after_action" in action and not isinstance(action["reset_after_action"], bool):
                raise PlanValidationError(
                    "INVALID_BASE_ACTION_RESET_AFTER_ACTION",
                    f"action[{i}] reset_after_action must be boolean",
                )

        return True

    def _first_pending_action_index_from_state(self, state):
        for action_index, action_state in enumerate(state["actions"]):
            if action_state["status"] != "completed":
                return action_index
        return None

    def _last_completed_action_id_from_state(self, state):
        for action_state in reversed(state["actions"]):
            if action_state["status"] == "completed":
                return action_state["action_id"]
        return None

    def default_state_from_base_plan(self, base_plan):
        state = {
            "runtime_file_version": RUNTIME_STATE_FILE_VERSION,
            "from_version": base_plan["from_version"],
            "to_version": base_plan["to_version"],
            "board_name": base_plan.get("board_name"),
            "platform": base_plan.get("platform"),
            "max_attempts_per_action": int(base_plan.get("max_attempts_per_action", 2)),
            "current_action_index": 0,
            "current_action_id": (
                base_plan["actions"][0].get("action_id")
                if base_plan.get("actions")
                else None
            ),
            "last_completed_action_id": None,
            "state": "pending",
            "last_error": None,
            "actions": [
                {
                    "action_id": action["action_id"],
                    "status": "pending",
                    "attempts": 0,
                }
                for action in base_plan.get("actions", [])
            ],
        }
        return state

    def runtime_state_matches_base_plan(self, state, base_plan):
        if state.get("from_version") != base_plan.get("from_version"):
            return False
        if state.get("to_version") != base_plan.get("to_version"):
            return False
        if state.get("board_name") != base_plan.get("board_name"):
            return False
        if state.get("platform") != base_plan.get("platform"):
            return False

        state_actions = state.get("actions", [])
        base_actions = base_plan.get("actions", [])
        if len(state_actions) != len(base_actions):
            return False

        for i in range(len(base_actions)):
            if state_actions[i].get("action_id") != base_actions[i].get("action_id"):
                return False

        return True

    def validate_upgrade_state(self, state, actions_count):
        if not isinstance(state, dict):
            raise PlanValidationError("INVALID_STATE_TYPE", "state file must be a dict")

        required_keys = [
            "runtime_file_version",
            "from_version",
            "to_version",
            "board_name",
            "platform",
            "max_attempts_per_action",
            "current_action_index",
            "current_action_id",
            "last_completed_action_id",
            "state",
            "last_error",
            "actions",
        ]
        for key in required_keys:
            if key not in state:
                raise PlanValidationError("MISSING_STATE_FIELD", f"missing state field: {key}")

        runtime_file_version = state.get("runtime_file_version")
        if not isinstance(runtime_file_version, (int, float)) or float(runtime_file_version) != float(RUNTIME_STATE_FILE_VERSION):
            raise PlanValidationError(
                "STATE_FILE_VERSION_MISMATCH",
                f"runtime_file_version {runtime_file_version} does not match expected {RUNTIME_STATE_FILE_VERSION}",
            )

        if not isinstance(state["max_attempts_per_action"], int) or state["max_attempts_per_action"] < 1:
            raise PlanValidationError("INVALID_STATE_MAX_ATTEMPTS", "max_attempts_per_action must be >= 1")
        if not self.is_non_empty_string(state["board_name"]):
            raise PlanValidationError("INVALID_STATE_BOARD_NAME", "state board_name is invalid")
        if not self.is_non_empty_string(state["platform"]):
            raise PlanValidationError("INVALID_STATE_PLATFORM", "state platform is invalid")
        if not isinstance(state["current_action_index"], int) or state["current_action_index"] < 0:
            raise PlanValidationError("INVALID_STATE_ACTION_INDEX", "current_action_index must be a non-negative int")
        if state["state"] not in STATE_VALUES["allowed_states"]:
            raise PlanValidationError(
                "INVALID_STATE_VALUE",
                f"state has invalid value, must be one of {sorted(STATE_VALUES['allowed_states'])}"
            )
        if not isinstance(state["actions"], list) or len(state["actions"]) != actions_count:
            raise PlanValidationError("INVALID_STATE_ACTIONS", "state actions length must match base plan actions")

        for i, action_state in enumerate(state["actions"]):
            if not isinstance(action_state, dict):
                raise PlanValidationError("INVALID_ACTION_STATE_TYPE", f"state action {i} must be a dict")
            if not self.is_non_empty_string(action_state.get("action_id")):
                raise PlanValidationError("INVALID_ACTION_STATE_ID", f"invalid action_id for state action {i}")
            if action_state.get("status") not in STATE_VALUES["allowed_states"]:
                raise PlanValidationError("INVALID_ACTION_STATE_STATUS", f"invalid status for state action {i}")
            if not isinstance(action_state.get("attempts"), int) or action_state["attempts"] < 0:
                raise PlanValidationError("INVALID_ACTION_STATE_ATTEMPTS", f"invalid attempts for state action {i}")

        if state["current_action_index"] > actions_count:
            raise PlanValidationError("STATE_ACTION_INDEX_OUT_OF_RANGE", "state current_action_index out of range")

        return True

    def extract_runtime_state_from_plan(self, plan):
        current_action_id = plan.get("current_action_id")
        last_completed_action_id = plan.get("last_completed_action_id")
        return {
            "runtime_file_version": RUNTIME_STATE_FILE_VERSION,
            "from_version": plan["from_version"],
            "to_version": plan["to_version"],
            "board_name": plan.get("board_name"),
            "platform": plan.get("platform"),
            "max_attempts_per_action": plan["max_attempts_per_action"],
            "current_action_index": plan["current_action_index"],
            "current_action_id": current_action_id,
            "last_completed_action_id": last_completed_action_id,
            "state": plan["state"],
            "last_error": plan.get("last_error"),
            "actions": [
                {
                    "action_id": action.get("action_id"),
                    "status": action.get("status", "pending"),
                    "attempts": action.get("attempts", 0),
                }
                for action in plan["actions"]
            ],
        }

    def save_upgrade_state(self, plan, state_file_path=None):
        try:
            target_path = state_file_path or self.state_file_path
            return self.executor.saveRuntimeState(plan, target_path)
        except Exception as e:
            logger.error(f"Failed to save state: {e}")
            return False

    def load_upgrade_plan_with_state(self, plan_file_path, state_file_path=None):
        try:
            source_state_file_path = state_file_path or self.state_file_path
            base_plan = self.executor.loadBasePlan(plan_file_path)
            runtime = self.executor.loadOrInitRuntimeState(base_plan, source_state_file_path)
            return self.executor.mergePlanAndState(base_plan, runtime)
        except Exception as e:
            logger.error(f"Failed to load plan/state: {e}")
            return None

    def extract_bios_files_from_plan_payload(self, plan, bios_password_in_use=None, current_board_name=None):
        zip_file_name = plan.get("payload_path")
        if not self.is_non_empty_string(zip_file_name) or not os.path.isfile(zip_file_name):
            logger.error("Payload file not found: %s", zip_file_name)
            return False

        try:
            exec_command("unzip -j -o {} -d /tmp/intel_bios".format(zip_file_name))
            board_name = current_board_name or get_board_name()
            cfg_path = "/tmp/intel_bios/flashupdt.cfg"
            if board_name != "M50FCP2SBSTD" and os.path.isfile(cfg_path):
                exec_command("sed -i 's/master\\.cfg/sdr_update_noprompt\\.cfg/' {}".format(cfg_path))
                bios_pass_to_add = "" if bios_password_in_use in {None, "''"} else str(bios_password_in_use).strip("'")
                if bios_pass_to_add:
                    exec_command_no_logging("sed -i 's/Password/Password={}/' {}".format(bios_pass_to_add, cfg_path))
                    exec_command_no_logging("sed -i 's/UpdateBackupBios/Password={}+UpdateBackupBios/' {}".format(bios_pass_to_add, cfg_path))
            logger.info("Extracted BIOS files from %s", zip_file_name)
            return True
        except Exception as e:
            logger.error("Failed to extract BIOS files from %s: %s", zip_file_name, e)
            return False

    def execute_next_action(self, plan):
        try:
            return self.executor.executeNextAction(plan)
        except Exception as e:
            logger.error(f"Failed to execute next action: {e}")
            return None

def auto_execute_bios_upgrade(
    plans_dir,
    state_file=PLAN_STATE_FILE_PATH,
    current_bios_version=None,
    current_board_name=None,
    current_platform=None,
    bios_password_in_use=None,
):
    """
    Main orchestration: check pending plan, if none then select by current BIOS version, execute, and persist state.
    Returns (result_str, plan_dict or None)
    """
    manager = UpgradePlanManager(state_file_path=state_file)

    # Step 1: Check if there is a pending plan from state file
    pending_plan_file = find_pending_plan_from_state(plans_dir, state_file)
    if pending_plan_file:
        logger.info("Resuming pending plan: %s", pending_plan_file)
        plan_file = pending_plan_file
    else:
        # Step 2: Check current BIOS version and find matching plan
        try:
            plan_file = select_transition_plan_for_bios_version(
                plans_dir,
                current_bios_version=current_bios_version,
                current_board_name=current_board_name,
                current_platform=current_platform,
            )
            logger.info("Selected plan for current BIOS version: %s", plan_file)
        except PlanValidationError as e:
            if e.code == "NO_MATCHING_PLAN":
                logger.info("No transition plan needs to be applied")
                return "No transition plan needs to be applied", None
            raise

    # Step 3: Load plan and state, execute next action
    plan = manager.load_upgrade_plan_with_state(plan_file, state_file)
    if plan is None:
        raise RuntimeError(f"Unable to load plan/state from {plan_file}")
    # Step 4: extract BIOS payload content before running plan actions.
    if not manager.extract_bios_files_from_plan_payload(
        plan,
        bios_password_in_use=bios_password_in_use,
        current_board_name=current_board_name,
    ):
        raise RuntimeError("Unable to extract BIOS files from plan payload")

    plan = manager.execute_next_action(plan)
    if plan is None:
        raise RuntimeError("Unable to execute next action")

    # Step 5: Persist state
    if not manager.save_upgrade_state(plan, state_file):
        raise RuntimeError(f"Unable to save state to {state_file}")

    logger.info("Upgrade step completed: %s -> %s", plan["from_version"], plan["to_version"])
    return f"Upgrade from {plan['from_version']} to {plan['to_version']} applied", plan

def _load_valid_transition_plans(plans_dir):
    """Return [(path, plan_dict)] for all valid transition plans in plans_dir."""
    manager = UpgradePlanManager()

    if not manager.is_non_empty_string(plans_dir):
        raise PlanValidationError("INVALID_PLANS_DIR", "plans_dir must be a non-empty string")
    if not os.path.isdir(plans_dir):
        raise PlanValidationError("PLANS_DIR_NOT_FOUND", f"plans directory not found: {plans_dir}")

    plans = []
    for name in sorted(os.listdir(plans_dir)):
        if not name.endswith(".json"):
            continue
        file_path = os.path.join(plans_dir, name)
        if not os.path.isfile(file_path):
            continue
        try:
            with open(file_path, "r") as f:
                plan = json.load(f)
            manager.validate_base_upgrade_plan(plan)
            plans.append((file_path, plan))
        except Exception:
            # Ignore non-plan JSON files.
            continue
    return plans

def find_pending_plan_from_state(plans_dir, state_file_path):
    """Return a matching plan path when runtime state indicates pending/in-progress work."""
    manager = UpgradePlanManager()

    if not os.path.exists(state_file_path):
        return None

    try:
        with open(state_file_path, "r") as f:
            state = json.load(f)
    except Exception:
        return None

    if state.get("state") not in STATE_VALUES["active_state"]:
        return None

    from_version = state.get("from_version")
    to_version = state.get("to_version")
    board_name = state.get("board_name")
    platform = state.get("platform")
    if not from_version or not to_version:
        return None

    plans = _load_valid_transition_plans(plans_dir)

    matches = []
    for plan_path, plan in plans:
        plan_from_version = plan.get("from_version")
        plan_to_version = plan.get("to_version")
        plan_board_name = plan.get("board_name")
        plan_platform = plan.get("platform")
        if (
            manager.plan_target_matches(from_version, plan_from_version)
            and manager.plan_target_matches(to_version, plan_to_version)
            and (
                not manager.is_non_empty_string(board_name)
                or manager.plan_target_matches(board_name, plan_board_name)
            )
            and (
                not manager.is_non_empty_string(platform)
                or manager.plan_target_matches(platform, plan_platform)
            )
        ):
            matches.append(plan_path)

    if len(matches) == 1:
        return matches[0]
    if len(matches) > 1:
        raise PlanValidationError(
            "AMBIGUOUS_PENDING_PLAN",
            f"Multiple plans match pending state ({from_version} -> {to_version}): {matches}",
        )
    return None

def select_transition_plan_for_bios_version(
    plans_dir,
    current_bios_version=None,
    current_board_name=None,
    current_platform=None,
):
    """Select exactly one transition plan for BIOS version, board name and platform."""
    manager = UpgradePlanManager()

    bios_version = current_bios_version or get_current_bios_version()
    if not manager.is_non_empty_string(bios_version):
        raise PlanValidationError("INVALID_CURRENT_BIOS_VERSION", "current BIOS version is empty")

    board_name = current_board_name or get_board_name()
    if not manager.is_non_empty_string(board_name):
        raise PlanValidationError("INVALID_CURRENT_BOARD_NAME", "current board name is empty")

    platform = current_platform or get_platform()
    if not manager.is_non_empty_string(platform):
        raise PlanValidationError("INVALID_CURRENT_PLATFORM", "current platform is empty")

    plans = _load_valid_transition_plans(plans_dir)
    matches = []
    candidate_transitions = []

    for plan_path, plan in plans:
        from_version = plan.get("from_version")
        plan_board_name = plan.get("board_name")
        plan_platform = plan.get("platform")

        if manager.is_non_empty_string(from_version):
            candidate_transitions.append(
                {
                    "from_version": from_version,
                    "board_name": plan_board_name,
                    "platform": plan_platform,
                }
            )

        if (
            manager.plan_target_matches(bios_version, from_version)
            and manager.plan_target_matches(board_name, plan_board_name)
            and manager.plan_target_matches(platform, plan_platform)
        ):
            matches.append(plan_path)

    if len(matches) == 1:
        return matches[0]
    if len(matches) > 1:
        raise PlanValidationError(
            "AMBIGUOUS_MATCHING_PLANS",
            f"Multiple transition plans match BIOS version {bios_version}: {matches}",
        )

    raise PlanValidationError(
        "NO_MATCHING_PLAN",
        (
            "No transition plan matches current BIOS version/board/platform "
            f"({bios_version}, {board_name}, {platform}). "
            f"Available plan targets: {candidate_transitions}"
        ),
    )

def exec_command(cmd, check_cmd_output = True, logLevel = "debug"):
    try:
        loggingFunc = logger.__getattribute__(logLevel)
        loggingFunc("Executing command: {}".format(cmd))
        execResult = subprocess.run(cmd, shell=True, text=True, stderr = subprocess.STDOUT, stdout = subprocess.PIPE, check = check_cmd_output, timeout = 900)
        execOutput = execResult.stdout.strip()

        if execResult.returncode != 0:
            loggingFunc("Execution RC:{}".format(execResult.returncode))

        if execOutput != "":
            loggingFunc("Execution output:\n{}".format(execOutput))
        return execResult
    except Exception as e:
        logger.error(e)
        raise


def exec_command_no_logging(cmd, check_cmd_output = True):
    try:
        return subprocess.run(cmd, shell=True, text=True, stderr = subprocess.STDOUT, stdout = subprocess.PIPE, check = check_cmd_output, timeout = 900)
    except Exception as e:
        logger.error(e)
        raise

def get_file_content(path):
    if not os.path.isfile(path):
        return None
    with open(path, 'r') as f:
        return f.read().strip()

def get_board_name():
    return get_file_content('/sys/devices/virtual/dmi/id/board_name')

def get_platform():
    try:
        execResult = exec_command("awk ' /^PRODUCT\s/ { print $2 }' /usr/share/product-defs/product.conf")
        platform = execResult.stdout.strip().upper()
        return platform if platform in ['COS', 'APOLLO_PLATFORM'] else None
    except: #can't detect version
            return None


def get_current_bios_version():
    with open('/sys/devices/virtual/dmi/id/bios_version', 'r') as f:
        return f.read().strip()

def get_pmu_name(check_cmd_output = True):
    try:
        execOutput = get_file_content('cat /sys/devices/cpu/caps/pmu_name')
        if execOutput == "haswell":
            return True
    except:
        logger.info("Cannot read /sys/devices/cpu/caps/pmu_name")
    return False

#bios version string example: SE5C610.86B.01.01.0016.C5.033120161139
def compare_bios_versions(currentVersion, newVersion):
    # use-case of servers production by Operations Team
    # through this file we pass info from "ostree-production" step to next Linux boot step
    force_update_path = '/var/lib/harmonic-bios-autoupdate/production_force_update'
    if os.path.exists(force_update_path):
        logger.info("Force BIOS upgrade/downgrade without version check (PRODUCTION)")
        os.remove(force_update_path)
        return 1


    # implemented to allow correct comparison between fields that mix letters and digits
    def lower_than(left, right):
        try:
            int(left)
            int(right)
            return int(left) < int(right)
        except ValueError:
            [left_str, left_num] = re.split(r'(^[^\d]+)([\d]+)', left)[1:-1]
            [right_str, right_num] = re.split(r'(^[^\d]+)([\d]+)', right)[1:-1]
            return (left_str < right_str) or (left_str == right_str and int(left_num) < int(right_num))


    currentVersionArray = currentVersion.split('.')[2:-1]
    newVersionArray = newVersion.split('.')[2:-1]

    # Update official to custom version case
    # In case both numbers are identical, default update to custom
    # Otherwise, decide based on comparison result
    rc = 1 if len(currentVersionArray) < len(newVersionArray) else 0
    
    if len(currentVersionArray) <= len(newVersionArray):
        for i in range(min(len(currentVersionArray),len(newVersionArray))):
            if lower_than(currentVersionArray[i], newVersionArray[i]):
                rc = 1
                break
            elif lower_than(newVersionArray[i], currentVersionArray[i]):
                rc = -1
                break
    else:
        #We should not update from a custom version to an official version
        rc = None
    logger.debug("Versions compare result: "+str(rc))
    return rc


def is_update_available(boardName, platform):
    config.preinstallSummary["board"] = boardName
    config.preinstallSummary["platform"] = platform
    
    if boardName is None or platform is None:
        logger.error("Board and/or platform not detected, aborting...")
        logger.error("Board:{}, platform:{}".format(str(boardName), str(platform)))
        return False

    # This change will block bios auto-update on Haswell
    if platform == "APOLLO_PLATFORM" and get_pmu_name():
        logger.info("Bios auto-update is not supported")
        return False

    logger.debug("Searching for board in inventory...")
    for board in config.BIOSVersionsDict.keys():
        # Board name may have additional "R" in model, meaning a refreshed version of the same board model.
        # In such case, they share the same BIOS version
        if boardName in board:
            config.preinstallSummary["matched_board"] = board
            installationCandidate = config.BIOSVersionsDict[board].get(platform)
            if installationCandidate is not None:
                config.preinstallSummary["candidate_version"] = installationCandidate
                logger.info("board:{}, platform:{}, installation candidate:{}".format(boardName, platform, installationCandidate))
                config.matchedBoardName = board
                logger.debug("matched board:{}".format(board))
                try:
                    compBiosVerRc = compare_bios_versions(get_current_bios_version(), installationCandidate)
                    if compBiosVerRc == 1:
                        config.preinstallSummary["update_available"] = True
                        logger.info("BIOS update is available for current machine")
                        return True
                    elif compBiosVerRc == 0:
                        logger.info("BIOS version on ISO is identical to currently installed version")
                        return False
                    elif compBiosVerRc == -1:
                        logger.info("BIOS version on ISO is older than currently installed version")
                        return False
                except:
                    logger.error("An unknown error occurred while trying to compare versions")
                    return False
            else:
                logger.info("board + platform combination not supported")
                return False
    logger.error("Unknown board")
    return False


def get_current_rootfs():
    return exec_command('findmnt / -o LABEL --noheadings').stdout.strip()


# Code taken from udev package
def is_chrooted():
    res1 = exec_command_no_logging("stat -c %d/%i /").stdout
    res2 = exec_command_no_logging("stat -Lc %d/%i /proc/1/root 2>/dev/null").stdout
    if res1 == res2:
        # the devicenumber/inode pair of / is the same as that of /sbin/init's
        # root, so we're *not* in a chroot and hence return false.
        return False
    return True


def set_update_service_mode(mode):
    if mode not in ["enable", "disable"]:
        logger.error("unknown mode")
        return False

    logger.info("{} harmonic-bios-autoupdate service".format((mode[:-1]+"ing")).capitalize())
    try:
        exec_command("systemctl {} harmonic-bios-autoupdate".format(mode))
        return True
    except:
        return False


class MyLogger:
    def __init__(self):
        global logDirPath
        global logName
        global logger
        self.logFileLocation = os.path.join(logDirPath, logName)   # Path to log file

        if not os.path.exists(logDirPath):
            os.makedirs(logDirPath)

        logger = logging.getLogger()
        logger.setLevel(logging.DEBUG)

        consoleHandler = logging.StreamHandler()
        consoleFormatter = logging.Formatter('%(levelname)-8s %(message)s')
        consoleHandler.setFormatter(consoleFormatter)
        consoleHandler.setLevel(logging.ERROR)

        fileHandler = logging.handlers.RotatingFileHandler(self.logFileLocation, backupCount=1, maxBytes=2*1024**2) #logsize 2MB
        fileFormatter = logging.Formatter('%(asctime)s %(levelname)-8s %(message)s')
        fileHandler.setFormatter(fileFormatter)
        fileHandler.setLevel(logging.DEBUG)

        # add the handlers to the logger
        logger.addHandler(fileHandler)
        logger.addHandler(consoleHandler)

    def set_console_log_level(new_log_level):
        for handler in logger.handlers:
            if isinstance(handler, logging.StreamHandler):
                handler.setLevel(new_log_level)
