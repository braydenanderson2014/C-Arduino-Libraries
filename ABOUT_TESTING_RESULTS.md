# Host Simulation Testing Report

## Summary

- Total runs: 2
- Passed runs: 2
- Failed runs: 0
- Max peak bytes: 4079616
- Avg peak bytes: 4036608
- Compile smoke objects found: 62
- Experimental compile results found: 26
- Experimental compile successes: 26
- Experimental compile failures: 0
- Experimental libraries covered: 14
- Experimental backends covered: 2
- Experimental optional modes covered: 14
- Memory profile runs: 0
- Runs that exceeded limit: 0
- Runs with limit enforcement enabled: 2
- Runs with capacity probe enabled: 0
- Runs where capacity probe reached limit: 0
- Runs with first limit-crossing test identified: 0
- Expected compile smoke objects: 49
- Expected experimental compile results: 13
- Stress test runs loaded: 10

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
| optional-sd | no | no | no |  |
| sd | no | no | no |  |

## Run Results

| Run | Success | Backend | PeakBytes | LimitBytes | LimitExceeded | LimitEnforced | FirstLimitCrossingTest | CrossingPeakBytes | ProbeElementsAtStop | ProbeCurrentBytesAtStop |
| --- | --- | --- | ---: | ---: | --- | --- | --- | ---: | ---: | ---: |
| optional-sd | yes | SD | 3993600 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4079616 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3624960 | 3624960 | 0 | 77040 | 77312 | 272 | 3538944 |  |
| testHashtableBasicBehavior | yes | 3624960 | 3633152 | 8192 | 77456 | 79648 | 2192 | 3538944 |  |
| testSDListMemoryMode | yes | 3633152 | 3633152 | 0 | 79648 | 79744 | 96 | 3538944 |  |
| testSDListFileIOMode | yes | 3633152 | 3825664 | 192512 | 79744 | 80848 | 1104 | 3731456 |  |
| testCustomStringBehavior | yes | 3825664 | 3825664 | 0 | 80848 | 80848 | 0 | 3731456 |  |
| testJSONRoundTrip | yes | 3825664 | 3960832 | 135168 | 80848 | 82320 | 1472 | 3862528 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3960832 | 3964928 | 4096 | 82320 | 85456 | 3136 | 3862528 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3964928 | 4071424 | 106496 | 85456 | 87120 | 1664 | 3993600 |  |
| testSimpleVectorLifecycleStress | yes | 4071424 | 4071424 | 0 | 87120 | 87120 | 0 | 3993600 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4071424 | 4071424 | 0 | 89184 | 89184 | 0 | 3993600 |  |
| testJSONFileRoundTrip | yes | 4071424 | 4071424 | 0 | 89184 | 89664 | 480 | 3993600 |  |
| testSDListPersistenceStress | yes | 4071424 | 4071424 | 0 | 89616 | 89680 | 64 | 3993600 |  |
| testJSONPersistenceStress | yes | 4071424 | 4071424 | 0 | 89680 | 90384 | 704 | 3993600 |  |
| testAVLTreeBasicBehavior | yes | 4071424 | 4071424 | 0 | 90384 | 90544 | 160 | 3993600 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4071424 | 4071424 | 0 | 90544 | 90784 | 240 | 3993600 |  |
| testPredicatesBasicBehavior | yes | 4071424 | 4071424 | 0 | 90784 | 90784 | 0 | 3993600 |  |
| testOperatorsBasicBehavior | yes | 4071424 | 4071424 | 0 | 90784 | 90784 | 0 | 3993600 |  |
| testDynamicStorageLibraryRamMode | yes | 4071424 | 4071424 | 0 | 92832 | 92832 | 0 | 3993600 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4071424 | 4071424 | 0 | 92832 | 92832 | 0 | 3993600 |  |
| testSimpleMutexFallbackBehavior | yes | 4071424 | 4141056 | 69632 | 92832 | 92832 | 0 | 3993600 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4141056 | 4141056 | 0 | 92832 | 92832 | 0 | 3993600 |  |
| testSimpleRWLockFallbackBehavior | yes | 4141056 | 4141056 | 0 | 92832 | 92976 | 144 | 3993600 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4141056 | 4141056 | 0 | 92976 | 93088 | 112 | 3993600 |  |
| testAVLTreeErrorCodes | yes | 4141056 | 4141056 | 0 | 93088 | 93136 | 48 | 3993600 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3641344 | 3641344 | 0 | 76928 | 77200 | 272 | 3493888 |  |
| testHashtableBasicBehavior | yes | 3641344 | 3649536 | 8192 | 77344 | 79536 | 2192 | 3493888 |  |
| testSDListMemoryMode | yes | 3649536 | 3649536 | 0 | 79536 | 79632 | 96 | 3493888 |  |
| testSDListFileIOMode | yes | 3649536 | 3842048 | 192512 | 79632 | 80752 | 1120 | 3686400 |  |
| testCustomStringBehavior | yes | 3842048 | 3842048 | 0 | 80752 | 80752 | 0 | 3686400 |  |
| testJSONRoundTrip | yes | 3842048 | 3977216 | 135168 | 80752 | 82272 | 1520 | 3817472 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3977216 | 3981312 | 4096 | 82272 | 85312 | 3040 | 3817472 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3981312 | 4087808 | 106496 | 85312 | 86832 | 1520 | 3948544 |  |
| testSimpleVectorLifecycleStress | yes | 4087808 | 4087808 | 0 | 86832 | 86832 | 0 | 3948544 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4087808 | 4087808 | 0 | 88896 | 88896 | 0 | 3948544 |  |
| testJSONFileRoundTrip | yes | 4087808 | 4087808 | 0 | 88896 | 89152 | 256 | 3948544 |  |
| testSDListPersistenceStress | yes | 4087808 | 4087808 | 0 | 89152 | 89504 | 352 | 3948544 |  |
| testJSONPersistenceStress | yes | 4087808 | 4087808 | 0 | 89504 | 90320 | 816 | 3948544 |  |
| testAVLTreeBasicBehavior | yes | 4087808 | 4087808 | 0 | 90320 | 90368 | 48 | 3948544 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4087808 | 4087808 | 0 | 90368 | 90464 | 96 | 3948544 |  |
| testPredicatesBasicBehavior | yes | 4087808 | 4087808 | 0 | 90464 | 90464 | 0 | 3948544 |  |
| testOperatorsBasicBehavior | yes | 4087808 | 4087808 | 0 | 90464 | 90464 | 0 | 3948544 |  |
| testDynamicStorageLibraryRamMode | yes | 4087808 | 4087808 | 0 | 92512 | 92512 | 0 | 3948544 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4087808 | 4087808 | 0 | 92512 | 92512 | 0 | 3948544 |  |
| testSimpleMutexFallbackBehavior | yes | 4087808 | 4157440 | 69632 | 92512 | 92512 | 0 | 4079616 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4157440 | 4157440 | 0 | 92512 | 92512 | 0 | 4079616 |  |
| testSimpleRWLockFallbackBehavior | yes | 4157440 | 4157440 | 0 | 92512 | 92704 | 192 | 4079616 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4157440 | 4157440 | 0 | 92704 | 92816 | 112 | 4079616 |  |

