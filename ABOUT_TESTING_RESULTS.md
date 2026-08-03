# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4087808
- Avg peak bytes: 4025344
- Compile smoke objects found: 62
- Experimental compile results found: 25
- Experimental compile successes: 24
- Experimental compile failures: 1
- Experimental libraries covered: 14
- Experimental backends covered: 2
- Experimental optional modes covered: 13
- Memory profile runs: 0
- Runs that exceeded limit: 0
- Runs with limit enforcement enabled: 4
- Runs with capacity probe enabled: 0
- Runs where capacity probe reached limit: 0
- Runs with first limit-crossing test identified: 0
- Expected compile smoke objects: 50
- Expected experimental compile results: 13

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
| littlefs | yes | LittleFS | 4087808 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-littlefs | yes | LittleFS | 3969024 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-sd | yes | SD | 3993600 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4050944 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3645440 | 3645440 | 0 | 77056 | 77328 | 272 | 3493888 |  |
| testHashtableBasicBehavior | yes | 3645440 | 3653632 | 8192 | 77472 | 79664 | 2192 | 3493888 |  |
| testSDListMemoryMode | yes | 3653632 | 3653632 | 0 | 79664 | 79760 | 96 | 3493888 |  |
| testSDListFileIOMode | yes | 3653632 | 3846144 | 192512 | 79760 | 81008 | 1248 | 3686400 |  |
| testCustomStringBehavior | yes | 3846144 | 3846144 | 0 | 81008 | 81008 | 0 | 3686400 |  |
| testJSONRoundTrip | yes | 3846144 | 3981312 | 135168 | 81008 | 82480 | 1472 | 3817472 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3981312 | 3985408 | 4096 | 82480 | 85600 | 3120 | 3817472 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3985408 | 4091904 | 106496 | 85600 | 87040 | 1440 | 3948544 |  |
| testSimpleVectorLifecycleStress | yes | 4091904 | 4091904 | 0 | 87040 | 87040 | 0 | 3948544 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4091904 | 4091904 | 0 | 89104 | 89152 | 48 | 3948544 |  |
| testJSONFileRoundTrip | yes | 4091904 | 4091904 | 0 | 89152 | 89472 | 320 | 3948544 |  |
| testSDListPersistenceStress | yes | 4091904 | 4091904 | 0 | 89472 | 89760 | 288 | 3948544 |  |
| testJSONPersistenceStress | yes | 4091904 | 4091904 | 0 | 89760 | 90576 | 816 | 3948544 |  |
| testAVLTreeBasicBehavior | yes | 4091904 | 4091904 | 0 | 90576 | 90672 | 96 | 3948544 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4091904 | 4091904 | 0 | 90672 | 90720 | 48 | 3948544 |  |
| testPredicatesBasicBehavior | yes | 4091904 | 4091904 | 0 | 90720 | 90720 | 0 | 3948544 |  |
| testOperatorsBasicBehavior | yes | 4091904 | 4091904 | 0 | 90720 | 90720 | 0 | 3948544 |  |
| testDynamicStorageLibraryRamMode | yes | 4091904 | 4100096 | 8192 | 92768 | 92768 | 0 | 3948544 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4100096 | 4100096 | 0 | 92768 | 92768 | 0 | 3948544 |  |
| testSimpleMutexFallbackBehavior | yes | 4100096 | 4169728 | 69632 | 92768 | 92768 | 0 | 4087808 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92768 | 92768 | 0 | 4087808 |  |
| testSimpleRWLockFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92768 | 92912 | 144 | 4087808 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92912 | 92912 | 0 | 4087808 |  |

