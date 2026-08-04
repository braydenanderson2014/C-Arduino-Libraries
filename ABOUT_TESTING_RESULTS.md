# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4403200
- Avg peak bytes: 4347904
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
- Stress test runs loaded: 20

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
| littlefs | yes | LittleFS | 4403200 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-littlefs | yes | LittleFS | 4341760 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-sd | yes | SD | 4345856 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4300800 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3956736 | 3956736 | 0 | 77056 | 77328 | 272 | 3874816 |  |
| testHashtableBasicBehavior | yes | 3956736 | 3964928 | 8192 | 77472 | 79664 | 2192 | 3874816 |  |
| testSDListMemoryMode | yes | 3964928 | 3964928 | 0 | 79664 | 79760 | 96 | 3874816 |  |
| testSDListFileIOMode | yes | 3964928 | 4161536 | 196608 | 79760 | 81008 | 1248 | 4005888 |  |
| testCustomStringBehavior | yes | 4161536 | 4161536 | 0 | 81008 | 81008 | 0 | 4005888 |  |
| testJSONRoundTrip | yes | 4161536 | 4296704 | 135168 | 81008 | 82480 | 1472 | 4136960 |  |
| testArrayListChurnAndBoundaryHealth | yes | 4296704 | 4300800 | 4096 | 82480 | 85600 | 3120 | 4136960 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4300800 | 4407296 | 106496 | 85600 | 87040 | 1440 | 4268032 |  |
| testSimpleVectorLifecycleStress | yes | 4407296 | 4407296 | 0 | 87040 | 87040 | 0 | 4268032 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4407296 | 4407296 | 0 | 89104 | 89152 | 48 | 4268032 |  |
| testJSONFileRoundTrip | yes | 4407296 | 4407296 | 0 | 89152 | 89472 | 320 | 4268032 |  |
| testSDListPersistenceStress | yes | 4407296 | 4407296 | 0 | 89472 | 89760 | 288 | 4268032 |  |
| testJSONPersistenceStress | yes | 4407296 | 4407296 | 0 | 89760 | 90576 | 816 | 4268032 |  |
| testAVLTreeBasicBehavior | yes | 4407296 | 4407296 | 0 | 90576 | 90672 | 96 | 4268032 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4407296 | 4407296 | 0 | 90672 | 90720 | 48 | 4268032 |  |
| testPredicatesBasicBehavior | yes | 4407296 | 4407296 | 0 | 90720 | 90720 | 0 | 4268032 |  |
| testOperatorsBasicBehavior | yes | 4407296 | 4407296 | 0 | 90720 | 90720 | 0 | 4268032 |  |
| testDynamicStorageLibraryRamMode | yes | 4407296 | 4411392 | 4096 | 92768 | 92768 | 0 | 4268032 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4411392 | 4411392 | 0 | 92768 | 92768 | 0 | 4268032 |  |
| testSimpleMutexFallbackBehavior | yes | 4411392 | 4481024 | 69632 | 92768 | 92768 | 0 | 4403200 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4481024 | 4481024 | 0 | 92768 | 92768 | 0 | 4403200 |  |
| testSimpleRWLockFallbackBehavior | yes | 4481024 | 4481024 | 0 | 92768 | 92912 | 144 | 4403200 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4481024 | 4481024 | 0 | 92912 | 92912 | 0 | 4403200 |  |