## Experimental Compile Results

_Each row is one experimental compile matrix entry (library + backend + optional mode)._

| Library | Backend | Optional | Success | ReturnCode | Artifact |
| --- | --- | --- | --- | ---: | --- |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_DynamicStorageLibrary-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_DynamicStorageLibrary-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetConnectionChecker-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetConnectionChecker-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetMach2ConnectionChecker-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetMach2ConnectionChecker-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Properties-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Properties-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleChannel-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleChannel-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleCoreRuntime-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleCoreRuntime-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleMutex-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleMutex-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleRWLock-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleRWLock-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleSemaphore-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleSemaphore-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleThreadManager-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleThreadManager-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_UnoQBridge-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_UnoQBridge-sd-off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/DynamicStorageLibrary | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_DynamicStorageLibrary/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/EthernetConnectionChecker | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetConnectionChecker/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/EthernetMach2ConnectionChecker | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetMach2ConnectionChecker/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/Hashtable | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/Properties | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_Properties/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleChannel | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleChannel/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleCoreRuntime | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleCoreRuntime/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleMutex | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleMutex/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleRWLock | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleRWLock/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleSemaphore | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleSemaphore/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleThreadManager | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleThreadManager/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleVector | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/UnoQBridge | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_UnoQBridge/sd/off |

## Stress Test Results

# Host Simulation Stress Test Report

## Summary

