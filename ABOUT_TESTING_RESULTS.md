# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4022272
- Avg peak bytes: 4005888
- Compile smoke objects found: 63
- Experimental compile results found: 26
- Experimental compile successes: 26
- Experimental compile failures: 0
- Experimental libraries covered: 14
- Experimental backends covered: 2
- Experimental optional modes covered: 14
- Memory profile runs: 0
- Runs that exceeded limit: 0
- Runs with limit enforcement enabled: 4
- Runs with capacity probe enabled: 0
- Runs where capacity probe reached limit: 0
- Runs with first limit-crossing test identified: 0
- Expected compile smoke objects: 50
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
| littlefs | no | no | no |  |
| optional-littlefs | no | no | no |  |
| optional-sd | no | no | no |  |
| sd | no | no | no |  |

## Run Results

| Run | Success | Backend | PeakBytes | LimitBytes | LimitExceeded | LimitEnforced | FirstLimitCrossingTest | CrossingPeakBytes | ProbeElementsAtStop | ProbeCurrentBytesAtStop |
| --- | --- | --- | ---: | ---: | --- | --- | --- | ---: | ---: | ---: |
| littlefs | yes | LittleFS | 4022272 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-littlefs | yes | LittleFS | 3985408 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-sd | yes | SD | 3993600 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4022272 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3649536 | 3649536 | 0 | 77056 | 77328 | 272 | 3567616 |  |
| testHashtableBasicBehavior | yes | 3649536 | 3657728 | 8192 | 77472 | 79664 | 2192 | 3567616 |  |
| testSDListMemoryMode | yes | 3657728 | 3657728 | 0 | 79664 | 79760 | 96 | 3567616 |  |
| testSDListFileIOMode | yes | 3657728 | 3850240 | 192512 | 79760 | 81008 | 1248 | 3760128 |  |
| testCustomStringBehavior | yes | 3850240 | 3850240 | 0 | 81008 | 81008 | 0 | 3760128 |  |
| testJSONRoundTrip | yes | 3850240 | 3985408 | 135168 | 81008 | 82480 | 1472 | 3891200 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3985408 | 3989504 | 4096 | 82480 | 85600 | 3120 | 3891200 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3989504 | 4096000 | 106496 | 85600 | 87040 | 1440 | 4022272 |  |
| testSimpleVectorLifecycleStress | yes | 4096000 | 4096000 | 0 | 87040 | 87040 | 0 | 4022272 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4096000 | 4096000 | 0 | 89104 | 89152 | 48 | 4022272 |  |
| testJSONFileRoundTrip | yes | 4096000 | 4096000 | 0 | 89152 | 89472 | 320 | 4022272 |  |
| testSDListPersistenceStress | yes | 4096000 | 4096000 | 0 | 89472 | 89760 | 288 | 4022272 |  |
| testJSONPersistenceStress | yes | 4096000 | 4096000 | 0 | 89760 | 90576 | 816 | 4022272 |  |
| testAVLTreeBasicBehavior | yes | 4096000 | 4096000 | 0 | 90576 | 90672 | 96 | 4022272 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4096000 | 4096000 | 0 | 90672 | 90720 | 48 | 4022272 |  |
| testPredicatesBasicBehavior | yes | 4096000 | 4096000 | 0 | 90720 | 90720 | 0 | 4022272 |  |
| testOperatorsBasicBehavior | yes | 4096000 | 4096000 | 0 | 90720 | 90720 | 0 | 4022272 |  |
| testDynamicStorageLibraryRamMode | yes | 4096000 | 4096000 | 0 | 92768 | 92768 | 0 | 4022272 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4096000 | 4096000 | 0 | 92768 | 92768 | 0 | 4022272 |  |
| testSimpleMutexFallbackBehavior | yes | 4096000 | 4165632 | 69632 | 92768 | 92768 | 0 | 4022272 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4165632 | 4165632 | 0 | 92768 | 92768 | 0 | 4022272 |  |
| testSimpleRWLockFallbackBehavior | yes | 4165632 | 4165632 | 0 | 92768 | 92912 | 144 | 4022272 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4165632 | 4165632 | 0 | 92912 | 92912 | 0 | 4022272 |  |