### Per-test Memory Stats: optional-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3969024 | 3969024 | 0 | 77232 | 77504 | 272 | 3817472 |  |
| testHashtableBasicBehavior | yes | 3969024 | 3977216 | 8192 | 77648 | 79776 | 2128 | 3817472 |  |
| testSDListMemoryMode | yes | 3977216 | 3977216 | 0 | 79776 | 79872 | 96 | 3817472 |  |
| testSDListFileIOMode | yes | 3977216 | 4173824 | 196608 | 79872 | 81120 | 1248 | 4079616 |  |
| testCustomStringBehavior | yes | 4173824 | 4173824 | 0 | 81120 | 81120 | 0 | 4079616 |  |
| testJSONRoundTrip | yes | 4173824 | 4308992 | 135168 | 81120 | 82592 | 1472 | 4210688 |  |
| testArrayListChurnAndBoundaryHealth | yes | 4308992 | 4317184 | 8192 | 82592 | 85728 | 3136 | 4210688 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4317184 | 4419584 | 102400 | 85728 | 87264 | 1536 | 4341760 |  |
| testSimpleVectorLifecycleStress | yes | 4419584 | 4419584 | 0 | 87264 | 87264 | 0 | 4341760 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4419584 | 4419584 | 0 | 89328 | 89328 | 0 | 4341760 |  |
| testJSONFileRoundTrip | yes | 4419584 | 4419584 | 0 | 89328 | 89808 | 480 | 4341760 |  |
| testSDListPersistenceStress | yes | 4419584 | 4419584 | 0 | 89760 | 90128 | 368 | 4341760 |  |
| testJSONPersistenceStress | yes | 4419584 | 4419584 | 0 | 90128 | 91168 | 1040 | 4341760 |  |
| testAVLTreeBasicBehavior | yes | 4419584 | 4419584 | 0 | 91168 | 91344 | 176 | 4341760 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4419584 | 4419584 | 0 | 91344 | 91504 | 160 | 4341760 |  |
| testPredicatesBasicBehavior | yes | 4419584 | 4419584 | 0 | 91504 | 91504 | 0 | 4341760 |  |
| testOperatorsBasicBehavior | yes | 4419584 | 4419584 | 0 | 91504 | 91504 | 0 | 4341760 |  |
| testDynamicStorageLibraryRamMode | yes | 4419584 | 4419584 | 0 | 93552 | 93552 | 0 | 4341760 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4419584 | 4419584 | 0 | 93552 | 93552 | 0 | 4341760 |  |
| testSimpleMutexFallbackBehavior | yes | 4419584 | 4489216 | 69632 | 93552 | 93552 | 0 | 4341760 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4489216 | 4489216 | 0 | 93552 | 93552 | 0 | 4341760 |  |
| testSimpleRWLockFallbackBehavior | yes | 4489216 | 4489216 | 0 | 93552 | 93696 | 144 | 4341760 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4489216 | 4489216 | 0 | 93696 | 93696 | 0 | 4341760 |  |
| testAVLTreeErrorCodes | yes | 4489216 | 4489216 | 0 | 93696 | 93744 | 48 | 4341760 |  |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3973120 | 3973120 | 0 | 77040 | 77312 | 272 | 3821568 |  |
| testHashtableBasicBehavior | yes | 3973120 | 3981312 | 8192 | 77456 | 79648 | 2192 | 3821568 |  |
| testSDListMemoryMode | yes | 3981312 | 3981312 | 0 | 79648 | 79744 | 96 | 3821568 |  |
| testSDListFileIOMode | yes | 3981312 | 4177920 | 196608 | 79744 | 80848 | 1104 | 4083712 |  |
| testCustomStringBehavior | yes | 4177920 | 4177920 | 0 | 80848 | 80848 | 0 | 4083712 |  |
| testJSONRoundTrip | yes | 4177920 | 4313088 | 135168 | 80848 | 82320 | 1472 | 4214784 |  |
| testArrayListChurnAndBoundaryHealth | yes | 4313088 | 4317184 | 4096 | 82320 | 85456 | 3136 | 4214784 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4317184 | 4423680 | 106496 | 85456 | 87120 | 1664 | 4345856 |  |
| testSimpleVectorLifecycleStress | yes | 4423680 | 4423680 | 0 | 87120 | 87120 | 0 | 4345856 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4423680 | 4423680 | 0 | 89184 | 89184 | 0 | 4345856 |  |
| testJSONFileRoundTrip | yes | 4423680 | 4423680 | 0 | 89184 | 89664 | 480 | 4345856 |  |
| testSDListPersistenceStress | yes | 4423680 | 4423680 | 0 | 89616 | 89680 | 64 | 4345856 |  |
| testJSONPersistenceStress | yes | 4423680 | 4423680 | 0 | 89680 | 90384 | 704 | 4345856 |  |
| testAVLTreeBasicBehavior | yes | 4423680 | 4423680 | 0 | 90384 | 90544 | 160 | 4345856 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4423680 | 4423680 | 0 | 90544 | 90784 | 240 | 4345856 |  |
| testPredicatesBasicBehavior | yes | 4423680 | 4423680 | 0 | 90784 | 90784 | 0 | 4345856 |  |
| testOperatorsBasicBehavior | yes | 4423680 | 4423680 | 0 | 90784 | 90784 | 0 | 4345856 |  |
| testDynamicStorageLibraryRamMode | yes | 4423680 | 4423680 | 0 | 92832 | 92832 | 0 | 4345856 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4423680 | 4423680 | 0 | 92832 | 92832 | 0 | 4345856 |  |
| testSimpleMutexFallbackBehavior | yes | 4423680 | 4493312 | 69632 | 92832 | 92832 | 0 | 4345856 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4493312 | 4493312 | 0 | 92832 | 92832 | 0 | 4345856 |  |
| testSimpleRWLockFallbackBehavior | yes | 4493312 | 4493312 | 0 | 92832 | 92976 | 144 | 4345856 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4493312 | 4493312 | 0 | 92976 | 93088 | 112 | 4345856 |  |
| testAVLTreeErrorCodes | yes | 4493312 | 4493312 | 0 | 93088 | 93136 | 48 | 4345856 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3923968 | 3923968 | 0 | 76928 | 77200 | 272 | 3776512 |  |
| testHashtableBasicBehavior | yes | 3923968 | 3932160 | 8192 | 77344 | 79536 | 2192 | 3776512 |  |
| testSDListMemoryMode | yes | 3932160 | 3932160 | 0 | 79536 | 79632 | 96 | 3776512 |  |
| testSDListFileIOMode | yes | 3932160 | 4128768 | 196608 | 79632 | 80752 | 1120 | 4038656 |  |
| testCustomStringBehavior | yes | 4128768 | 4128768 | 0 | 80752 | 80752 | 0 | 4038656 |  |
| testJSONRoundTrip | yes | 4128768 | 4263936 | 135168 | 80752 | 82272 | 1520 | 4169728 |  |
| testArrayListChurnAndBoundaryHealth | yes | 4263936 | 4268032 | 4096 | 82272 | 85312 | 3040 | 4169728 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4268032 | 4374528 | 106496 | 85312 | 86832 | 1520 | 4300800 |  |
| testSimpleVectorLifecycleStress | yes | 4374528 | 4374528 | 0 | 86832 | 86832 | 0 | 4300800 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4374528 | 4374528 | 0 | 88896 | 88896 | 0 | 4300800 |  |
| testJSONFileRoundTrip | yes | 4374528 | 4374528 | 0 | 88896 | 89152 | 256 | 4300800 |  |
| testSDListPersistenceStress | yes | 4374528 | 4374528 | 0 | 89152 | 89504 | 352 | 4300800 |  |
| testJSONPersistenceStress | yes | 4374528 | 4374528 | 0 | 89504 | 90320 | 816 | 4300800 |  |
| testAVLTreeBasicBehavior | yes | 4374528 | 4374528 | 0 | 90320 | 90368 | 48 | 4300800 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4374528 | 4374528 | 0 | 90368 | 90464 | 96 | 4300800 |  |
| testPredicatesBasicBehavior | yes | 4374528 | 4374528 | 0 | 90464 | 90464 | 0 | 4300800 |  |
| testOperatorsBasicBehavior | yes | 4374528 | 4374528 | 0 | 90464 | 90464 | 0 | 4300800 |  |
| testDynamicStorageLibraryRamMode | yes | 4374528 | 4374528 | 0 | 92512 | 92512 | 0 | 4300800 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4374528 | 4374528 | 0 | 92512 | 92512 | 0 | 4300800 |  |
| testSimpleMutexFallbackBehavior | yes | 4374528 | 4444160 | 69632 | 92512 | 92512 | 0 | 4300800 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4444160 | 4444160 | 0 | 92512 | 92512 | 0 | 4300800 |  |
| testSimpleRWLockFallbackBehavior | yes | 4444160 | 4444160 | 0 | 92512 | 92704 | 192 | 4300800 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4444160 | 4444160 | 0 | 92704 | 92816 | 112 | 4300800 |  |

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

