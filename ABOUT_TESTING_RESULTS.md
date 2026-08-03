# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4022272
- Avg peak bytes: 3992576
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
- Stress test runs loaded: 8

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
| optional-sd | yes | SD | 3977216 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 3985408 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3653632 | 3653632 | 0 | 77056 | 77328 | 272 | 3567616 |  |
| testHashtableBasicBehavior | yes | 3653632 | 3661824 | 8192 | 77472 | 79664 | 2192 | 3567616 |  |
| testSDListMemoryMode | yes | 3661824 | 3661824 | 0 | 79664 | 79760 | 96 | 3567616 |  |
| testSDListFileIOMode | yes | 3661824 | 3854336 | 192512 | 79760 | 81008 | 1248 | 3760128 |  |
| testCustomStringBehavior | yes | 3854336 | 3854336 | 0 | 81008 | 81008 | 0 | 3760128 |  |
| testJSONRoundTrip | yes | 3854336 | 3989504 | 135168 | 81008 | 82480 | 1472 | 3891200 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3989504 | 3993600 | 4096 | 82480 | 85600 | 3120 | 3891200 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3993600 | 4100096 | 106496 | 85600 | 87040 | 1440 | 4022272 |  |
| testSimpleVectorLifecycleStress | yes | 4100096 | 4100096 | 0 | 87040 | 87040 | 0 | 4022272 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4100096 | 4100096 | 0 | 89104 | 89152 | 48 | 4022272 |  |
| testJSONFileRoundTrip | yes | 4100096 | 4100096 | 0 | 89152 | 89472 | 320 | 4022272 |  |
| testSDListPersistenceStress | yes | 4100096 | 4100096 | 0 | 89472 | 89760 | 288 | 4022272 |  |
| testJSONPersistenceStress | yes | 4100096 | 4100096 | 0 | 89760 | 90576 | 816 | 4022272 |  |
| testAVLTreeBasicBehavior | yes | 4100096 | 4100096 | 0 | 90576 | 90672 | 96 | 4022272 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4100096 | 4100096 | 0 | 90672 | 90720 | 48 | 4022272 |  |
| testPredicatesBasicBehavior | yes | 4100096 | 4100096 | 0 | 90720 | 90720 | 0 | 4022272 |  |
| testOperatorsBasicBehavior | yes | 4100096 | 4100096 | 0 | 90720 | 90720 | 0 | 4022272 |  |
| testDynamicStorageLibraryRamMode | yes | 4100096 | 4100096 | 0 | 92768 | 92768 | 0 | 4022272 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4100096 | 4100096 | 0 | 92768 | 92768 | 0 | 4022272 |  |
| testSimpleMutexFallbackBehavior | yes | 4100096 | 4169728 | 69632 | 92768 | 92768 | 0 | 4022272 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92768 | 92768 | 0 | 4022272 |  |
| testSimpleRWLockFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92768 | 92912 | 144 | 4022272 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92912 | 92912 | 0 | 4022272 |  |