### Per-test Memory Stats: optional-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3600384 | 3600384 | 0 | 77232 | 77504 | 272 | 3514368 |  |
| testHashtableBasicBehavior | yes | 3600384 | 3608576 | 8192 | 77648 | 79776 | 2128 | 3514368 |  |
| testSDListMemoryMode | yes | 3608576 | 3608576 | 0 | 79776 | 79872 | 96 | 3514368 |  |
| testSDListFileIOMode | yes | 3608576 | 3801088 | 192512 | 79872 | 81120 | 1248 | 3706880 |  |
| testCustomStringBehavior | yes | 3801088 | 3801088 | 0 | 81120 | 81120 | 0 | 3706880 |  |
| testJSONRoundTrip | yes | 3801088 | 3936256 | 135168 | 81120 | 82592 | 1472 | 3837952 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3936256 | 3944448 | 8192 | 82592 | 85728 | 3136 | 3837952 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3944448 | 4046848 | 102400 | 85728 | 87264 | 1536 | 3969024 |  |
| testSimpleVectorLifecycleStress | yes | 4046848 | 4046848 | 0 | 87264 | 87264 | 0 | 3969024 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4046848 | 4046848 | 0 | 89328 | 89328 | 0 | 3969024 |  |
| testJSONFileRoundTrip | yes | 4046848 | 4046848 | 0 | 89328 | 89808 | 480 | 3969024 |  |
| testSDListPersistenceStress | yes | 4046848 | 4046848 | 0 | 89760 | 90128 | 368 | 3969024 |  |
| testJSONPersistenceStress | yes | 4046848 | 4046848 | 0 | 90128 | 91168 | 1040 | 3969024 |  |
| testAVLTreeBasicBehavior | yes | 4046848 | 4046848 | 0 | 91168 | 91344 | 176 | 3969024 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4046848 | 4046848 | 0 | 91344 | 91504 | 160 | 3969024 |  |
| testPredicatesBasicBehavior | yes | 4046848 | 4046848 | 0 | 91504 | 91504 | 0 | 3969024 |  |
| testOperatorsBasicBehavior | yes | 4046848 | 4046848 | 0 | 91504 | 91504 | 0 | 3969024 |  |
| testDynamicStorageLibraryRamMode | yes | 4046848 | 4046848 | 0 | 93552 | 93552 | 0 | 3969024 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4046848 | 4046848 | 0 | 93552 | 93552 | 0 | 3969024 |  |
| testSimpleMutexFallbackBehavior | yes | 4046848 | 4116480 | 69632 | 93552 | 93552 | 0 | 3969024 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4116480 | 4116480 | 0 | 93552 | 93552 | 0 | 3969024 |  |
| testSimpleRWLockFallbackBehavior | yes | 4116480 | 4116480 | 0 | 93552 | 93696 | 144 | 3969024 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4116480 | 4116480 | 0 | 93696 | 93696 | 0 | 3969024 |  |
| testAVLTreeErrorCodes | yes | 4116480 | 4116480 | 0 | 93696 | 93744 | 48 | 3969024 |  |

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
| testArrayListBasicBehavior | yes | 3612672 | 3612672 | 0 | 76928 | 77200 | 272 | 3465216 |  |
| testHashtableBasicBehavior | yes | 3612672 | 3620864 | 8192 | 77344 | 79536 | 2192 | 3465216 |  |
| testSDListMemoryMode | yes | 3620864 | 3620864 | 0 | 79536 | 79632 | 96 | 3465216 |  |
| testSDListFileIOMode | yes | 3620864 | 3813376 | 192512 | 79632 | 80752 | 1120 | 3657728 |  |
| testCustomStringBehavior | yes | 3813376 | 3813376 | 0 | 80752 | 80752 | 0 | 3657728 |  |
| testJSONRoundTrip | yes | 3813376 | 3948544 | 135168 | 80752 | 82272 | 1520 | 3788800 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3948544 | 3952640 | 4096 | 82272 | 85312 | 3040 | 3788800 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3952640 | 4059136 | 106496 | 85312 | 86832 | 1520 | 3919872 |  |
| testSimpleVectorLifecycleStress | yes | 4059136 | 4059136 | 0 | 86832 | 86832 | 0 | 3919872 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4059136 | 4059136 | 0 | 88896 | 88896 | 0 | 3919872 |  |
| testJSONFileRoundTrip | yes | 4059136 | 4059136 | 0 | 88896 | 89152 | 256 | 3919872 |  |
| testSDListPersistenceStress | yes | 4059136 | 4059136 | 0 | 89152 | 89504 | 352 | 3919872 |  |
| testJSONPersistenceStress | yes | 4059136 | 4059136 | 0 | 89504 | 90320 | 816 | 3919872 |  |
| testAVLTreeBasicBehavior | yes | 4059136 | 4059136 | 0 | 90320 | 90368 | 48 | 3919872 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4059136 | 4059136 | 0 | 90368 | 90464 | 96 | 3919872 |  |
| testPredicatesBasicBehavior | yes | 4059136 | 4059136 | 0 | 90464 | 90464 | 0 | 3919872 |  |
| testOperatorsBasicBehavior | yes | 4059136 | 4059136 | 0 | 90464 | 90464 | 0 | 3919872 |  |
| testDynamicStorageLibraryRamMode | yes | 4059136 | 4059136 | 0 | 92512 | 92512 | 0 | 3919872 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4059136 | 4059136 | 0 | 92512 | 92512 | 0 | 3919872 |  |
| testSimpleMutexFallbackBehavior | yes | 4059136 | 4128768 | 69632 | 92512 | 92512 | 0 | 4050944 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92512 | 92512 | 0 | 4050944 |  |
| testSimpleRWLockFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92512 | 92704 | 192 | 4050944 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92704 | 92816 | 112 | 4050944 |  |

## Experimental Compile Results

_Each row is one experimental compile matrix entry (library + backend + optional mode)._

| Library | Backend | Optional | Success | ReturnCode | Artifact |
| --- | --- | --- | --- | ---: | --- |
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
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/DynamicStorageLibrary | sd | off | no | 1 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_DynamicStorageLibrary/sd/off |
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