- Boards profiled: 5
- Total runs processed: 20
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
| leonardo | 2560 | 20000 | 20000 | 14161 ✓ | 9745 ✓ | 20000 | 8001 ✓ | 20000 | 20000 | 20000 | 20000 | 20000 | 12977 ✓ |
| mega2560 | 8192 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 | 20000 |
| micro | 2560 | 20000 | 20000 | 14161 ✓ | 9745 ✓ | 20000 | 8001 ✓ | 20000 | 20000 | 20000 | 20000 | 20000 | 12977 ✓ |
| nano | 2048 | 20000 | 20000 | 11185 ✓ | 7953 ✓ | 20000 | 6353 ✓ | 20000 | 20000 | 20000 | 20000 | 20000 | 10257 ✓ |
| uno | 2048 | 20000 | 20000 | 11185 ✓ | 7953 ✓ | 20000 | 6353 ✓ | 20000 | 20000 | 20000 | 20000 | 20000 | 10257 ✓ |

✓ = budget reached during probe (count is the boundary value)

## Element Fill Probes

_How many elements fit in a single container instance within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList[int] | ArrayList[float] | ArrayList[double] | ArrayList[String] | SimpleVector[int] | SimpleVector[float] | SimpleVector[double] | Hashtable[int_int] | Hashtable[String_String] | AVLTree[int] | DynamicStorage[String_int] | Stack[int] | Stack[float] | Stack[double] | Stack[String] | Queue[int] | Queue[float] | Queue[double] | Queue[String] | LinkedList[int] | LinkedList[float] | LinkedList[double] | LinkedList[String] | OrderedMap[int_int] | OrderedMap[String_String] |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| leonardo | 2560 | 5000000 | 446977 ✓ | 297985 ✓ | 58881 ✓ | 524289 ✓ | 524289 ✓ | 262145 ✓ | 49153 ✓ | 26369 ✓ | 54785 ✓ | 21249 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 81921 ✓ | 82177 ✓ | 81921 ✓ | 54785 ✓ | 297985 ✓ | 39425 ✓ |
| mega2560 | 8192 | 5000000 | 1508097 ✓ | 1005313 ✓ | 198657 ✓ | 1048577 ✓ | 1048577 ✓ | 524289 ✓ | 183553 ✓ | 91905 ✓ | 174849 ✓ | 65793 ✓ | 1310721 ✓ | 1310721 ✓ | 655361 ✓ | 163841 ✓ | 1310721 ✓ | 1310721 ✓ | 655361 ✓ | 163841 ✓ | 262401 ✓ | 262145 ✓ | 262145 ✓ | 174849 ✓ | 1005313 ✓ | 88321 ✓ |
| micro | 2560 | 5000000 | 446977 ✓ | 297985 ✓ | 58881 ✓ | 524289 ✓ | 524289 ✓ | 262145 ✓ | 49153 ✓ | 26369 ✓ | 54785 ✓ | 21249 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 81921 ✓ | 82177 ✓ | 81921 ✓ | 54785 ✓ | 297985 ✓ | 39425 ✓ |
| nano | 2048 | 5000000 | 446977 ✓ | 198657 ✓ | 58881 ✓ | 262145 ✓ | 262145 ✓ | 131073 ✓ | 46081 ✓ | 23041 ✓ | 43777 ✓ | 16641 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 65793 ✓ | 65537 ✓ | 65537 ✓ | 43777 ✓ | 198657 ✓ | 26369 ✓ |
| uno | 2048 | 5000000 | 446977 ✓ | 198657 ✓ | 58881 ✓ | 262145 ✓ | 262145 ✓ | 131073 ✓ | 46081 ✓ | 23041 ✓ | 43777 ✓ | 16641 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 65793 ✓ | 65537 ✓ | 65537 ✓ | 43777 ✓ | 198657 ✓ | 26369 ✓ |

✓ = budget reached during probe (count is the boundary value)

## Per-Board Details

### leonardo

- SRAM: 2560 bytes
- Host budget: 2621440 bytes (2560 × 1024)

### mega2560

- SRAM: 8192 bytes
- Host budget: 8388608 bytes (8192 × 1024)

### micro

- SRAM: 2560 bytes
- Host budget: 2621440 bytes (2560 × 1024)

### nano

- SRAM: 2048 bytes
- Host budget: 2097152 bytes (2048 × 1024)

### uno

- SRAM: 2048 bytes
- Host budget: 2097152 bytes (2048 × 1024)