### Per-test Memory Stats: optional-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3612672 | 3612672 | 0 | 77232 | 77504 | 272 | 3530752 |  |
| testHashtableBasicBehavior | yes | 3612672 | 3620864 | 8192 | 77648 | 79776 | 2128 | 3530752 |  |
| testSDListMemoryMode | yes | 3620864 | 3620864 | 0 | 79776 | 79872 | 96 | 3530752 |  |
| testSDListFileIOMode | yes | 3620864 | 3813376 | 192512 | 79872 | 81120 | 1248 | 3723264 |  |
| testCustomStringBehavior | yes | 3813376 | 3813376 | 0 | 81120 | 81120 | 0 | 3723264 |  |
| testJSONRoundTrip | yes | 3813376 | 3948544 | 135168 | 81120 | 82592 | 1472 | 3854336 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3948544 | 3956736 | 8192 | 82592 | 85728 | 3136 | 3854336 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3956736 | 4059136 | 102400 | 85728 | 87264 | 1536 | 3985408 |  |
| testSimpleVectorLifecycleStress | yes | 4059136 | 4059136 | 0 | 87264 | 87264 | 0 | 3985408 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4059136 | 4059136 | 0 | 89328 | 89328 | 0 | 3985408 |  |
| testJSONFileRoundTrip | yes | 4059136 | 4059136 | 0 | 89328 | 89808 | 480 | 3985408 |  |
| testSDListPersistenceStress | yes | 4059136 | 4059136 | 0 | 89760 | 90128 | 368 | 3985408 |  |
| testJSONPersistenceStress | yes | 4059136 | 4059136 | 0 | 90128 | 91168 | 1040 | 3985408 |  |
| testAVLTreeBasicBehavior | yes | 4059136 | 4059136 | 0 | 91168 | 91344 | 176 | 3985408 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4059136 | 4059136 | 0 | 91344 | 91504 | 160 | 3985408 |  |
| testPredicatesBasicBehavior | yes | 4059136 | 4059136 | 0 | 91504 | 91504 | 0 | 3985408 |  |
| testOperatorsBasicBehavior | yes | 4059136 | 4059136 | 0 | 91504 | 91504 | 0 | 3985408 |  |
| testDynamicStorageLibraryRamMode | yes | 4059136 | 4059136 | 0 | 93552 | 93552 | 0 | 3985408 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4059136 | 4059136 | 0 | 93552 | 93552 | 0 | 3985408 |  |
| testSimpleMutexFallbackBehavior | yes | 4059136 | 4128768 | 69632 | 93552 | 93552 | 0 | 3985408 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4128768 | 4128768 | 0 | 93552 | 93552 | 0 | 3985408 |  |
| testSimpleRWLockFallbackBehavior | yes | 4128768 | 4128768 | 0 | 93552 | 93696 | 144 | 3985408 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4128768 | 4128768 | 0 | 93696 | 93696 | 0 | 3985408 |  |
| testAVLTreeErrorCodes | yes | 4128768 | 4128768 | 0 | 93696 | 93744 | 48 | 3985408 |  |

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
| testArrayListBasicBehavior | yes | 3649536 | 3649536 | 0 | 76928 | 77200 | 272 | 3567616 |  |
| testHashtableBasicBehavior | yes | 3649536 | 3657728 | 8192 | 77344 | 79536 | 2192 | 3567616 |  |
| testSDListMemoryMode | yes | 3657728 | 3657728 | 0 | 79536 | 79632 | 96 | 3567616 |  |
| testSDListFileIOMode | yes | 3657728 | 3850240 | 192512 | 79632 | 80752 | 1120 | 3760128 |  |
| testCustomStringBehavior | yes | 3850240 | 3850240 | 0 | 80752 | 80752 | 0 | 3760128 |  |
| testJSONRoundTrip | yes | 3850240 | 3985408 | 135168 | 80752 | 82272 | 1520 | 3891200 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3985408 | 3989504 | 4096 | 82272 | 85312 | 3040 | 3891200 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3989504 | 4096000 | 106496 | 85312 | 86832 | 1520 | 4022272 |  |
| testSimpleVectorLifecycleStress | yes | 4096000 | 4096000 | 0 | 86832 | 86832 | 0 | 4022272 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4096000 | 4096000 | 0 | 88896 | 88896 | 0 | 4022272 |  |
| testJSONFileRoundTrip | yes | 4096000 | 4096000 | 0 | 88896 | 89152 | 256 | 4022272 |  |
| testSDListPersistenceStress | yes | 4096000 | 4096000 | 0 | 89152 | 89504 | 352 | 4022272 |  |
| testJSONPersistenceStress | yes | 4096000 | 4096000 | 0 | 89504 | 90320 | 816 | 4022272 |  |
| testAVLTreeBasicBehavior | yes | 4096000 | 4096000 | 0 | 90320 | 90368 | 48 | 4022272 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4096000 | 4096000 | 0 | 90368 | 90464 | 96 | 4022272 |  |
| testPredicatesBasicBehavior | yes | 4096000 | 4096000 | 0 | 90464 | 90464 | 0 | 4022272 |  |
| testOperatorsBasicBehavior | yes | 4096000 | 4096000 | 0 | 90464 | 90464 | 0 | 4022272 |  |
| testDynamicStorageLibraryRamMode | yes | 4096000 | 4096000 | 0 | 92512 | 92512 | 0 | 4022272 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4096000 | 4096000 | 0 | 92512 | 92512 | 0 | 4022272 |  |
| testSimpleMutexFallbackBehavior | yes | 4096000 | 4165632 | 69632 | 92512 | 92512 | 0 | 4022272 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4165632 | 4165632 | 0 | 92512 | 92512 | 0 | 4022272 |  |
| testSimpleRWLockFallbackBehavior | yes | 4165632 | 4165632 | 0 | 92512 | 92704 | 192 | 4022272 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4165632 | 4165632 | 0 | 92704 | 92816 | 112 | 4022272 |  |

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
