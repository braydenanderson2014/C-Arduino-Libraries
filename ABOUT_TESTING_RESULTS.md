# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4096000
- Avg peak bytes: 4030464
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
| littlefs | yes | LittleFS | 4050944 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-littlefs | yes | LittleFS | 3964928 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-sd | yes | SD | 4096000 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4009984 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3612672 | 3612672 | 0 | 77056 | 77328 | 272 | 3461120 |  |
| testHashtableBasicBehavior | yes | 3612672 | 3620864 | 8192 | 77472 | 79664 | 2192 | 3461120 |  |
| testSDListMemoryMode | yes | 3620864 | 3620864 | 0 | 79664 | 79760 | 96 | 3461120 |  |
| testSDListFileIOMode | yes | 3620864 | 3813376 | 192512 | 79760 | 81008 | 1248 | 3653632 |  |
| testCustomStringBehavior | yes | 3813376 | 3813376 | 0 | 81008 | 81008 | 0 | 3653632 |  |
| testJSONRoundTrip | yes | 3813376 | 3948544 | 135168 | 81008 | 82480 | 1472 | 3784704 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3948544 | 3952640 | 4096 | 82480 | 85600 | 3120 | 3784704 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3952640 | 4059136 | 106496 | 85600 | 87040 | 1440 | 3915776 |  |
| testSimpleVectorLifecycleStress | yes | 4059136 | 4059136 | 0 | 87040 | 87040 | 0 | 3915776 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4059136 | 4059136 | 0 | 89104 | 89152 | 48 | 3915776 |  |
| testJSONFileRoundTrip | yes | 4059136 | 4059136 | 0 | 89152 | 89472 | 320 | 3915776 |  |
| testSDListPersistenceStress | yes | 4059136 | 4059136 | 0 | 89472 | 89760 | 288 | 3915776 |  |
| testJSONPersistenceStress | yes | 4059136 | 4059136 | 0 | 89760 | 90576 | 816 | 3915776 |  |
| testAVLTreeBasicBehavior | yes | 4059136 | 4059136 | 0 | 90576 | 90672 | 96 | 3915776 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4059136 | 4059136 | 0 | 90672 | 90720 | 48 | 3915776 |  |
| testPredicatesBasicBehavior | yes | 4059136 | 4059136 | 0 | 90720 | 90720 | 0 | 3915776 |  |
| testOperatorsBasicBehavior | yes | 4059136 | 4059136 | 0 | 90720 | 90720 | 0 | 3915776 |  |
| testDynamicStorageLibraryRamMode | yes | 4059136 | 4063232 | 4096 | 92768 | 92768 | 0 | 3915776 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4063232 | 4063232 | 0 | 92768 | 92768 | 0 | 3915776 |  |
| testSimpleMutexFallbackBehavior | yes | 4063232 | 4132864 | 69632 | 92768 | 92768 | 0 | 4050944 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92768 | 92768 | 0 | 4050944 |  |
| testSimpleRWLockFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92768 | 92912 | 144 | 4050944 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92912 | 92912 | 0 | 4050944 |  |

