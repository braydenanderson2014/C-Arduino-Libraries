# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4091904
- Avg peak bytes: 4027392
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
| optional-littlefs | yes | LittleFS | 4026368 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-sd | yes | SD | 4091904 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 3969024 | 8388608 | no | yes |  | 0 | 0 | 0 |

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
| testArrayListBasicBehavior | yes | 3653632 | 3653632 | 0 | 77232 | 77504 | 272 | 3571712 |  |
| testHashtableBasicBehavior | yes | 3653632 | 3661824 | 8192 | 77648 | 79776 | 2128 | 3571712 |  |
| testSDListMemoryMode | yes | 3661824 | 3661824 | 0 | 79776 | 79872 | 96 | 3571712 |  |
| testSDListFileIOMode | yes | 3661824 | 3854336 | 192512 | 79872 | 81120 | 1248 | 3764224 |  |
| testCustomStringBehavior | yes | 3854336 | 3854336 | 0 | 81120 | 81120 | 0 | 3764224 |  |
| testJSONRoundTrip | yes | 3854336 | 3989504 | 135168 | 81120 | 82592 | 1472 | 3895296 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3989504 | 3997696 | 8192 | 82592 | 85728 | 3136 | 3895296 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3997696 | 4100096 | 102400 | 85728 | 87264 | 1536 | 4026368 |  |
| testSimpleVectorLifecycleStress | yes | 4100096 | 4100096 | 0 | 87264 | 87264 | 0 | 4026368 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4100096 | 4100096 | 0 | 89328 | 89328 | 0 | 4026368 |  |
| testJSONFileRoundTrip | yes | 4100096 | 4100096 | 0 | 89328 | 89808 | 480 | 4026368 |  |
| testSDListPersistenceStress | yes | 4100096 | 4100096 | 0 | 89760 | 90128 | 368 | 4026368 |  |
| testJSONPersistenceStress | yes | 4100096 | 4100096 | 0 | 90128 | 91168 | 1040 | 4026368 |  |
| testAVLTreeBasicBehavior | yes | 4100096 | 4100096 | 0 | 91168 | 91344 | 176 | 4026368 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4100096 | 4100096 | 0 | 91344 | 91504 | 160 | 4026368 |  |
| testPredicatesBasicBehavior | yes | 4100096 | 4100096 | 0 | 91504 | 91504 | 0 | 4026368 |  |
| testOperatorsBasicBehavior | yes | 4100096 | 4100096 | 0 | 91504 | 91504 | 0 | 4026368 |  |
| testDynamicStorageLibraryRamMode | yes | 4100096 | 4100096 | 0 | 93552 | 93552 | 0 | 4026368 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4100096 | 4100096 | 0 | 93552 | 93552 | 0 | 4026368 |  |
| testSimpleMutexFallbackBehavior | yes | 4100096 | 4169728 | 69632 | 93552 | 93552 | 0 | 4026368 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4169728 | 4169728 | 0 | 93552 | 93552 | 0 | 4026368 |  |
| testSimpleRWLockFallbackBehavior | yes | 4169728 | 4169728 | 0 | 93552 | 93696 | 144 | 4026368 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4169728 | 4169728 | 0 | 93696 | 93696 | 0 | 4026368 |  |
| testAVLTreeErrorCodes | yes | 4169728 | 4169728 | 0 | 93696 | 93744 | 48 | 4026368 |  |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3649536 | 3649536 | 0 | 77040 | 77312 | 272 | 3502080 |  |
| testHashtableBasicBehavior | yes | 3649536 | 3657728 | 8192 | 77456 | 79648 | 2192 | 3502080 |  |
| testSDListMemoryMode | yes | 3657728 | 3657728 | 0 | 79648 | 79744 | 96 | 3502080 |  |
| testSDListFileIOMode | yes | 3657728 | 3850240 | 192512 | 79744 | 80848 | 1104 | 3694592 |  |
| testCustomStringBehavior | yes | 3850240 | 3850240 | 0 | 80848 | 80848 | 0 | 3694592 |  |
| testJSONRoundTrip | yes | 3850240 | 3985408 | 135168 | 80848 | 82320 | 1472 | 3825664 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3985408 | 3989504 | 4096 | 82320 | 85456 | 3136 | 3825664 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3989504 | 4096000 | 106496 | 85456 | 87120 | 1664 | 3956736 |  |
| testSimpleVectorLifecycleStress | yes | 4096000 | 4096000 | 0 | 87120 | 87120 | 0 | 3956736 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4096000 | 4096000 | 0 | 89184 | 89184 | 0 | 3956736 |  |
| testJSONFileRoundTrip | yes | 4096000 | 4096000 | 0 | 89184 | 89664 | 480 | 3956736 |  |
| testSDListPersistenceStress | yes | 4096000 | 4096000 | 0 | 89616 | 89680 | 64 | 3956736 |  |
| testJSONPersistenceStress | yes | 4096000 | 4096000 | 0 | 89680 | 90384 | 704 | 3956736 |  |
| testAVLTreeBasicBehavior | yes | 4096000 | 4096000 | 0 | 90384 | 90544 | 160 | 3956736 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4096000 | 4096000 | 0 | 90544 | 90784 | 240 | 3956736 |  |
| testPredicatesBasicBehavior | yes | 4096000 | 4096000 | 0 | 90784 | 90784 | 0 | 3956736 |  |
| testOperatorsBasicBehavior | yes | 4096000 | 4096000 | 0 | 90784 | 90784 | 0 | 3956736 |  |
| testDynamicStorageLibraryRamMode | yes | 4096000 | 4100096 | 4096 | 92832 | 92832 | 0 | 3956736 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4100096 | 4100096 | 0 | 92832 | 92832 | 0 | 3956736 |  |
| testSimpleMutexFallbackBehavior | yes | 4100096 | 4169728 | 69632 | 92832 | 92832 | 0 | 4091904 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92832 | 92832 | 0 | 4091904 |  |
| testSimpleRWLockFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92832 | 92976 | 144 | 4091904 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92976 | 93088 | 112 | 4091904 |  |
| testAVLTreeErrorCodes | yes | 4169728 | 4169728 | 0 | 93088 | 93136 | 48 | 4091904 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3596288 | 3596288 | 0 | 76928 | 77200 | 272 | 3514368 |  |
| testHashtableBasicBehavior | yes | 3596288 | 3604480 | 8192 | 77344 | 79536 | 2192 | 3514368 |  |
| testSDListMemoryMode | yes | 3604480 | 3604480 | 0 | 79536 | 79632 | 96 | 3514368 |  |
| testSDListFileIOMode | yes | 3604480 | 3796992 | 192512 | 79632 | 80752 | 1120 | 3706880 |  |
| testCustomStringBehavior | yes | 3796992 | 3796992 | 0 | 80752 | 80752 | 0 | 3706880 |  |
| testJSONRoundTrip | yes | 3796992 | 3932160 | 135168 | 80752 | 82272 | 1520 | 3837952 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3932160 | 3936256 | 4096 | 82272 | 85312 | 3040 | 3837952 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3936256 | 4042752 | 106496 | 85312 | 86832 | 1520 | 3969024 |  |
| testSimpleVectorLifecycleStress | yes | 4042752 | 4042752 | 0 | 86832 | 86832 | 0 | 3969024 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4042752 | 4042752 | 0 | 88896 | 88896 | 0 | 3969024 |  |
| testJSONFileRoundTrip | yes | 4042752 | 4042752 | 0 | 88896 | 89152 | 256 | 3969024 |  |
| testSDListPersistenceStress | yes | 4042752 | 4042752 | 0 | 89152 | 89504 | 352 | 3969024 |  |
| testJSONPersistenceStress | yes | 4042752 | 4042752 | 0 | 89504 | 90320 | 816 | 3969024 |  |
| testAVLTreeBasicBehavior | yes | 4042752 | 4042752 | 0 | 90320 | 90368 | 48 | 3969024 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4042752 | 4042752 | 0 | 90368 | 90464 | 96 | 3969024 |  |
| testPredicatesBasicBehavior | yes | 4042752 | 4042752 | 0 | 90464 | 90464 | 0 | 3969024 |  |
| testOperatorsBasicBehavior | yes | 4042752 | 4042752 | 0 | 90464 | 90464 | 0 | 3969024 |  |
| testDynamicStorageLibraryRamMode | yes | 4042752 | 4042752 | 0 | 92512 | 92512 | 0 | 3969024 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4042752 | 4042752 | 0 | 92512 | 92512 | 0 | 3969024 |  |
| testSimpleMutexFallbackBehavior | yes | 4042752 | 4112384 | 69632 | 92512 | 92512 | 0 | 3969024 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4112384 | 4112384 | 0 | 92512 | 92512 | 0 | 3969024 |  |
| testSimpleRWLockFallbackBehavior | yes | 4112384 | 4112384 | 0 | 92512 | 92704 | 192 | 3969024 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4112384 | 4112384 | 0 | 92704 | 92816 | 112 | 3969024 |  |

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