### Per-test Memory Stats: optional-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3616768 | 3616768 | 0 | 77232 | 77504 | 272 | 3530752 |  |
| testHashtableBasicBehavior | yes | 3616768 | 3624960 | 8192 | 77648 | 79776 | 2128 | 3530752 |  |
| testSDListMemoryMode | yes | 3624960 | 3624960 | 0 | 79776 | 79872 | 96 | 3530752 |  |
| testSDListFileIOMode | yes | 3624960 | 3817472 | 192512 | 79872 | 81120 | 1248 | 3723264 |  |
| testCustomStringBehavior | yes | 3817472 | 3817472 | 0 | 81120 | 81120 | 0 | 3723264 |  |
| testJSONRoundTrip | yes | 3817472 | 3952640 | 135168 | 81120 | 82592 | 1472 | 3854336 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3952640 | 3960832 | 8192 | 82592 | 85728 | 3136 | 3854336 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3960832 | 4063232 | 102400 | 85728 | 87264 | 1536 | 3985408 |  |
| testSimpleVectorLifecycleStress | yes | 4063232 | 4063232 | 0 | 87264 | 87264 | 0 | 3985408 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4063232 | 4063232 | 0 | 89328 | 89328 | 0 | 3985408 |  |
| testJSONFileRoundTrip | yes | 4063232 | 4063232 | 0 | 89328 | 89808 | 480 | 3985408 |  |
| testSDListPersistenceStress | yes | 4063232 | 4063232 | 0 | 89760 | 90128 | 368 | 3985408 |  |
| testJSONPersistenceStress | yes | 4063232 | 4063232 | 0 | 90128 | 91168 | 1040 | 3985408 |  |
| testAVLTreeBasicBehavior | yes | 4063232 | 4063232 | 0 | 91168 | 91344 | 176 | 3985408 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4063232 | 4063232 | 0 | 91344 | 91504 | 160 | 3985408 |  |
| testPredicatesBasicBehavior | yes | 4063232 | 4063232 | 0 | 91504 | 91504 | 0 | 3985408 |  |
| testOperatorsBasicBehavior | yes | 4063232 | 4063232 | 0 | 91504 | 91504 | 0 | 3985408 |  |
| testDynamicStorageLibraryRamMode | yes | 4063232 | 4063232 | 0 | 93552 | 93552 | 0 | 3985408 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4063232 | 4063232 | 0 | 93552 | 93552 | 0 | 3985408 |  |
| testSimpleMutexFallbackBehavior | yes | 4063232 | 4132864 | 69632 | 93552 | 93552 | 0 | 3985408 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4132864 | 4132864 | 0 | 93552 | 93552 | 0 | 3985408 |  |
| testSimpleRWLockFallbackBehavior | yes | 4132864 | 4132864 | 0 | 93552 | 93696 | 144 | 3985408 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4132864 | 4132864 | 0 | 93696 | 93696 | 0 | 3985408 |  |
| testAVLTreeErrorCodes | yes | 4132864 | 4132864 | 0 | 93696 | 93744 | 48 | 3985408 |  |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3608576 | 3608576 | 0 | 77040 | 77312 | 272 | 3522560 |  |
| testHashtableBasicBehavior | yes | 3608576 | 3616768 | 8192 | 77456 | 79648 | 2192 | 3522560 |  |
| testSDListMemoryMode | yes | 3616768 | 3616768 | 0 | 79648 | 79744 | 96 | 3522560 |  |
| testSDListFileIOMode | yes | 3616768 | 3809280 | 192512 | 79744 | 80848 | 1104 | 3715072 |  |
| testCustomStringBehavior | yes | 3809280 | 3809280 | 0 | 80848 | 80848 | 0 | 3715072 |  |
| testJSONRoundTrip | yes | 3809280 | 3944448 | 135168 | 80848 | 82320 | 1472 | 3846144 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3944448 | 3948544 | 4096 | 82320 | 85456 | 3136 | 3846144 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3948544 | 4055040 | 106496 | 85456 | 87120 | 1664 | 3977216 |  |
| testSimpleVectorLifecycleStress | yes | 4055040 | 4055040 | 0 | 87120 | 87120 | 0 | 3977216 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4055040 | 4055040 | 0 | 89184 | 89184 | 0 | 3977216 |  |
| testJSONFileRoundTrip | yes | 4055040 | 4055040 | 0 | 89184 | 89664 | 480 | 3977216 |  |
| testSDListPersistenceStress | yes | 4055040 | 4055040 | 0 | 89616 | 89680 | 64 | 3977216 |  |
| testJSONPersistenceStress | yes | 4055040 | 4055040 | 0 | 89680 | 90384 | 704 | 3977216 |  |
| testAVLTreeBasicBehavior | yes | 4055040 | 4055040 | 0 | 90384 | 90544 | 160 | 3977216 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4055040 | 4055040 | 0 | 90544 | 90784 | 240 | 3977216 |  |
| testPredicatesBasicBehavior | yes | 4055040 | 4055040 | 0 | 90784 | 90784 | 0 | 3977216 |  |
| testOperatorsBasicBehavior | yes | 4055040 | 4055040 | 0 | 90784 | 90784 | 0 | 3977216 |  |
| testDynamicStorageLibraryRamMode | yes | 4055040 | 4055040 | 0 | 92832 | 92832 | 0 | 3977216 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4055040 | 4055040 | 0 | 92832 | 92832 | 0 | 3977216 |  |
| testSimpleMutexFallbackBehavior | yes | 4055040 | 4124672 | 69632 | 92832 | 92832 | 0 | 3977216 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4124672 | 4124672 | 0 | 92832 | 92832 | 0 | 3977216 |  |
| testSimpleRWLockFallbackBehavior | yes | 4124672 | 4124672 | 0 | 92832 | 92976 | 144 | 3977216 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4124672 | 4124672 | 0 | 92976 | 93088 | 112 | 3977216 |  |
| testAVLTreeErrorCodes | yes | 4124672 | 4124672 | 0 | 93088 | 93136 | 48 | 3977216 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3612672 | 3612672 | 0 | 76928 | 77200 | 272 | 3530752 |  |
| testHashtableBasicBehavior | yes | 3612672 | 3620864 | 8192 | 77344 | 79536 | 2192 | 3530752 |  |
| testSDListMemoryMode | yes | 3620864 | 3620864 | 0 | 79536 | 79632 | 96 | 3530752 |  |
| testSDListFileIOMode | yes | 3620864 | 3813376 | 192512 | 79632 | 80752 | 1120 | 3723264 |  |
| testCustomStringBehavior | yes | 3813376 | 3813376 | 0 | 80752 | 80752 | 0 | 3723264 |  |
| testJSONRoundTrip | yes | 3813376 | 3948544 | 135168 | 80752 | 82272 | 1520 | 3854336 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3948544 | 3952640 | 4096 | 82272 | 85312 | 3040 | 3854336 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3952640 | 4059136 | 106496 | 85312 | 86832 | 1520 | 3985408 |  |
| testSimpleVectorLifecycleStress | yes | 4059136 | 4059136 | 0 | 86832 | 86832 | 0 | 3985408 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4059136 | 4059136 | 0 | 88896 | 88896 | 0 | 3985408 |  |
| testJSONFileRoundTrip | yes | 4059136 | 4059136 | 0 | 88896 | 89152 | 256 | 3985408 |  |
| testSDListPersistenceStress | yes | 4059136 | 4059136 | 0 | 89152 | 89504 | 352 | 3985408 |  |
| testJSONPersistenceStress | yes | 4059136 | 4059136 | 0 | 89504 | 90320 | 816 | 3985408 |  |
| testAVLTreeBasicBehavior | yes | 4059136 | 4059136 | 0 | 90320 | 90368 | 48 | 3985408 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4059136 | 4059136 | 0 | 90368 | 90464 | 96 | 3985408 |  |
| testPredicatesBasicBehavior | yes | 4059136 | 4059136 | 0 | 90464 | 90464 | 0 | 3985408 |  |
| testOperatorsBasicBehavior | yes | 4059136 | 4059136 | 0 | 90464 | 90464 | 0 | 3985408 |  |
| testDynamicStorageLibraryRamMode | yes | 4059136 | 4059136 | 0 | 92512 | 92512 | 0 | 3985408 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4059136 | 4059136 | 0 | 92512 | 92512 | 0 | 3985408 |  |
| testSimpleMutexFallbackBehavior | yes | 4059136 | 4128768 | 69632 | 92512 | 92512 | 0 | 3985408 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92512 | 92512 | 0 | 3985408 |  |
| testSimpleRWLockFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92512 | 92704 | 192 | 3985408 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92704 | 92816 | 112 | 3985408 |  |

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

