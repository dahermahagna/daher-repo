# BIOS Upgrade Orchestrator - Software Requirements Document (SRD)

## 1. Purpose and Scope
1. Load and execute one BIOS transition plan at a time.
2. Resume safely after reboot or interruption by persisting runtime state to disk.
3. Select the correct plan automatically when multiple transition plans are available by matching BIOS version, board name, and platform.
4. Validate plan and state data before execution.
5. Keep plan generation, payload packaging, and firmware command implementation out of scope for this SRD.

## 2. System Flow and Core Requirements
1. If transition plan files exist, run the plan-based flow; otherwise use the legacy direct-update flow.
2. If runtime state is `pending` or `in_progress`, resume that plan; otherwise select one unique plan matching BIOS version, board name, and platform.
3. Load and validate base plan and runtime state, reinitialize invalid or plan-mismatched state, and merge state into plan without changing the base plan file.
4. Extract payload files before action execution and fail fast if payload is missing.
5. Execute pending actions in a cycle until a stop condition is reached (`reset_after_action`, failure, or completion), then persist runtime state after that cycle outcome.
6. On success, mark action `done` and advance; on `reset_after_action`, stop cycle for reboot; if no actions remain, mark plan `completed`.
7. On failure, increment attempts and set `last_error`; retry until `max_attempts_per_action`, then mark action and plan `failed`.
8. Persist runtime metadata including action index/id, last completed action id, state, error, board/platform, and runtime file version.

## 3. Data Model

### 3.1 Base Transition Plan
The executor consumes a single-transition plan with this shape:

```json
{
  "from_version": "SE5C741.86B.01.01.0008",
  "to_version": "SE5C741.86B.01.02.0001",
  "board_name": "M50FCP2SBSTD",
  "platform": "APOLLO_PLATFORM",
  "payload_path": "/opt/harmonic/bios-update/intel/M50FCP2SBSTD_APOLLO_PLATFORM_01020001.zip",
  "max_attempts_per_action": 2,
  "actions": [
    {
      "action_id": "action_001",
      "command": "update_component --component cpld --version V3P0 --reset none",
      "reset_after_action": false
    }
  ]
}
```

Required fields are `board_name`, `platform`, `from_version`, `to_version`, `payload_path`, and `actions`. `max_attempts_per_action` is optional and defaults to 2 when absent. Each action must include `action_id` and `command`; `reset_after_action` is optional and must be boolean when present.

### 3.2 Runtime State
The executor persists mutable progress separately from the base plan:

```json
{
  "runtime_file_version": 1.0,
  "from_version": "SE5C741.86B.01.01.0008",
  "to_version": "SE5C741.86B.01.02.0001",
  "board_name": "M50FCP2SBSTD",
  "platform": "APOLLO_PLATFORM",
  "max_attempts_per_action": 2,
  "current_action_index": 0,
  "current_action_id": "action_001",
  "last_completed_action_id": null,
  "state": "pending",
  "last_error": null,
  "actions": [
    {
      "action_id": "action_001",
      "status": "pending",
      "attempts": 0
    }
  ]
}
```

Runtime state must contain the same number of actions as the base plan. `runtime_file_version` must match the executor's expected state version. Runtime state must also match the selected base plan identity (`from_version`, `to_version`, `board_name`, `platform`) and action IDs by position; otherwise, state is reinitialized from the selected base plan. Valid action statuses are `pending`, `in_progress`, `done`, and `failed`. Valid plan states are `pending`, `in_progress`, `completed`, and `failed`.

### 3.3 State File Location
1. The default state path is `/var/lib/harmonic-bios-autoupdate/upgrade_plan_state.json`.
2. The executor writes to the configured state path and creates parent directories as needed.
3. If writing fails, state persistence fails for that cycle.

## 4. Use Cases

### UC-01: First-Time Plan Load
**Trigger:** No runtime state file exists for the selected transition.
**Steps:**
1. Load the selected base plan.
2. Validate the base plan schema.
3. Build the default runtime state from the base plan.
4. Extract the BIOS payload into `/tmp/intel_bios` and adjust `flashupdt.cfg` when needed.
5. Merge the state into the plan.
6. Execute the first pending action.

### UC-02: Resume After Interruption
**Trigger:** A runtime state file exists and its `state` is `pending` or `in_progress`.
**Steps:**
1. Load the runtime state file.
2. Match the state to the unique transition plan with the same `from_version`, `to_version`, `board_name`, and `platform`.
3. Validate the state and verify compatibility with the selected plan (transition identity and action IDs).
4. Extract the BIOS payload before action execution.
5. Merge state into the plan.
6. Resume execution from the first non-`done` action.

### UC-03: Execute Actions in a Cycle
**Trigger:** At least one non-`done` action is available.
**Steps:**
1. Set `current_action_index` and `current_action_id` to the pending action.
2. Run the action command.
3. On success, mark the action `done`, update `last_completed_action_id`, and continue to the next pending action.
4. If the action has `reset_after_action = true`, stop the cycle so the caller can persist state and reboot.
5. If any action fails, apply retry/failure handling and stop the cycle.
6. If no actions remain, mark the plan `completed`.

