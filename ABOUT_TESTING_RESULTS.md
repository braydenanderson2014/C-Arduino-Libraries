# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4050944
- Avg peak bytes: 3996672
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
- Stress test runs loaded: 7

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
| littlefs | yes | LittleFS | 3985408 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-littlefs | yes | LittleFS | 3956736 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-sd | yes | SD | 3993600 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4050944 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3612672 | 3612672 | 0 | 77056 | 77328 | 272 | 3530752 |  |
| testHashtableBasicBehavior | yes | 3612672 | 3620864 | 8192 | 77472 | 79664 | 2192 | 3530752 |  |
| testSDListMemoryMode | yes | 3620864 | 3620864 | 0 | 79664 | 79760 | 96 | 3530752 |  |
| testSDListFileIOMode | yes | 3620864 | 3813376 | 192512 | 79760 | 81008 | 1248 | 3723264 |  |
| testCustomStringBehavior | yes | 3813376 | 3813376 | 0 | 81008 | 81008 | 0 | 3723264 |  |
| testJSONRoundTrip | yes | 3813376 | 3948544 | 135168 | 81008 | 82480 | 1472 | 3854336 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3948544 | 3952640 | 4096 | 82480 | 85600 | 3120 | 3854336 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3952640 | 4059136 | 106496 | 85600 | 87040 | 1440 | 3985408 |  |
| testSimpleVectorLifecycleStress | yes | 4059136 | 4059136 | 0 | 87040 | 87040 | 0 | 3985408 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4059136 | 4059136 | 0 | 89104 | 89152 | 48 | 3985408 |  |
| testJSONFileRoundTrip | yes | 4059136 | 4059136 | 0 | 89152 | 89472 | 320 | 3985408 |  |
| testSDListPersistenceStress | yes | 4059136 | 4059136 | 0 | 89472 | 89760 | 288 | 3985408 |  |
| testJSONPersistenceStress | yes | 4059136 | 4059136 | 0 | 89760 | 90576 | 816 | 3985408 |  |
| testAVLTreeBasicBehavior | yes | 4059136 | 4059136 | 0 | 90576 | 90672 | 96 | 3985408 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4059136 | 4059136 | 0 | 90672 | 90720 | 48 | 3985408 |  |
| testPredicatesBasicBehavior | yes | 4059136 | 4059136 | 0 | 90720 | 90720 | 0 | 3985408 |  |
| testOperatorsBasicBehavior | yes | 4059136 | 4059136 | 0 | 90720 | 90720 | 0 | 3985408 |  |
| testDynamicStorageLibraryRamMode | yes | 4059136 | 4059136 | 0 | 92768 | 92768 | 0 | 3985408 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4059136 | 4059136 | 0 | 92768 | 92768 | 0 | 3985408 |  |
| testSimpleMutexFallbackBehavior | yes | 4059136 | 4128768 | 69632 | 92768 | 92768 | 0 | 3985408 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92768 | 92768 | 0 | 3985408 |  |
| testSimpleRWLockFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92768 | 92912 | 144 | 3985408 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92912 | 92912 | 0 | 3985408 |  |

