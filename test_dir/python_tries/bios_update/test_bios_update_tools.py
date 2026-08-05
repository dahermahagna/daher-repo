#!/usr/bin/python3

import json
import logging
import os
import sys
import types
import tempfile
import unittest
from unittest import mock

import config as local_config

package = types.ModuleType("harmonic_bios_autoupdate")
package.config = local_config
sys.modules.setdefault("harmonic_bios_autoupdate", package)
sys.modules.setdefault("harmonic_bios_autoupdate.config", local_config)

import bios_update_tools as tools

tools.logger = logging.getLogger("bios_update_tools_tests")
tools.logger.addHandler(logging.NullHandler())


class TestUpgradePlanBuilder(unittest.TestCase):

    def _base_plan(self):
        return {
            "board_name": "M50FCP2SBSTD",
            "platform": "APOLLO_PLATFORM",
            "from_version": "SE5C741.86B.01.01.0005",
            "to_version": "SE5C741.86B.01.02.0001",
            "payload_path": "/opt/harmonic/bios-update/intel/M50FCP2SBSTD_R01020001.zip",
            "actions": [
                {
                    "action_id": "action_001",
                    "command": "update_component --component cpld --version V3P0 --reset none",
                    "reset_after_action": False,
                },
                {
                    "action_id": "action_002",
                    "command": "update_component --component bmc --version 1.81 --reset system_reset",
                    "reset_after_action": True,
                },
            ],
        }

    def _manager(self, state_path):
        return tools.UpgradePlanManager(state_file_path=state_path)

    # ------------------------------------------------------------------ load / merge

    def test_load_plan_initialises_default_state(self):
        with tempfile.TemporaryDirectory() as d:
            plan_path = os.path.join(d, "plan.json")
            state_path = os.path.join(d, "state.json")
            with open(plan_path, "w") as f:
                json.dump(self._base_plan(), f)

            merged = self._manager(state_path).load_upgrade_plan_with_state(plan_path, state_path)

            self.assertIsNotNone(merged)
            self.assertEqual(merged["board_name"], "M50FCP2SBSTD")
            self.assertEqual(merged["platform"], "APOLLO_PLATFORM")
            self.assertEqual(merged["from_version"], "SE5C741.86B.01.01.0005")
            self.assertEqual(merged["to_version"], "SE5C741.86B.01.02.0001")
            self.assertEqual(len(merged["actions"]), 2)
            self.assertEqual(merged["state"], "pending")
            self.assertEqual(merged["current_action_index"], 0)
            self.assertEqual(merged["current_action_id"], "action_001")
            self.assertFalse(os.path.exists(state_path))

    def test_fail_fast_on_invalid_base_plan_action_schema(self):
        base_plan = self._base_plan()
        del base_plan["actions"][0]["command"]

        with self.assertRaises(tools.PlanValidationError) as ctx:
            tools.UpgradePlanManager().validate_base_upgrade_plan(base_plan)

        self.assertEqual(ctx.exception.code, "MISSING_BASE_ACTION_FIELD")

    # ------------------------------------------------------------------ state persistence

    def test_runtime_state_persists_without_modifying_base_plan(self):
        with tempfile.TemporaryDirectory() as d:
            plan_path = os.path.join(d, "plan.json")
            state_path = os.path.join(d, "state.json")
            base_plan = self._base_plan()
            with open(plan_path, "w") as f:
                json.dump(base_plan, f)

            mgr = self._manager(state_path)
            merged = mgr.load_upgrade_plan_with_state(plan_path, state_path)
            merged["state"] = "in_progress"
            merged["current_action_index"] = 1
            merged["current_action_id"] = "action_002"
            merged["last_completed_action_id"] = "action_001"
            merged["actions"][0]["status"] = "completed"
            merged["actions"][0]["attempts"] = 1

            self.assertTrue(mgr.save_upgrade_state(merged, state_path))

            reloaded = mgr.load_upgrade_plan_with_state(plan_path, state_path)
            self.assertEqual(reloaded["board_name"], "M50FCP2SBSTD")
            self.assertEqual(reloaded["platform"], "APOLLO_PLATFORM")
            self.assertEqual(reloaded["state"], "in_progress")
            self.assertEqual(reloaded["current_action_index"], 1)
            self.assertEqual(reloaded["current_action_id"], "action_002")
            self.assertEqual(reloaded["last_completed_action_id"], "action_001")
            self.assertEqual(reloaded["actions"][0]["status"], "completed")
            self.assertEqual(reloaded["actions"][0]["attempts"], 1)

            with open(state_path) as f:
                saved_state = json.load(f)
            self.assertEqual(saved_state["runtime_file_version"], 1.0)
            self.assertEqual(saved_state["board_name"], "M50FCP2SBSTD")
            self.assertEqual(saved_state["platform"], "APOLLO_PLATFORM")

            with open(plan_path) as f:
                self.assertEqual(json.load(f), base_plan)

    # ------------------------------------------------------------------ execution

    def test_execute_next_action_runs_command_and_advances(self):
        with tempfile.TemporaryDirectory() as d:
            plan_path = os.path.join(d, "plan.json")
            state_path = os.path.join(d, "state.json")
            base_plan = self._base_plan()
            base_plan["actions"][0]["command"] = "true"
            with open(plan_path, "w") as f:
                json.dump(base_plan, f)

            mgr = self._manager(state_path)
            plan = mgr.load_upgrade_plan_with_state(plan_path, state_path)
            updated = mgr.execute_next_action(plan)

            self.assertIsNotNone(updated)
            self.assertEqual(updated["actions"][0]["status"], "completed")
            self.assertEqual(updated["state"], "in_progress")
            self.assertEqual(updated["current_action_index"], 1)
            self.assertEqual(updated["current_action_id"], "action_002")

    def test_execute_next_action_marks_failed_on_retry_exhaustion(self):
        with tempfile.TemporaryDirectory() as d:
            plan_path = os.path.join(d, "plan.json")
            state_path = os.path.join(d, "state.json")
            base_plan = self._base_plan()
            base_plan["actions"][0]["command"] = "false"
            with open(plan_path, "w") as f:
                json.dump(base_plan, f)

            mgr = self._manager(state_path)
            plan = mgr.load_upgrade_plan_with_state(plan_path, state_path)
            plan["max_attempts_per_action"] = 1

            updated = mgr.execute_next_action(plan)

            self.assertIsNotNone(updated)
            self.assertEqual(updated["state"], "failed")
            self.assertEqual(updated["actions"][0]["status"], "failed")
            self.assertIn("returned non-zero exit status", updated["last_error"])

    # ------------------------------------------------------------------ plan selection

    def test_select_plan_matches_current_bios_version(self):
        with tempfile.TemporaryDirectory() as d:
            plan_a = {**self._base_plan(), "from_version": "SE5C741.86B.01.00.0005",
                      "to_version": "SE5C741.86B.01.01.0008", "payload_path": "/fw/a.zip",
                      "actions": [{"action_id": "action_001", "command": "true"}]}
            plan_b = {**self._base_plan(), "from_version": "SE5C741.86B.01.01.0008",
                      "to_version": "SE5C741.86B.01.02.0001", "payload_path": "/fw/b.zip",
                      "actions": [{"action_id": "action_001", "command": "true"}]}

            with open(os.path.join(d, "Plan_A.json"), "w") as f:
                json.dump(plan_a, f)
            with open(os.path.join(d, "Plan_B.json"), "w") as f:
                json.dump(plan_b, f)

            selected = tools.select_transition_plan_for_bios_version(
                plans_dir=d,
                current_bios_version="SE5C741.86B.01.01.0008",
                current_board_name="M50FCP2SBSTD",
                current_platform="APOLLO_PLATFORM",
            )
            self.assertEqual(selected, os.path.join(d, "Plan_B.json"))

    def test_select_plan_raises_when_no_match(self):
        with tempfile.TemporaryDirectory() as d:
            plan = {**self._base_plan(), "from_version": "SE5C741.86B.01.00.0005",
                    "payload_path": "/fw/a.zip",
                    "actions": [{"action_id": "action_001", "command": "true"}]}
            with open(os.path.join(d, "Plan_A.json"), "w") as f:
                json.dump(plan, f)

            with self.assertRaises(tools.PlanValidationError) as ctx:
                tools.select_transition_plan_for_bios_version(
                    plans_dir=d,
                    current_bios_version="SE5C741.86B.99.99.9999",
                    current_board_name="M50FCP2SBSTD",
                    current_platform="APOLLO_PLATFORM",
                )
            self.assertEqual(ctx.exception.code, "NO_MATCHING_PLAN")

    def test_select_plan_raises_when_ambiguous(self):
        with tempfile.TemporaryDirectory() as d:
            plan = {**self._base_plan(), "from_version": "SE5C741.86B.01.01.0008",
                    "payload_path": "/fw/x.zip",
                    "actions": [{"action_id": "action_001", "command": "true"}]}
            with open(os.path.join(d, "Plan_1.json"), "w") as f:
                json.dump(plan, f)
            with open(os.path.join(d, "Plan_2.json"), "w") as f:
                json.dump(plan, f)

            with self.assertRaises(tools.PlanValidationError) as ctx:
                tools.select_transition_plan_for_bios_version(
                    plans_dir=d,
                    current_bios_version="SE5C741.86B.01.01.0008",
                    current_board_name="M50FCP2SBSTD",
                    current_platform="APOLLO_PLATFORM",
                )
            self.assertEqual(ctx.exception.code, "AMBIGUOUS_MATCHING_PLANS")

    # ------------------------------------------------------------------ pending plan detection

    def test_find_pending_plan_resumes_in_progress(self):
        with tempfile.TemporaryDirectory() as d:
            plan_path = os.path.join(d, "Plan_A.json")
            state_path = os.path.join(d, "state.json")
            base_plan = self._base_plan()
            with open(plan_path, "w") as f:
                json.dump(base_plan, f)

            state = tools.UpgradePlanManager().default_state_from_base_plan(base_plan)
            state["state"] = "in_progress"
            state["actions"][0]["status"] = "completed"
            with open(state_path, "w") as f:
                json.dump(state, f)

            # from_version/to_version are now set by default_state_from_base_plan
            self.assertEqual(state["board_name"], base_plan["board_name"])
            self.assertEqual(state["platform"], base_plan["platform"])
            self.assertEqual(state["from_version"], base_plan["from_version"])
            self.assertEqual(state["to_version"], base_plan["to_version"])
            self.assertEqual(tools.find_pending_plan_from_state(d, state_path), plan_path)

    def test_find_pending_plan_returns_none_when_no_state_file(self):
        with tempfile.TemporaryDirectory() as d:
            self.assertIsNone(
                tools.find_pending_plan_from_state(d, os.path.join(d, "missing.json")))

    def test_find_pending_plan_returns_none_when_state_completed(self):
        with tempfile.TemporaryDirectory() as d:
            plan_path = os.path.join(d, "Plan_A.json")
            state_path = os.path.join(d, "state.json")
            base_plan = self._base_plan()
            with open(plan_path, "w") as f:
                json.dump(base_plan, f)

            state = tools.UpgradePlanManager().default_state_from_base_plan(base_plan)
            state["state"] = "completed"
            with open(state_path, "w") as f:
                json.dump(state, f)

            self.assertIsNone(tools.find_pending_plan_from_state(d, state_path))

    # ------------------------------------------------------------------ payload extraction

    def test_extract_payload_fails_when_payload_missing(self):
        with tempfile.TemporaryDirectory() as d:
            plan = self._base_plan()
            plan["payload_path"] = os.path.join(d, "missing.zip")

            mgr = self._manager(os.path.join(d, "state.json"))
            self.assertFalse(mgr.extract_bios_files_from_plan_payload(plan))

    def test_extract_payload_uses_plan_payload_path(self):
        with tempfile.TemporaryDirectory() as d:
            payload_path = os.path.join(d, "payload.zip")
            with open(payload_path, "wb") as f:
                f.write(b"PK\x05\x06" + b"\x00" * 18)

            plan = self._base_plan()
            plan["payload_path"] = payload_path

            mgr = self._manager(os.path.join(d, "state.json"))
            with mock.patch.object(tools, "exec_command", return_value=object()), \
                 mock.patch.object(tools, "exec_command_no_logging", return_value=object()):
                self.assertTrue(
                    mgr.extract_bios_files_from_plan_payload(
                        plan,
                        current_board_name="M50FCP2SBSTD",
                    )
                )

    def test_daher(self):
        self.assertTrue(True)

# if __name__ == "__main__":
#     unittest.main()
