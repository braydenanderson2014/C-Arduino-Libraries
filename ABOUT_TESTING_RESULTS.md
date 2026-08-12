# Host Simulation Testing Report

## Summary

- Total runs: 0
- Passed runs: 0
- Failed runs: 0
- Max peak bytes: 0
- Avg peak bytes: 0
- Compile smoke objects found: 2
- Experimental compile results found: 8
- Experimental compile successes: 4
- Experimental compile failures: 4
- Experimental libraries covered: 7
- Experimental backends covered: 2
- Experimental optional modes covered: 3
- Memory profile runs: 0
- Runs that exceeded limit: 0
- Runs with limit enforcement enabled: 0
- Runs with capacity probe enabled: 0
- Runs where capacity probe reached limit: 0
- Runs with first limit-crossing test identified: 0
- Expected compile smoke objects: 0
- Expected experimental compile results: 6
- Stress test runs loaded: 0

## Understanding

- This report only summarizes artifacts downloaded into this workflow run.
- RSS means resident set size: physical memory pages currently resident for the process.
- PeakBytes is the process-level peak resident memory (high-water RSS) for the full run.
- BeforeRSS/AfterRSS are per-test resident-memory snapshots; DeltaRSS is AfterRSS minus BeforeRSS.
- BeforeHeap/AfterHeap are allocator-managed heap snapshots; DeltaHeap is AfterHeap minus BeforeHeap.
- PeakAfterTest is the process peak resident memory observed after a specific test completed.
- LimitBytes is the configured memory threshold for the run.
- LimitExceeded means measured memory crossed LimitBytes.
- LimitEnforced means crossing LimitBytes should mark that run as failed/gated.
- Experimental compile failures are reported separately and do not gate the main host simulation lanes.
- Experimental compile metrics track each library/backend/optional combination discovered in the experimental lane.
- ProbeElementsAtStop and ProbeCurrentBytesAtStop come from the optional capacity probe.
- FirstLimitCrossingTest is the first test whose per-test memory reached or exceeded the run limit.
- If FirstLimitCrossingTest is blank, no per-test crossing was found (or no per-test stats were present).

## Memory Profile Coverage

| Run | MemoryProfileRun | ProbeEnabled | ProbeLimitReached | FirstLimitCrossingTest |
| --- | --- | --- | --- | --- |

## Run Results

| Run | Success | Backend | PeakBytes | LimitBytes | LimitExceeded | LimitEnforced | FirstLimitCrossingTest | CrossingPeakBytes | ProbeElementsAtStop | ProbeCurrentBytesAtStop |
| --- | --- | --- | ---: | ---: | --- | --- | --- | ---: | ---: | ---: |

## Experimental Compile Results

_Each row is one experimental compile matrix entry (library + backend + optional mode)._

| Library | Backend | Optional | Success | ReturnCode | Artifact |
| --- | --- | --- | --- | ---: | --- |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector-sd-off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/DynamicStorageLibrary | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_DynamicStorageLibrary/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/EthernetConnectionChecker | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetConnectionChecker/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/EthernetMach2ConnectionChecker | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetMach2ConnectionChecker/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/Hashtable | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/Properties | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_Properties/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleVector | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector/sd/off |
