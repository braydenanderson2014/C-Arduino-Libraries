# Host Arduino Simulation Tests

This test platform runs selected Arduino-style library behavior on a Linux host in GitHub Actions.

## What it validates

- Header-level compilation of core libraries used by the test runner
- Dynamic structure behavior for ArrayList and Hashtable
- SDList behavior in memory mode
- File-backed SDList behavior in host mode (file creation, write, readback, header verification)
- Peak process heap usage against a configurable byte limit

## Files

- `include/Arduino.h`: minimal Arduino compatibility shim
- `include/SD.h`: file-backed SD mock using the local filesystem
- `include/LittleFS.h`: LittleFS alias backed by the same host file mock
- `include/SPI.h`: minimal SPI placeholder
- `host_arduino_sim_tests.cpp`: executable test runner and memory tracker

## Run locally

From repository root:

```bash
g++ -std=c++17 -O2 \
  -DAL_NO_SERIAL \
  -Itest/host_arduino_sim/include \
  -Ilib/ArrayList/src \
  -Ilib/Hashtable/src \
  -Ilib/SimpleVector/src \
  -Ilib/SDList/src \
  -Ilib/TypeTraits/src \
  test/host_arduino_sim/host_arduino_sim_tests.cpp \
  -o test/host_arduino_sim/out/bin/host_arduino_sim

HOST_MEM_LIMIT_BYTES=8388608 \
HOST_SIM_REPORT=test/host_arduino_sim/out/report-local.json \
HOST_SIM_FS_ROOT=test/host_arduino_sim/out/fs-local \
./test/host_arduino_sim/out/bin/host_arduino_sim
```

## Configure memory budget

Set `HOST_MEM_LIMIT_BYTES` to a numeric byte limit.

Examples:

- `4194304` for 4 MB
- `8388608` for 8 MB
- `16777216` for 16 MB

If omitted, the runner defaults to 8 MB.