### Per-test Memory Stats: optional-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3596288 | 3596288 | 0 | 77232 | 77504 | 272 | 3510272 |  |
| testHashtableBasicBehavior | yes | 3596288 | 3604480 | 8192 | 77648 | 79776 | 2128 | 3510272 |  |
| testSDListMemoryMode | yes | 3604480 | 3604480 | 0 | 79776 | 79872 | 96 | 3510272 |  |
| testSDListFileIOMode | yes | 3604480 | 3796992 | 192512 | 79872 | 81120 | 1248 | 3702784 |  |
| testCustomStringBehavior | yes | 3796992 | 3796992 | 0 | 81120 | 81120 | 0 | 3702784 |  |
| testJSONRoundTrip | yes | 3796992 | 3932160 | 135168 | 81120 | 82592 | 1472 | 3833856 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3932160 | 3940352 | 8192 | 82592 | 85728 | 3136 | 3833856 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3940352 | 4042752 | 102400 | 85728 | 87264 | 1536 | 3964928 |  |
| testSimpleVectorLifecycleStress | yes | 4042752 | 4042752 | 0 | 87264 | 87264 | 0 | 3964928 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4042752 | 4042752 | 0 | 89328 | 89328 | 0 | 3964928 |  |
| testJSONFileRoundTrip | yes | 4042752 | 4042752 | 0 | 89328 | 89808 | 480 | 3964928 |  |
| testSDListPersistenceStress | yes | 4042752 | 4042752 | 0 | 89760 | 90128 | 368 | 3964928 |  |
| testJSONPersistenceStress | yes | 4042752 | 4042752 | 0 | 90128 | 91168 | 1040 | 3964928 |  |
| testAVLTreeBasicBehavior | yes | 4042752 | 4042752 | 0 | 91168 | 91344 | 176 | 3964928 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4042752 | 4042752 | 0 | 91344 | 91504 | 160 | 3964928 |  |
| testPredicatesBasicBehavior | yes | 4042752 | 4042752 | 0 | 91504 | 91504 | 0 | 3964928 |  |
| testOperatorsBasicBehavior | yes | 4042752 | 4042752 | 0 | 91504 | 91504 | 0 | 3964928 |  |
| testDynamicStorageLibraryRamMode | yes | 4042752 | 4042752 | 0 | 93552 | 93552 | 0 | 3964928 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4042752 | 4042752 | 0 | 93552 | 93552 | 0 | 3964928 |  |
| testSimpleMutexFallbackBehavior | yes | 4042752 | 4112384 | 69632 | 93552 | 93552 | 0 | 3964928 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4112384 | 4112384 | 0 | 93552 | 93552 | 0 | 3964928 |  |
| testSimpleRWLockFallbackBehavior | yes | 4112384 | 4112384 | 0 | 93552 | 93696 | 144 | 3964928 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4112384 | 4112384 | 0 | 93696 | 93696 | 0 | 3964928 |  |
| testAVLTreeErrorCodes | yes | 4112384 | 4112384 | 0 | 93696 | 93744 | 48 | 3964928 |  |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3657728 | 3657728 | 0 | 77040 | 77312 | 272 | 3510272 |  |
| testHashtableBasicBehavior | yes | 3657728 | 3665920 | 8192 | 77456 | 79648 | 2192 | 3510272 |  |
| testSDListMemoryMode | yes | 3665920 | 3665920 | 0 | 79648 | 79744 | 96 | 3510272 |  |
| testSDListFileIOMode | yes | 3665920 | 3858432 | 192512 | 79744 | 80848 | 1104 | 3702784 |  |
| testCustomStringBehavior | yes | 3858432 | 3858432 | 0 | 80848 | 80848 | 0 | 3702784 |  |
| testJSONRoundTrip | yes | 3858432 | 3993600 | 135168 | 80848 | 82320 | 1472 | 3833856 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3993600 | 3997696 | 4096 | 82320 | 85456 | 3136 | 3833856 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3997696 | 4104192 | 106496 | 85456 | 87120 | 1664 | 3964928 |  |
| testSimpleVectorLifecycleStress | yes | 4104192 | 4104192 | 0 | 87120 | 87120 | 0 | 3964928 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4104192 | 4104192 | 0 | 89184 | 89184 | 0 | 3964928 |  |
| testJSONFileRoundTrip | yes | 4104192 | 4104192 | 0 | 89184 | 89664 | 480 | 3964928 |  |
| testSDListPersistenceStress | yes | 4104192 | 4104192 | 0 | 89616 | 89680 | 64 | 3964928 |  |
| testJSONPersistenceStress | yes | 4104192 | 4104192 | 0 | 89680 | 90384 | 704 | 3964928 |  |
| testAVLTreeBasicBehavior | yes | 4104192 | 4104192 | 0 | 90384 | 90544 | 160 | 3964928 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4104192 | 4104192 | 0 | 90544 | 90784 | 240 | 3964928 |  |
| testPredicatesBasicBehavior | yes | 4104192 | 4104192 | 0 | 90784 | 90784 | 0 | 3964928 |  |
| testOperatorsBasicBehavior | yes | 4104192 | 4104192 | 0 | 90784 | 90784 | 0 | 3964928 |  |
| testDynamicStorageLibraryRamMode | yes | 4104192 | 4104192 | 0 | 92832 | 92832 | 0 | 3964928 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4104192 | 4104192 | 0 | 92832 | 92832 | 0 | 3964928 |  |
| testSimpleMutexFallbackBehavior | yes | 4104192 | 4173824 | 69632 | 92832 | 92832 | 0 | 4096000 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92832 | 92832 | 0 | 4096000 |  |
| testSimpleRWLockFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92832 | 92976 | 144 | 4096000 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92976 | 93088 | 112 | 4096000 |  |
| testAVLTreeErrorCodes | yes | 4173824 | 4173824 | 0 | 93088 | 93136 | 48 | 4096000 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3641344 | 3641344 | 0 | 76928 | 77200 | 272 | 3555328 |  |
| testHashtableBasicBehavior | yes | 3641344 | 3649536 | 8192 | 77344 | 79536 | 2192 | 3555328 |  |
| testSDListMemoryMode | yes | 3649536 | 3649536 | 0 | 79536 | 79632 | 96 | 3555328 |  |
| testSDListFileIOMode | yes | 3649536 | 3842048 | 192512 | 79632 | 80752 | 1120 | 3747840 |  |
| testCustomStringBehavior | yes | 3842048 | 3842048 | 0 | 80752 | 80752 | 0 | 3747840 |  |
| testJSONRoundTrip | yes | 3842048 | 3977216 | 135168 | 80752 | 82272 | 1520 | 3878912 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3977216 | 3981312 | 4096 | 82272 | 85312 | 3040 | 3878912 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3981312 | 4087808 | 106496 | 85312 | 86832 | 1520 | 4009984 |  |
| testSimpleVectorLifecycleStress | yes | 4087808 | 4087808 | 0 | 86832 | 86832 | 0 | 4009984 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4087808 | 4087808 | 0 | 88896 | 88896 | 0 | 4009984 |  |
| testJSONFileRoundTrip | yes | 4087808 | 4087808 | 0 | 88896 | 89152 | 256 | 4009984 |  |
| testSDListPersistenceStress | yes | 4087808 | 4087808 | 0 | 89152 | 89504 | 352 | 4009984 |  |
| testJSONPersistenceStress | yes | 4087808 | 4087808 | 0 | 89504 | 90320 | 816 | 4009984 |  |
| testAVLTreeBasicBehavior | yes | 4087808 | 4087808 | 0 | 90320 | 90368 | 48 | 4009984 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4087808 | 4087808 | 0 | 90368 | 90464 | 96 | 4009984 |  |
| testPredicatesBasicBehavior | yes | 4087808 | 4087808 | 0 | 90464 | 90464 | 0 | 4009984 |  |
| testOperatorsBasicBehavior | yes | 4087808 | 4087808 | 0 | 90464 | 90464 | 0 | 4009984 |  |
| testDynamicStorageLibraryRamMode | yes | 4087808 | 4087808 | 0 | 92512 | 92512 | 0 | 4009984 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4087808 | 4087808 | 0 | 92512 | 92512 | 0 | 4009984 |  |
| testSimpleMutexFallbackBehavior | yes | 4087808 | 4157440 | 69632 | 92512 | 92512 | 0 | 4009984 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4157440 | 4157440 | 0 | 92512 | 92512 | 0 | 4009984 |  |
| testSimpleRWLockFallbackBehavior | yes | 4157440 | 4157440 | 0 | 92512 | 92704 | 192 | 4009984 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4157440 | 4157440 | 0 | 92704 | 92816 | 112 | 4009984 |  |

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