- Boards profiled: 10
- Total runs processed: 10
- Instance probe types: 14
- Element fill probe types: 27

## Understanding

- Counts shown are the **minimum** across all run variants for that board (most conservative).
- Heap delta from baseline is used for measurement (not absolute process RSS).
- ✓ means the probe stopped because the configured budget was reached.
- A count equal to the configured cap (HOST_STRESS_MAX_INSTANCES / HOST_STRESS_MAX_ELEMENTS) means the probe finished without hitting the budget.
- Budget is taken from each run's `limitBytes` field (CI typically sets it to `sramBytes × 1024`).

## Instance Count Probes

_How many simultaneously-alive empty instances fit within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList_int | SimpleVector_int | Hashtable_int_int | JSON | AVLTree_int | DynamicStorage_String_int | Predicates_int | Operators_int | Stack_int | Queue_int | LinkedList_int | OrderedMap_int_int | Variant_int | ExtremeVariant_int_int |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| due | 98304 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| esp32 | 532480 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| esp32c3 | 400000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| esp32s3 | 524288 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| esp8266 | 81920 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| mega2560 | 8192 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| micro | 2560 | 20000 | 20000 | 14161 ✓ | 9169 ✓ | 20000 | 7617 ✓ | 20000 | 20000 | 20000 | 20000 | 20000 | 10385 ✓ | 20000 | 20000 |
| uno | 2048 | 16385 ✓ | 20000 | 11185 ✓ | 7489 ✓ | 20000 | 6065 ✓ | 20000 | 20000 | 20000 | 20000 | 20000 | 8209 ✓ | 20000 | 20000 |
| ventunoq | 65536 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| zero | 32768 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |

✓ = budget reached during probe (count is the boundary value)

## Element Fill Probes

_How many elements fit in a single container instance within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList[int] | ArrayList[float] | ArrayList[double] | ArrayList[String] | ArrayList[Optional_int] | SimpleVector[int] | SimpleVector[float] | SimpleVector[double] | SimpleVector[Optional_int] | Hashtable[int_int] | Hashtable[String_String] | AVLTree[int] | DynamicStorage[String_int] | Stack[int] | Stack[float] | Stack[double] | Stack[String] | Queue[int] | Queue[float] | Queue[double] | Queue[String] | LinkedList[int] | LinkedList[float] | LinkedList[double] | LinkedList[String] | OrderedMap[int_int] | OrderedMap[String_String] |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| due | 98304 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 |
| esp32 | 532480 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 |
| esp32c3 | 400000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 |
| esp32s3 | 524288 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 | 1000000 |
| esp8266 | 81920 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 |
| mega2560 | 8192 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 |
| micro | 2560 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 49153 ✓ | 26369 ✓ | 50000 | 18689 ✓ | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 40961 ✓ | 50000 | 50000 | 50000 | 50000 | 50000 | 18433 ✓ |
| uno | 2048 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 46081 ✓ | 23041 ✓ | 43777 ✓ | 14593 ✓ | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 40961 ✓ | 50000 | 50000 | 50000 | 43777 ✓ | 50000 | 18433 ✓ |
| ventunoq | 65536 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 |
| zero | 32768 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 |

✓ = budget reached during probe (count is the boundary value)

## Per-Board Details

### due

- SRAM: 98304 bytes
- Host budget: 100663296 bytes (98304 × 1024)

### esp32

- SRAM: 532480 bytes
- Host budget: 545259520 bytes (532480 × 1024)

### esp32c3

- SRAM: 400000 bytes
- Host budget: 409600000 bytes (400000 × 1024)

### esp32s3

- SRAM: 524288 bytes
- Host budget: 536870912 bytes (524288 × 1024)

### esp8266

- SRAM: 81920 bytes
- Host budget: 83886080 bytes (81920 × 1024)

### mega2560

- SRAM: 8192 bytes
- Host budget: 8388608 bytes (8192 × 1024)

### micro

- SRAM: 2560 bytes
- Host budget: 2621440 bytes (2560 × 1024)

### uno

- SRAM: 2048 bytes
- Host budget: 2097152 bytes (2048 × 1024)

### ventunoq

- SRAM: 65536 bytes
- Host budget: 67108864 bytes (65536 × 1024)

### zero

- SRAM: 32768 bytes
- Host budget: 33554432 bytes (32768 × 1024)
