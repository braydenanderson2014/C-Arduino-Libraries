# Host Arduino Simulation Tests

This test platform runs selected Arduino-style library behavior on a Linux host in GitHub Actions.

## What it validates

- Header-level compilation of core libraries used by the test runner
- Dynamic structure behavior for ArrayList and Hashtable
- CustomString safety-critical string behavior
- Json path lookup, serialization, parsing, and file round-trips
- SDList behavior in memory mode
- File-backed SDList behavior in host mode (file creation, write, readback, header verification)
- Concurrency primitive fallback behavior for SimpleMutex, SimpleThreadManager,
  SimpleSemaphore, and SimpleRWLock
- Peak process heap usage collected as informational data (not a failure gate)

## What the stress tester measures (separate workflow)

The dedicated stress binary (`host_stress_tests.cpp`) runs in `host-sim-memory-profiles.yml`
on a weekly schedule. It measures capacity metrics against each board's SRAM budget scaled to
a host heap-delta limit (`sramBytes × 1024`) and now includes periodic data-health checks while
containers are being filled.

Probes currently run:

| Probe type | What is measured |
|---|---|
| Instance count | How many simultaneously-alive empty container instances fit within the budget |
| Element fill | How many elements fit in a single container instance before the budget is exhausted |
| Health/recovery metadata | Checkpointed invariants/checksums, post-limit recovery checks, and post-cleanup deltas |

Container and type combinations:
- Instance count: `ArrayList<int>`, `SimpleVector<int>`, `Hashtable<int,int>`, `JSON`, `AVLTree<int>`
- Element fill: `ArrayList<{int,float,double,String}>`, `SimpleVector<{int,float,double}>`,
  `Hashtable<{int→int,String→String}>`, `AVLTree<int>`

Results are written to `stress-{board}.json` and aggregated into
`reports/host-sim-stress-report.md` / `.json`.

The stress binary always exits with status 0 — hitting the budget produces a measurement,
not a failure.

## Files

- `include/Arduino.h`: minimal Arduino compatibility shim
- `include/SD.h`: file-backed SD mock using the local filesystem
- `include/LittleFS.h`: LittleFS alias backed by the same host file mock
- `include/SPI.h`: minimal SPI placeholder
- `host_arduino_sim_tests.cpp`: correctness test runner (pass/fail on assertions only)
- `host_json_mode_tests.cpp`: JSON-focused runner for optional-off and optional-on modes
- `host_stress_tests.cpp`: memory stress / capacity measurement runner
- `host_test_matrix.json`: per-library host compile matrix (backend/optional combinations)

## Config-driven host compile matrix

`test/host_arduino_sim/host_test_matrix.json` controls which compile-time host options are
tested for each discovered library in `arduino-host-sim-tests.yml`.

- `defaults.compile.backends` / `defaults.compile.optionalModes` apply to all libraries.
- `libraries.<path>.compile.backends` / `optionalModes` override defaults per library path.
- This allows skipping irrelevant permutations (for example, ArrayList with LittleFS/optional).

## Run the correctness tests locally

From repository root:

```bash
g++ -std=c++17 -O2 \
  -DAL_NO_SERIAL \
  -Itest/host_arduino_sim/include \
  -Ilib/ArrayList/src \
  -Ilib/AVLTree/src \
  -Ilib/Hashtable/src \
  -Ilib/Json/src \
  -Ilib/MathLib/src \
  -Ilib/SimpleVector/src \
  -Ilib/SDList/src \
  -Ilib/String/src \
  -Ilib/TypeTraits/src \
  lib/Json/src/JSON.cpp \
  test/host_arduino_sim/host_arduino_sim_tests.cpp \
  -o test/host_arduino_sim/out/bin/host_arduino_sim

HOST_SIM_REPORT=test/host_arduino_sim/out/report-local.json \
HOST_SIM_FS_ROOT=test/host_arduino_sim/out/fs-local \
./test/host_arduino_sim/out/bin/host_arduino_sim
```

