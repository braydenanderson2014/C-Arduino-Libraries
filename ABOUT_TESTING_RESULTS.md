# Host Simulation Testing Report

## Summary

- Total runs: 2
- Passed runs: 2
- Failed runs: 0
- Max peak bytes: 4059136
- Avg peak bytes: 4020224
- Compile smoke objects found: 57
- Experimental compile results found: 20
- Experimental compile successes: 16
- Experimental compile failures: 4
- Experimental libraries covered: 13
- Experimental backends covered: 2
- Experimental optional modes covered: 9
- Memory profile runs: 0
- Runs that exceeded limit: 0
- Runs with limit enforcement enabled: 2
- Runs with capacity probe enabled: 0
- Runs where capacity probe reached limit: 0
- Runs with first limit-crossing test identified: 0
- Expected compile smoke objects: 49
- Expected experimental compile results: 12
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
| optional-sd | no | no | no |  |
| sd | no | no | no |  |

## Run Results

| Run | Success | Backend | PeakBytes | LimitBytes | LimitExceeded | LimitEnforced | FirstLimitCrossingTest | CrossingPeakBytes | ProbeElementsAtStop | ProbeCurrentBytesAtStop |
| --- | --- | --- | ---: | ---: | --- | --- | --- | ---: | ---: | ---: |
| optional-sd | yes | SD | 4059136 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 3981312 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3620864 | 3620864 | 0 | 77040 | 77312 | 272 | 3473408 |  |
| testHashtableBasicBehavior | yes | 3620864 | 3629056 | 8192 | 77456 | 79648 | 2192 | 3473408 |  |
| testSDListMemoryMode | yes | 3629056 | 3629056 | 0 | 79648 | 79744 | 96 | 3473408 |  |
| testSDListFileIOMode | yes | 3629056 | 3821568 | 192512 | 79744 | 80848 | 1104 | 3665920 |  |
| testCustomStringBehavior | yes | 3821568 | 3821568 | 0 | 80848 | 80848 | 0 | 3665920 |  |
| testJSONRoundTrip | yes | 3821568 | 3956736 | 135168 | 80848 | 82320 | 1472 | 3796992 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3956736 | 3960832 | 4096 | 82320 | 85456 | 3136 | 3796992 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3960832 | 4067328 | 106496 | 85456 | 87120 | 1664 | 3928064 |  |
| testSimpleVectorLifecycleStress | yes | 4067328 | 4067328 | 0 | 87120 | 87120 | 0 | 3928064 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4067328 | 4067328 | 0 | 89184 | 89184 | 0 | 3928064 |  |
| testJSONFileRoundTrip | yes | 4067328 | 4067328 | 0 | 89184 | 89664 | 480 | 3928064 |  |
| testSDListPersistenceStress | yes | 4067328 | 4067328 | 0 | 89616 | 89680 | 64 | 3928064 |  |
| testJSONPersistenceStress | yes | 4067328 | 4067328 | 0 | 89680 | 90384 | 704 | 3928064 |  |
| testAVLTreeBasicBehavior | yes | 4067328 | 4067328 | 0 | 90384 | 90544 | 160 | 3928064 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4067328 | 4067328 | 0 | 90544 | 90784 | 240 | 3928064 |  |
| testPredicatesBasicBehavior | yes | 4067328 | 4067328 | 0 | 90784 | 90784 | 0 | 3928064 |  |
| testOperatorsBasicBehavior | yes | 4067328 | 4067328 | 0 | 90784 | 90784 | 0 | 3928064 |  |
| testDynamicStorageLibraryRamMode | yes | 4067328 | 4067328 | 0 | 92832 | 92832 | 0 | 3928064 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4067328 | 4067328 | 0 | 92832 | 92832 | 0 | 3928064 |  |
| testSimpleMutexFallbackBehavior | yes | 4067328 | 4136960 | 69632 | 92832 | 92832 | 0 | 4059136 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4136960 | 4136960 | 0 | 92832 | 92832 | 0 | 4059136 |  |
| testSimpleRWLockFallbackBehavior | yes | 4136960 | 4136960 | 0 | 92832 | 92976 | 144 | 4059136 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4136960 | 4136960 | 0 | 92976 | 93088 | 112 | 4059136 |  |
| testAVLTreeErrorCodes | yes | 4136960 | 4136960 | 0 | 93088 | 93136 | 48 | 4059136 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3608576 | 3608576 | 0 | 76928 | 77200 | 272 | 3526656 |  |
| testHashtableBasicBehavior | yes | 3608576 | 3616768 | 8192 | 77344 | 79536 | 2192 | 3526656 |  |
| testSDListMemoryMode | yes | 3616768 | 3616768 | 0 | 79536 | 79632 | 96 | 3526656 |  |
| testSDListFileIOMode | yes | 3616768 | 3809280 | 192512 | 79632 | 80752 | 1120 | 3719168 |  |
| testCustomStringBehavior | yes | 3809280 | 3809280 | 0 | 80752 | 80752 | 0 | 3719168 |  |
| testJSONRoundTrip | yes | 3809280 | 3944448 | 135168 | 80752 | 82272 | 1520 | 3850240 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3944448 | 3948544 | 4096 | 82272 | 85312 | 3040 | 3850240 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3948544 | 4055040 | 106496 | 85312 | 86832 | 1520 | 3981312 |  |
| testSimpleVectorLifecycleStress | yes | 4055040 | 4055040 | 0 | 86832 | 86832 | 0 | 3981312 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4055040 | 4055040 | 0 | 88896 | 88896 | 0 | 3981312 |  |
| testJSONFileRoundTrip | yes | 4055040 | 4055040 | 0 | 88896 | 89152 | 256 | 3981312 |  |
| testSDListPersistenceStress | yes | 4055040 | 4055040 | 0 | 89152 | 89504 | 352 | 3981312 |  |
| testJSONPersistenceStress | yes | 4055040 | 4055040 | 0 | 89504 | 90320 | 816 | 3981312 |  |
| testAVLTreeBasicBehavior | yes | 4055040 | 4055040 | 0 | 90320 | 90368 | 48 | 3981312 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4055040 | 4055040 | 0 | 90368 | 90464 | 96 | 3981312 |  |
| testPredicatesBasicBehavior | yes | 4055040 | 4055040 | 0 | 90464 | 90464 | 0 | 3981312 |  |
| testOperatorsBasicBehavior | yes | 4055040 | 4055040 | 0 | 90464 | 90464 | 0 | 3981312 |  |
| testDynamicStorageLibraryRamMode | yes | 4055040 | 4055040 | 0 | 92512 | 92512 | 0 | 3981312 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4055040 | 4055040 | 0 | 92512 | 92512 | 0 | 3981312 |  |
| testSimpleMutexFallbackBehavior | yes | 4055040 | 4124672 | 69632 | 92512 | 92512 | 0 | 3981312 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4124672 | 4124672 | 0 | 92512 | 92512 | 0 | 3981312 |  |
| testSimpleRWLockFallbackBehavior | yes | 4124672 | 4124672 | 0 | 92512 | 92704 | 192 | 3981312 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4124672 | 4124672 | 0 | 92704 | 92816 | 112 | 3981312 |  |

## Experimental Compile Results

_Each row is one experimental compile matrix entry (library + backend + optional mode)._

| Library | Backend | Optional | Success | ReturnCode | Artifact |
| --- | --- | --- | --- | ---: | --- |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleChannel-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleChannel-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleCoreRuntime-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleCoreRuntime-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleMutex-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleMutex-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleRWLock-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleRWLock-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleSemaphore-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleSemaphore-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleThreadManager-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleThreadManager-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector-sd-off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/DynamicStorageLibrary | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_DynamicStorageLibrary/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/EthernetConnectionChecker | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetConnectionChecker/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/EthernetMach2ConnectionChecker | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_EthernetMach2ConnectionChecker/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/Hashtable | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/Properties | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_Properties/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleChannel | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleChannel/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleCoreRuntime | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleCoreRuntime/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleMutex | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleMutex/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleRWLock | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleRWLock/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleSemaphore | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleSemaphore/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleThreadManager | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleThreadManager/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleVector | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector/sd/off |