### Per-test Memory Stats: optional-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3653632 | 3653632 | 0 | 77232 | 77504 | 272 | 3502080 |  |
| testHashtableBasicBehavior | yes | 3653632 | 3661824 | 8192 | 77648 | 79776 | 2128 | 3502080 |  |
| testSDListMemoryMode | yes | 3661824 | 3661824 | 0 | 79776 | 79872 | 96 | 3502080 |  |
| testSDListFileIOMode | yes | 3661824 | 3854336 | 192512 | 79872 | 81120 | 1248 | 3694592 |  |
| testCustomStringBehavior | yes | 3854336 | 3854336 | 0 | 81120 | 81120 | 0 | 3694592 |  |
| testJSONRoundTrip | yes | 3854336 | 3989504 | 135168 | 81120 | 82592 | 1472 | 3825664 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3989504 | 3997696 | 8192 | 82592 | 85728 | 3136 | 3825664 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3997696 | 4100096 | 102400 | 85728 | 87264 | 1536 | 3956736 |  |
| testSimpleVectorLifecycleStress | yes | 4100096 | 4100096 | 0 | 87264 | 87264 | 0 | 3956736 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4100096 | 4100096 | 0 | 89328 | 89328 | 0 | 3956736 |  |
| testJSONFileRoundTrip | yes | 4100096 | 4100096 | 0 | 89328 | 89808 | 480 | 3956736 |  |
| testSDListPersistenceStress | yes | 4100096 | 4100096 | 0 | 89760 | 90128 | 368 | 3956736 |  |
| testJSONPersistenceStress | yes | 4100096 | 4100096 | 0 | 90128 | 91168 | 1040 | 3956736 |  |
| testAVLTreeBasicBehavior | yes | 4100096 | 4100096 | 0 | 91168 | 91344 | 176 | 3956736 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4100096 | 4100096 | 0 | 91344 | 91504 | 160 | 3956736 |  |
| testPredicatesBasicBehavior | yes | 4100096 | 4100096 | 0 | 91504 | 91504 | 0 | 3956736 |  |
| testOperatorsBasicBehavior | yes | 4100096 | 4100096 | 0 | 91504 | 91504 | 0 | 3956736 |  |
| testDynamicStorageLibraryRamMode | yes | 4100096 | 4104192 | 4096 | 93552 | 93552 | 0 | 3956736 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4104192 | 4104192 | 0 | 93552 | 93552 | 0 | 3956736 |  |
| testSimpleMutexFallbackBehavior | yes | 4104192 | 4173824 | 69632 | 93552 | 93552 | 0 | 3956736 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4173824 | 4173824 | 0 | 93552 | 93552 | 0 | 3956736 |  |
| testSimpleRWLockFallbackBehavior | yes | 4173824 | 4173824 | 0 | 93552 | 93696 | 144 | 3956736 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4173824 | 4173824 | 0 | 93696 | 93696 | 0 | 3956736 |  |
| testAVLTreeErrorCodes | yes | 4173824 | 4173824 | 0 | 93696 | 93744 | 48 | 3956736 |  |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3620864 | 3620864 | 0 | 77040 | 77312 | 272 | 3538944 |  |
| testHashtableBasicBehavior | yes | 3620864 | 3629056 | 8192 | 77456 | 79648 | 2192 | 3538944 |  |
| testSDListMemoryMode | yes | 3629056 | 3629056 | 0 | 79648 | 79744 | 96 | 3538944 |  |
| testSDListFileIOMode | yes | 3629056 | 3821568 | 192512 | 79744 | 80848 | 1104 | 3731456 |  |
| testCustomStringBehavior | yes | 3821568 | 3821568 | 0 | 80848 | 80848 | 0 | 3731456 |  |
| testJSONRoundTrip | yes | 3821568 | 3956736 | 135168 | 80848 | 82320 | 1472 | 3862528 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3956736 | 3960832 | 4096 | 82320 | 85456 | 3136 | 3862528 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3960832 | 4067328 | 106496 | 85456 | 87120 | 1664 | 3993600 |  |
| testSimpleVectorLifecycleStress | yes | 4067328 | 4067328 | 0 | 87120 | 87120 | 0 | 3993600 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4067328 | 4067328 | 0 | 89184 | 89184 | 0 | 3993600 |  |
| testJSONFileRoundTrip | yes | 4067328 | 4067328 | 0 | 89184 | 89664 | 480 | 3993600 |  |
| testSDListPersistenceStress | yes | 4067328 | 4067328 | 0 | 89616 | 89680 | 64 | 3993600 |  |
| testJSONPersistenceStress | yes | 4067328 | 4067328 | 0 | 89680 | 90384 | 704 | 3993600 |  |
| testAVLTreeBasicBehavior | yes | 4067328 | 4067328 | 0 | 90384 | 90544 | 160 | 3993600 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4067328 | 4067328 | 0 | 90544 | 90784 | 240 | 3993600 |  |
| testPredicatesBasicBehavior | yes | 4067328 | 4067328 | 0 | 90784 | 90784 | 0 | 3993600 |  |
| testOperatorsBasicBehavior | yes | 4067328 | 4067328 | 0 | 90784 | 90784 | 0 | 3993600 |  |
| testDynamicStorageLibraryRamMode | yes | 4067328 | 4067328 | 0 | 92832 | 92832 | 0 | 3993600 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4067328 | 4067328 | 0 | 92832 | 92832 | 0 | 3993600 |  |
| testSimpleMutexFallbackBehavior | yes | 4067328 | 4136960 | 69632 | 92832 | 92832 | 0 | 3993600 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4136960 | 4136960 | 0 | 92832 | 92832 | 0 | 3993600 |  |
| testSimpleRWLockFallbackBehavior | yes | 4136960 | 4136960 | 0 | 92832 | 92976 | 144 | 3993600 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4136960 | 4136960 | 0 | 92976 | 93088 | 112 | 3993600 |  |
| testAVLTreeErrorCodes | yes | 4136960 | 4136960 | 0 | 93088 | 93136 | 48 | 3993600 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3612672 | 3612672 | 0 | 76928 | 77200 | 272 | 3461120 |  |
| testHashtableBasicBehavior | yes | 3612672 | 3620864 | 8192 | 77344 | 79536 | 2192 | 3461120 |  |
| testSDListMemoryMode | yes | 3620864 | 3620864 | 0 | 79536 | 79632 | 96 | 3461120 |  |
| testSDListFileIOMode | yes | 3620864 | 3813376 | 192512 | 79632 | 80752 | 1120 | 3653632 |  |
| testCustomStringBehavior | yes | 3813376 | 3813376 | 0 | 80752 | 80752 | 0 | 3653632 |  |
| testJSONRoundTrip | yes | 3813376 | 3948544 | 135168 | 80752 | 82272 | 1520 | 3784704 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3948544 | 3952640 | 4096 | 82272 | 85312 | 3040 | 3784704 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3952640 | 4059136 | 106496 | 85312 | 86832 | 1520 | 3915776 |  |
| testSimpleVectorLifecycleStress | yes | 4059136 | 4059136 | 0 | 86832 | 86832 | 0 | 3915776 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4059136 | 4059136 | 0 | 88896 | 88896 | 0 | 3915776 |  |
| testJSONFileRoundTrip | yes | 4059136 | 4059136 | 0 | 88896 | 89152 | 256 | 3915776 |  |
| testSDListPersistenceStress | yes | 4059136 | 4059136 | 0 | 89152 | 89504 | 352 | 3915776 |  |
| testJSONPersistenceStress | yes | 4059136 | 4059136 | 0 | 89504 | 90320 | 816 | 3915776 |  |
| testAVLTreeBasicBehavior | yes | 4059136 | 4059136 | 0 | 90320 | 90368 | 48 | 3915776 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4059136 | 4059136 | 0 | 90368 | 90464 | 96 | 3915776 |  |
| testPredicatesBasicBehavior | yes | 4059136 | 4059136 | 0 | 90464 | 90464 | 0 | 3915776 |  |
| testOperatorsBasicBehavior | yes | 4059136 | 4059136 | 0 | 90464 | 90464 | 0 | 3915776 |  |
| testDynamicStorageLibraryRamMode | yes | 4059136 | 4063232 | 4096 | 92512 | 92512 | 0 | 3915776 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4063232 | 4063232 | 0 | 92512 | 92512 | 0 | 3915776 |  |
| testSimpleMutexFallbackBehavior | yes | 4063232 | 4132864 | 69632 | 92512 | 92512 | 0 | 4050944 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92512 | 92512 | 0 | 4050944 |  |
| testSimpleRWLockFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92512 | 92704 | 192 | 4050944 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92704 | 92816 | 112 | 4050944 |  |

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