### UC-04: Handle Action Failure
**Trigger:** An action command exits non-zero or raises an execution error.
**Steps:**
1. Increment the action's `attempts` counter.
2. Store the error message in `last_error`.
3. If attempts are below `max_attempts_per_action`, return the action to `pending`.
4. If attempts reach the limit, mark the action `failed` and the plan `failed`.

### UC-05: Select a Matching Plan
**Trigger:** No resumable state exists.
**Steps:**
1. Scan the transition plan directory for valid JSON plan files.
2. Select the unique plan whose `from_version`, `board_name`, and `platform` match the current BIOS environment.
3. If multiple plans match, raise an ambiguous-plan error.
4. If no plans match, report that no transition is required.

### UC-06: Legacy Direct Update Path
**Trigger:** No transition plan files are present.
**Steps:**
1. Use the existing direct-update flow.
2. Execute the older BIOS update logic outside the plan executor.

## 5. Software Quality Attributes

### 5.1 Reusability
- The executor works with any plan that follows the base transition schema.
- Action commands are opaque to the executor, so the same flow can support different firmware steps.
- Validation logic is reusable across plan files and runtime state files.

### 5.2 Debuggability
- `PlanValidationError.code` provides deterministic error classification.
- The persisted state exposes `current_action_index`, `attempts`, `state`, `last_error`, `board_name`, and `platform` for post-failure analysis.
- Reloading a saved state should reproduce the same next action.

### 5.3 Testability
- Base-plan validation, runtime-state validation, state merge, retry exhaustion, and resume behavior are deterministic.
- The executor can be tested with temporary plan and state files.
- State persistence is isolated from plan loading, making it easy to unit-test each layer independently.

### 5.4 Recoverability
- Progress survives reboot because the current state is written to disk after each execution cycle outcome.
- If a saved state is invalid or incompatible with the selected plan identity/actions, the executor reinitializes the state for that plan.
- State files are versioned through `runtime_file_version` so incompatible persisted formats can be rejected cleanly.
- Retry counters are persisted, so failure limits survive interruption.

### 5.5 Survivability
- The orchestrator only advances when the current action is explicitly marked successful.
- A failed action cannot be skipped without modifying the plan or the persisted state.
- A reboot-requesting action stops the current cycle after the action succeeds so the next boot can resume safely.
- The launcher can resume from the last known action after a reboot or service restart.

### 5.6 Required Unit Tests
1. Valid base plan schema is accepted and invalid schema is rejected.
2. Runtime state merges correctly without mutating the base plan file.
3. Successful action execution advances the current action index.
4. Retry exhaustion marks the action and plan as failed.
5. Resume logic selects the first pending action from saved state.
6. Plan selection by current BIOS version, board, and platform returns the unique matching plan.
7. Payload extraction succeeds for a valid plan payload path and fails when the payload is missing.

## 6. Compliance and Compatibility Considerations

### 6.1 Backward Compatibility
- The plan executor does not change the BIOS or node-to-node control protocol.
- The plan-based flow is local to the node and does not require external orchestration changes.
- New plan schemas should be versioned if additional required fields are introduced later.

### 6.2 ISSU Impact
- BIOS updates are not treated as in-service software updates.
- Reboot behavior makes the flow unsuitable for rolling ISSU-style updates.
- Operators should schedule BIOS transitions in a maintenance window.

### 6.3 Third-Party IP and Licensing
- BIOS payloads and vendor tools remain subject to their own licensing and redistribution terms.
- The orchestrator itself does not embed vendor firmware binaries.
- Release packaging should continue to track external tools referenced by action commands.

## 7. Resource Considerations

### 7.1 Scalability
- Runtime complexity scales linearly with the number of actions in a plan.
- The executor manages one node and one active transition at a time.

### 7.2 CPU Utilization
- CPU use is dominated by command execution and JSON I/O.
- The orchestrator does not rely on background polling loops.

### 7.3 Memory Utilization
- The in-memory representation is small because it stores plan metadata and per-action state only.
- Large firmware payloads are referenced by path, not loaded into memory by the executor.

### 7.4 Real-Time Considerations
- The system has no hard real-time requirements.
- Reboot waits and firmware command durations are bounded by the underlying tools and hardware.

## 8. Risks and Mitigation

### 8.1 Invalid Plan Commands
- **Risk:** An action command is malformed or references a missing tool.
- **Mitigation:** Validate that `command` is a non-empty string before execution and keep error text in `last_error`.

### 8.2 Stale Runtime State
- **Risk:** A saved state belongs to a different plan or action count.
- **Mitigation:** Validate state against the selected plan and reinitialize when the shapes do not match.

### 8.3 Retry Exhaustion
- **Risk:** A failing action consumes the retry budget and stops the transition.
- **Mitigation:** Persist the failure state so operators can inspect the cause and decide whether to re-run or abort.

### 8.4 Ambiguous Plan Selection
- **Risk:** Multiple plans match the same `from_version`.
- **Mitigation:** Treat ambiguity as an error and require the plan set to be disambiguated.

### 8.5 Hardware-Specific Post-Checks
- **Risk:** A command can succeed while the hardware state still needs verification.
- **Mitigation:** Encode any required verification as part of the action command or as a later action.

---

*SRD Version: 2.1 (Plan-Based Execution)*
*Last Updated: May 19, 2026*