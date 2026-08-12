# Host Simulation Testing Report

## Summary

- Total runs: 2
- Passed runs: 2
- Failed runs: 0
- Max peak bytes: 4030464
- Avg peak bytes: 4026368
- Compile smoke objects found: 58
- Experimental compile results found: 22
- Experimental compile successes: 18
- Experimental compile failures: 4
- Experimental libraries covered: 14
- Experimental backends covered: 2
- Experimental optional modes covered: 10
- Memory profile runs: 0
- Runs that exceeded limit: 0
- Runs with limit enforcement enabled: 2
- Runs with capacity probe enabled: 0
- Runs where capacity probe reached limit: 0
- Runs with first limit-crossing test identified: 0
- Expected compile smoke objects: 49
- Expected experimental compile results: 13
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
| optional-sd | yes | SD | 4030464 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4022272 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3657728 | 3657728 | 0 | 77040 | 77312 | 272 | 3575808 |  |
| testHashtableBasicBehavior | yes | 3657728 | 3665920 | 8192 | 77456 | 79648 | 2192 | 3575808 |  |
| testSDListMemoryMode | yes | 3665920 | 3665920 | 0 | 79648 | 79744 | 96 | 3575808 |  |
| testSDListFileIOMode | yes | 3665920 | 3858432 | 192512 | 79744 | 80848 | 1104 | 3768320 |  |
| testCustomStringBehavior | yes | 3858432 | 3858432 | 0 | 80848 | 80848 | 0 | 3768320 |  |
| testJSONRoundTrip | yes | 3858432 | 3993600 | 135168 | 80848 | 82320 | 1472 | 3899392 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3993600 | 3997696 | 4096 | 82320 | 85456 | 3136 | 3899392 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3997696 | 4104192 | 106496 | 85456 | 87120 | 1664 | 4030464 |  |
| testSimpleVectorLifecycleStress | yes | 4104192 | 4104192 | 0 | 87120 | 87120 | 0 | 4030464 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4104192 | 4104192 | 0 | 89184 | 89184 | 0 | 4030464 |  |
| testJSONFileRoundTrip | yes | 4104192 | 4104192 | 0 | 89184 | 89664 | 480 | 4030464 |  |
| testSDListPersistenceStress | yes | 4104192 | 4104192 | 0 | 89616 | 89680 | 64 | 4030464 |  |
| testJSONPersistenceStress | yes | 4104192 | 4104192 | 0 | 89680 | 90384 | 704 | 4030464 |  |
| testAVLTreeBasicBehavior | yes | 4104192 | 4104192 | 0 | 90384 | 90544 | 160 | 4030464 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4104192 | 4104192 | 0 | 90544 | 90784 | 240 | 4030464 |  |
| testPredicatesBasicBehavior | yes | 4104192 | 4104192 | 0 | 90784 | 90784 | 0 | 4030464 |  |
| testOperatorsBasicBehavior | yes | 4104192 | 4104192 | 0 | 90784 | 90784 | 0 | 4030464 |  |
| testDynamicStorageLibraryRamMode | yes | 4104192 | 4104192 | 0 | 92832 | 92832 | 0 | 4030464 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4104192 | 4104192 | 0 | 92832 | 92832 | 0 | 4030464 |  |
| testSimpleMutexFallbackBehavior | yes | 4104192 | 4173824 | 69632 | 92832 | 92832 | 0 | 4030464 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92832 | 92832 | 0 | 4030464 |  |
| testSimpleRWLockFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92832 | 92976 | 144 | 4030464 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92976 | 93088 | 112 | 4030464 |  |
| testAVLTreeErrorCodes | yes | 4173824 | 4173824 | 0 | 93088 | 93136 | 48 | 4030464 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3653632 | 3653632 | 0 | 76928 | 77200 | 272 | 3567616 |  |
| testHashtableBasicBehavior | yes | 3653632 | 3661824 | 8192 | 77344 | 79536 | 2192 | 3567616 |  |
| testSDListMemoryMode | yes | 3661824 | 3661824 | 0 | 79536 | 79632 | 96 | 3567616 |  |
| testSDListFileIOMode | yes | 3661824 | 3854336 | 192512 | 79632 | 80752 | 1120 | 3760128 |  |
| testCustomStringBehavior | yes | 3854336 | 3854336 | 0 | 80752 | 80752 | 0 | 3760128 |  |
| testJSONRoundTrip | yes | 3854336 | 3989504 | 135168 | 80752 | 82272 | 1520 | 3891200 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3989504 | 3993600 | 4096 | 82272 | 85312 | 3040 | 3891200 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3993600 | 4100096 | 106496 | 85312 | 86832 | 1520 | 4022272 |  |
| testSimpleVectorLifecycleStress | yes | 4100096 | 4100096 | 0 | 86832 | 86832 | 0 | 4022272 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4100096 | 4100096 | 0 | 88896 | 88896 | 0 | 4022272 |  |
| testJSONFileRoundTrip | yes | 4100096 | 4100096 | 0 | 88896 | 89152 | 256 | 4022272 |  |
| testSDListPersistenceStress | yes | 4100096 | 4100096 | 0 | 89152 | 89504 | 352 | 4022272 |  |
| testJSONPersistenceStress | yes | 4100096 | 4100096 | 0 | 89504 | 90320 | 816 | 4022272 |  |
| testAVLTreeBasicBehavior | yes | 4100096 | 4100096 | 0 | 90320 | 90368 | 48 | 4022272 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4100096 | 4100096 | 0 | 90368 | 90464 | 96 | 4022272 |  |
| testPredicatesBasicBehavior | yes | 4100096 | 4100096 | 0 | 90464 | 90464 | 0 | 4022272 |  |
| testOperatorsBasicBehavior | yes | 4100096 | 4100096 | 0 | 90464 | 90464 | 0 | 4022272 |  |
| testDynamicStorageLibraryRamMode | yes | 4100096 | 4100096 | 0 | 92512 | 92512 | 0 | 4022272 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4100096 | 4100096 | 0 | 92512 | 92512 | 0 | 4022272 |  |
| testSimpleMutexFallbackBehavior | yes | 4100096 | 4169728 | 69632 | 92512 | 92512 | 0 | 4022272 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92512 | 92512 | 0 | 4022272 |  |
| testSimpleRWLockFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92512 | 92704 | 192 | 4022272 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92704 | 92816 | 112 | 4022272 |  |

## Experimental Compile Results

_Each row is one experimental compile matrix entry (library + backend + optional mode)._

| Library | Backend | Optional | Success | ReturnCode | Artifact |
| --- | --- | --- | --- | ---: | --- |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_Hashtable-sd-off |
| .artifacts | host-compile | host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SDList-sd-off | yes | 0 | .artifacts/host-compile/host-experimental-library-compile-lib___ArduinoUnoQLibraryDevelopment__Experimental_SDList-sd-off |
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
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SDList | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SDList/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleChannel | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleChannel/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleCoreRuntime | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleCoreRuntime/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleMutex | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleMutex/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleRWLock | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleRWLock/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleSemaphore | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleSemaphore/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleThreadManager | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleThreadManager/sd/off |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleVector | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleVector/sd/off |