- Boards profiled: 2
- Total runs processed: 8
- Instance probe types: 8
- Element fill probe types: 11

## Understanding

- Counts shown are the **minimum** across all run variants for that board (most conservative).
- Heap delta from baseline is used for measurement (not absolute process RSS).
- ✓ means the probe stopped because the configured budget was reached.
- A count equal to the configured cap (HOST_STRESS_MAX_INSTANCES / HOST_STRESS_MAX_ELEMENTS) means the probe finished without hitting the budget.
- Budget is taken from each run's `limitBytes` field (CI typically sets it to `sramBytes × 1024`).

## Instance Count Probes

_How many simultaneously-alive empty instances fit within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList_int | SimpleVector_int | Hashtable_int_int | JSON | AVLTree_int | DynamicStorage_String_int | Predicates_int | Operators_int |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| unoq | 32768 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| ventunoq | 65536 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |

✓ = budget reached during probe (count is the boundary value)

## Element Fill Probes

_How many elements fit in a single container instance within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList[int] | ArrayList[float] | ArrayList[double] | ArrayList[String] | SimpleVector[int] | SimpleVector[float] | SimpleVector[double] | Hashtable[int_int] | Hashtable[String_String] | AVLTree[int] | DynamicStorage[String_int] |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| unoq | 32768 | 5000000 | 5000000 | 5000000 | 1005313 ✓ | 5000000 | 5000000 | 5000000 | 734209 ✓ | 367105 ✓ | 699137 ✓ | 262401 ✓ |
| ventunoq | 65536 | 5000000 | 5000000 | 5000000 | 5000000 | 5000000 | 5000000 | 5000000 | 2097153 ✓ | 734209 ✓ | 1398273 ✓ | 524545 ✓ |

✓ = budget reached during probe (count is the boundary value)

## Per-Board Details

### unoq

- SRAM: 32768 bytes
- Host budget: 33554432 bytes (32768 × 1024)

### ventunoq

- SRAM: 65536 bytes
- Host budget: 67108864 bytes (65536 × 1024)