- Boards profiled: 7
- Total runs processed: 7
- Instance probe types: 12
- Element fill probe types: 25

## Understanding

- Counts shown are the **minimum** across all run variants for that board (most conservative).
- Heap delta from baseline is used for measurement (not absolute process RSS).
- ✓ means the probe stopped because the configured budget was reached.
- A count equal to the configured cap (HOST_STRESS_MAX_INSTANCES / HOST_STRESS_MAX_ELEMENTS) means the probe finished without hitting the budget.
- Budget is taken from each run's `limitBytes` field (CI typically sets it to `sramBytes × 1024`).

## Instance Count Probes

_How many simultaneously-alive empty instances fit within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList_int | SimpleVector_int | Hashtable_int_int | JSON | AVLTree_int | DynamicStorage_String_int | Predicates_int | Operators_int | Stack_int | Queue_int | LinkedList_int | OrderedMap_int_int |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| due | 98304 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| esp8266 | 81920 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| mega2560 | 8192 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| micro | 2560 | 20000 | 20000 | 14161 ✓ | 9745 ✓ | 20000 | 8001 ✓ | 20000 | 20000 | 20000 | 20000 | 20000 | 12977 ✓ |
| uno | 2048 | 20000 | 20000 | 11185 ✓ | 7953 ✓ | 20000 | 6353 ✓ | 20000 | 20000 | 20000 | 20000 | 20000 | 10257 ✓ |
| ventunoq | 65536 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| zero | 32768 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |

✓ = budget reached during probe (count is the boundary value)

## Element Fill Probes

_How many elements fit in a single container instance within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList[int] | ArrayList[float] | ArrayList[double] | ArrayList[String] | SimpleVector[int] | SimpleVector[float] | SimpleVector[double] | Hashtable[int_int] | Hashtable[String_String] | AVLTree[int] | DynamicStorage[String_int] | Stack[int] | Stack[float] | Stack[double] | Stack[String] | Queue[int] | Queue[float] | Queue[double] | Queue[String] | LinkedList[int] | LinkedList[float] | LinkedList[double] | LinkedList[String] | OrderedMap[int_int] | OrderedMap[String_String] |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| due | 98304 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 |
| esp8266 | 81920 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 |
| mega2560 | 8192 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 |
| micro | 2560 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 49153 ✓ | 26369 ✓ | 50000 | 21249 ✓ | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 40961 ✓ | 50000 | 50000 | 50000 | 50000 | 50000 | 39425 ✓ |
| uno | 2048 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 46081 ✓ | 23041 ✓ | 43777 ✓ | 16641 ✓ | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 50000 | 40961 ✓ | 50000 | 50000 | 50000 | 43777 ✓ | 50000 | 26369 ✓ |
| ventunoq | 65536 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 |
| zero | 32768 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 | 200000 |

✓ = budget reached during probe (count is the boundary value)

## Per-Board Details

### due

- SRAM: 98304 bytes
- Host budget: 100663296 bytes (98304 × 1024)

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