## Run the stress tests locally

From repository root:

```bash
g++ -std=c++17 -O2 \
  -DAL_NO_SERIAL \
  -Itest/host_arduino_sim/include \
  -Ilib/ArrayList/src \
  -Ilib/AVLTree/src \
  -Ilib/Hashtable/src \
  -Ilib/Json/src \
  -Ilib/MathLib/src \
  -Ilib/SimpleVector/src \
  -Ilib/SDList/src \
  -Ilib/String/src \
  -Ilib/TypeTraits/src \
  lib/Json/src/JSON.cpp \
  test/host_arduino_sim/host_stress_tests.cpp \
  -o test/host_arduino_sim/out/bin/host_stress

# Example: simulate an Arduino Uno (2 KB SRAM × 1024 = 2 MB heap-delta budget)
HOST_MEM_LIMIT_BYTES=2097152 \
HOST_STRESS_BOARD=uno \
HOST_STRESS_SRAM_BYTES=2048 \
HOST_STRESS_REPORT=test/host_arduino_sim/out/stress-uno.json \
./test/host_arduino_sim/out/bin/host_stress
```

## Run JSON mode checks locally

From repository root:

```bash
g++ -std=c++17 -O2 \
  -DAL_NO_SERIAL \
  -Itest/host_arduino_sim/include \
  -Ilib/Json/src \
  -Ilib/SimpleVector/src \
  -Ilib/MathLib/src \
  -Ilib/String/src \
  -Ilib/TypeTraits/src \
  lib/Json/src/JSON.cpp \
  test/host_arduino_sim/host_json_mode_tests.cpp \
  -o test/host_arduino_sim/out/bin/json_mode_optional_off

g++ -std=c++17 -O2 \
  -DAL_NO_SERIAL \
  -DJSON_ENABLE_OPTIONAL_RETURNS=1 \
  -Itest/host_arduino_sim/include \
  -Ilib/Json/src \
  -Ilib/Optional/src \
  -Ilib/SimpleVector/src \
  -Ilib/MathLib/src \
  -Ilib/String/src \
  -Ilib/TypeTraits/src \
  lib/Json/src/JSON.cpp \
  test/host_arduino_sim/host_json_mode_tests.cpp \
  -o test/host_arduino_sim/out/bin/json_mode_optional_on

HOST_JSON_MODE_REPORT=test/host_arduino_sim/out/report-json-optional-off-local.json \
HOST_SIM_FS_ROOT=test/host_arduino_sim/out/fs-json-optional-off-local \
./test/host_arduino_sim/out/bin/json_mode_optional_off

HOST_JSON_MODE_REPORT=test/host_arduino_sim/out/report-json-optional-on-local.json \
HOST_SIM_FS_ROOT=test/host_arduino_sim/out/fs-json-optional-on-local \
./test/host_arduino_sim/out/bin/json_mode_optional_on
```

## Per-test memory stats output

The correctness runner writes a per-test memory stats JSON report.

- Default path: `test/host_arduino_sim/out/host-arduino-sim-stats.json`
- Override with env var: `HOST_SIM_STATS_REPORT`

Each entry includes:

- `beforeCurrentBytes`
- `afterCurrentBytes`
- `deltaCurrentBytes`
- `peakBytesAfterTest`
- `passed` and `error`

## Periodic board-profile stress checks

The workflow `Host Simulation Memory Profiles` in `.github/workflows/host-sim-memory-profiles.yml`
runs on a weekly schedule and manual dispatch, not on every push.

It builds `host_stress_tests.cpp` and runs it once per board profile.  The heap-delta
budget for each board is:

```
HOST_MEM_LIMIT_BYTES = sramBytes × 1024
```

Results are uploaded as `host-stress-report-{board}` artifacts and then consolidated into
`reports/host-sim-stress-report.md` / `.json`.
