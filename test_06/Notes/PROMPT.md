# Target Objective
Create a pre-compiled C dynamic library (.dll) and an accompanying Modelica package (`pkg_appsw.mo`) that interfaces with it. The software architecture should mimic an AUTOSAR Classic layout with discrete-time signals, event-based runnable scheduling, and global interface accessors.

Reference structure pattern: Inspired by `test_03` in `https://github.com/giampa95/tests_C2Modelica`.

---

## 1. Directory & File Organization
Organize the repository as follows:
- `pkg_appsw.mo` (Modelica package at root level)
- `Resources/`
  - `Build/` (Destination directory for compiled `.dll` outputs)
  - `src/` (or subfolder containing C sources and headers):
    - `datatypes.h` / `datatypes.c`: Standard type definitions.
    - `interfaces.h` / `interfaces.c`: Global variables and RTE-like read/write accessors.
    - `swc1_runnables.c` & `swc2_runnables.c`: Software components, internal state, and runnables.
    - `scheduler_manager.c`: Runnable stack/queue management (`pushRunnable`, `runRunnables`, `resetScheduler`).
    - `scheduler_events.c`: Event management functions (`Mng_*`).
    - `appsw.c`: Main DLL entry points (constructor, destructor, `doStep`).
  - `README.md`: Build and compilation instructions using both `gcc` and `make`.

---

## 2. C Software Architecture (AUTOSAR Classic-Inspired)

### A. Datatypes (`datatypes.c` / `datatypes.h`)
Define standard fixed-width integer types:
- `int32`: 32-bit signed integer.
- `uint8`: 8-bit unsigned integer.

### B. Global Interfaces & Accessors (`interfaces.c` / `interfaces.h`)
Declare the following global variables representing system interfaces:
- `int32 gSWC1_value`
- `int32 gSWC1_gain`
- `int32 gSWC1_valueGained`
- `uint8 gSWC2_counter`

Define read/write accessor functions mimicking AUTOSAR RTE ports (add a code comment explaining that these simplify real `Rte_Read_*` / `Rte_Write_*` primitives):
- `void Write_gSWC1_value(const int32 value)`
- `void Write_gSWC1_gain(const int32 value)`
- `void Write_gSWC1_valueGained(const int32 value)`
- `void Write_gSWC2_counter(const uint8 value)`
- `void Read_gSWC1_value(int32 *value)`
- `void Read_gSWC1_gain(int32 *value)`
- `void Read_gSWC1_valueGained(int32 *value)`
- `void Read_gSWC2_counter(uint8 *value)`

### C. Software Components (SWCs) & Runnables

#### SWC 1 (`swc1_runnables.c`)
Internal variables: `SWC1_value` (`int32`), `SWC1_gain` (`int32`), `SWC1_valueGained` (`int32`).
Runnables:
1. `SWC1_ComputeValueGained()`:
   - Reads `gSWC1_value` via `Read_gSWC1_value` into local `SWC1_value`.
   - Multiplies local `SWC1_value` by local `SWC1_gain` to calculate local `SWC1_valueGained`.
   - Writes result to `gSWC1_valueGained` via `Write_gSWC1_valueGained`.
2. `SWC1_SaveGain()`:
   - Reads global gain `gSWC1_gain` via `Read_gSWC1_gain` and stores it into local `SWC1_gain`.
3. `SWC1_ResetGain()`:
   - Resets local `SWC1_gain` to `1`.
   - Writes `1` to `gSWC1_gain` via `Write_gSWC1_gain`.

#### SWC 2 (`swc2_runnables.c`)
Internal variable: `SWC2_counter` (`uint8`).
Runnables:
1. `SWC2_Count()`:
   - Reads `gSWC2_counter` via `Read_gSWC2_counter` into local `SWC2_counter`.
   - Increments local `SWC2_counter` by `1`.
   - Writes updated value to `gSWC2_counter` via `Write_gSWC2_counter`.
2. `SWC2_ResetCounter()`:
   - Resets local `SWC2_counter` to `0`.
   - Writes `0` to `gSWC2_counter` via `Write_gSWC2_counter`.

*(Include comments noting where additional runnables can be inserted).*

### D. Event Scheduling & Priority (`scheduler_events.c` & `scheduler_manager.c`)
Define manager functions (`Mng_*`) that push associated runnables onto an execution stack/queue. (Add code comments noting that in real AUTOSAR implementations, runnables are dispatched by low-level architectural components/OS tasks).

Event Mappings:
- `Mng_eventClockA`: Pushes `SWC1_ComputeValueGained` and `SWC2_Count`.
- `Mng_eventG`: Pushes `SWC1_SaveGain`.
- `Mng_eventR`: Pushes `SWC1_ResetGain` and `SWC2_ResetCounter`.

Event Priority Order:
`eventR` > `eventG` > `eventClockA`

When multiple events trigger simultaneously, `Mng_*` functions must be scheduled onto the stack in order of priority (highest priority processed first).

---

## 3. Dynamic Library (DLL) Core Interface (`appsw.c`)

The C library is pre-compiled (no class namespace needed per application instance) and provides 3 core interface functions:

1. **Constructor (`appsw_init`)**:
   - Initializes internal states.
   - Calls `Mng_eventR` by default on initialization.
2. **Destructor (`appsw_free`)**:
   - Handles memory cleanup and teardown.
3. **Step Function (`doStep`)**:
   - Signature: `doStep(const int events[3], int inp_SWC1_value, int inp_SWC1_gain, int *outp_SWC1_valueGained, int *outp_SWC2_counter)`
   - Execution Sequence:
     1. Cast input double arguments to target C types and assign to global interface variables (`gSWC1_value`, `gSWC1_gain`).
     2. Process `events` vector (array/bitmask representing `[eventR, eventG, eventClockA]`).
     3. Invoke `Mng_*` functions according to priority rules to populate the runnable queue.
     4. Execute `runRunnables()` to flush and run all queued tasks.
     5. Cast outputs (`gSWC1_valueGained`, `gSWC2_counter`) and write to output pointers.

---

## 4. Modelica Package Specification (`pkg_appsw.mo`)

Define a Modelica package containing three components:

1. `class_appsw` (Modelica Class for External Object):
   - Encapsulates dynamic library references (`.dll`).
   - Declares external C functions: constructor, destructor, and an internal `step` method (do not declare `step` as an external standalone function outside the class).
   - `step` accepts an event indicator vector/flags and time parameter.

2. `sys_appsw` (Modelica Block Model):
   - Wraps `class_appsw`.
   - Uses discrete-time signals equipped with Modelica's `discrete` keyword.
   - Uses `when` clauses to detect incoming events and construct an event flag collection (e.g., array `[eventR, eventG, eventClockA]`) capable of handling simultaneous event triggers in the same time step.

3. `tb_sys_appsw` (Modelica Testbench):
   - Instantiates `sys_appsw` block model.
   - Provides stimulus inputs (e.g., discrete step inputs, event trigger clocks) to test combined and simultaneous event handling.