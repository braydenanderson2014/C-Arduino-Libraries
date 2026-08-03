# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4370432
- Avg peak bytes: 4179968
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
| littlefs | yes | LittleFS | 4337664 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-littlefs | yes | LittleFS | 4370432 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-sd | yes | SD | 3993600 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4018176 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3960832 | 3960832 | 0 | 77056 | 77328 | 272 | 3813376 |  |
| testHashtableBasicBehavior | yes | 3960832 | 3969024 | 8192 | 77472 | 79664 | 2192 | 3813376 |  |
| testSDListMemoryMode | yes | 3969024 | 3969024 | 0 | 79664 | 79760 | 96 | 3813376 |  |
| testSDListFileIOMode | yes | 3969024 | 4165632 | 196608 | 79760 | 81008 | 1248 | 4075520 |  |
| testCustomStringBehavior | yes | 4165632 | 4165632 | 0 | 81008 | 81008 | 0 | 4075520 |  |
| testJSONRoundTrip | yes | 4165632 | 4300800 | 135168 | 81008 | 82480 | 1472 | 4206592 |  |
| testArrayListChurnAndBoundaryHealth | yes | 4300800 | 4304896 | 4096 | 82480 | 85600 | 3120 | 4206592 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4304896 | 4411392 | 106496 | 85600 | 87040 | 1440 | 4337664 |  |
| testSimpleVectorLifecycleStress | yes | 4411392 | 4411392 | 0 | 87040 | 87040 | 0 | 4337664 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4411392 | 4411392 | 0 | 89104 | 89152 | 48 | 4337664 |  |
| testJSONFileRoundTrip | yes | 4411392 | 4411392 | 0 | 89152 | 89472 | 320 | 4337664 |  |
| testSDListPersistenceStress | yes | 4411392 | 4411392 | 0 | 89472 | 89760 | 288 | 4337664 |  |
| testJSONPersistenceStress | yes | 4411392 | 4411392 | 0 | 89760 | 90576 | 816 | 4337664 |  |
| testAVLTreeBasicBehavior | yes | 4411392 | 4411392 | 0 | 90576 | 90672 | 96 | 4337664 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4411392 | 4411392 | 0 | 90672 | 90720 | 48 | 4337664 |  |
| testPredicatesBasicBehavior | yes | 4411392 | 4411392 | 0 | 90720 | 90720 | 0 | 4337664 |  |
| testOperatorsBasicBehavior | yes | 4411392 | 4411392 | 0 | 90720 | 90720 | 0 | 4337664 |  |
| testDynamicStorageLibraryRamMode | yes | 4411392 | 4411392 | 0 | 92768 | 92768 | 0 | 4337664 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4411392 | 4411392 | 0 | 92768 | 92768 | 0 | 4337664 |  |
| testSimpleMutexFallbackBehavior | yes | 4411392 | 4481024 | 69632 | 92768 | 92768 | 0 | 4337664 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4481024 | 4481024 | 0 | 92768 | 92768 | 0 | 4337664 |  |
| testSimpleRWLockFallbackBehavior | yes | 4481024 | 4481024 | 0 | 92768 | 92912 | 144 | 4337664 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4481024 | 4481024 | 0 | 92912 | 92912 | 0 | 4337664 |  |

### Per-test Memory Stats: optional-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3928064 | 3928064 | 0 | 77232 | 77504 | 272 | 3846144 |  |
| testHashtableBasicBehavior | yes | 3928064 | 3936256 | 8192 | 77648 | 79776 | 2128 | 3846144 |  |
| testSDListMemoryMode | yes | 3936256 | 3936256 | 0 | 79776 | 79872 | 96 | 3846144 |  |
| testSDListFileIOMode | yes | 3936256 | 4132864 | 196608 | 79872 | 81120 | 1248 | 3977216 |  |
| testCustomStringBehavior | yes | 4132864 | 4132864 | 0 | 81120 | 81120 | 0 | 3977216 |  |
| testJSONRoundTrip | yes | 4132864 | 4268032 | 135168 | 81120 | 82592 | 1472 | 4108288 |  |
| testArrayListChurnAndBoundaryHealth | yes | 4268032 | 4276224 | 8192 | 82592 | 85728 | 3136 | 4108288 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4276224 | 4378624 | 102400 | 85728 | 87264 | 1536 | 4239360 |  |
| testSimpleVectorLifecycleStress | yes | 4378624 | 4378624 | 0 | 87264 | 87264 | 0 | 4239360 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4378624 | 4378624 | 0 | 89328 | 89328 | 0 | 4239360 |  |
| testJSONFileRoundTrip | yes | 4378624 | 4378624 | 0 | 89328 | 89808 | 480 | 4239360 |  |
| testSDListPersistenceStress | yes | 4378624 | 4378624 | 0 | 89760 | 90128 | 368 | 4239360 |  |
| testJSONPersistenceStress | yes | 4378624 | 4378624 | 0 | 90128 | 91168 | 1040 | 4239360 |  |
| testAVLTreeBasicBehavior | yes | 4378624 | 4378624 | 0 | 91168 | 91344 | 176 | 4239360 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4378624 | 4378624 | 0 | 91344 | 91504 | 160 | 4239360 |  |
| testPredicatesBasicBehavior | yes | 4378624 | 4378624 | 0 | 91504 | 91504 | 0 | 4239360 |  |
| testOperatorsBasicBehavior | yes | 4378624 | 4378624 | 0 | 91504 | 91504 | 0 | 4239360 |  |
| testDynamicStorageLibraryRamMode | yes | 4378624 | 4378624 | 0 | 93552 | 93552 | 0 | 4239360 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4378624 | 4378624 | 0 | 93552 | 93552 | 0 | 4239360 |  |
| testSimpleMutexFallbackBehavior | yes | 4378624 | 4448256 | 69632 | 93552 | 93552 | 0 | 4370432 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4448256 | 4448256 | 0 | 93552 | 93552 | 0 | 4370432 |  |
| testSimpleRWLockFallbackBehavior | yes | 4448256 | 4448256 | 0 | 93552 | 93696 | 144 | 4370432 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4448256 | 4448256 | 0 | 93696 | 93696 | 0 | 4370432 |  |
| testAVLTreeErrorCodes | yes | 4448256 | 4448256 | 0 | 93696 | 93744 | 48 | 4370432 |  |

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
| testArrayListBasicBehavior | yes | 3645440 | 3645440 | 0 | 76928 | 77200 | 272 | 3563520 |  |
| testHashtableBasicBehavior | yes | 3645440 | 3653632 | 8192 | 77344 | 79536 | 2192 | 3563520 |  |
| testSDListMemoryMode | yes | 3653632 | 3653632 | 0 | 79536 | 79632 | 96 | 3563520 |  |
| testSDListFileIOMode | yes | 3653632 | 3846144 | 192512 | 79632 | 80752 | 1120 | 3756032 |  |
| testCustomStringBehavior | yes | 3846144 | 3846144 | 0 | 80752 | 80752 | 0 | 3756032 |  |
| testJSONRoundTrip | yes | 3846144 | 3981312 | 135168 | 80752 | 82272 | 1520 | 3887104 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3981312 | 3985408 | 4096 | 82272 | 85312 | 3040 | 3887104 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3985408 | 4091904 | 106496 | 85312 | 86832 | 1520 | 4018176 |  |
| testSimpleVectorLifecycleStress | yes | 4091904 | 4091904 | 0 | 86832 | 86832 | 0 | 4018176 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4091904 | 4091904 | 0 | 88896 | 88896 | 0 | 4018176 |  |
| testJSONFileRoundTrip | yes | 4091904 | 4091904 | 0 | 88896 | 89152 | 256 | 4018176 |  |
| testSDListPersistenceStress | yes | 4091904 | 4091904 | 0 | 89152 | 89504 | 352 | 4018176 |  |
| testJSONPersistenceStress | yes | 4091904 | 4091904 | 0 | 89504 | 90320 | 816 | 4018176 |  |
| testAVLTreeBasicBehavior | yes | 4091904 | 4091904 | 0 | 90320 | 90368 | 48 | 4018176 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4091904 | 4091904 | 0 | 90368 | 90464 | 96 | 4018176 |  |
| testPredicatesBasicBehavior | yes | 4091904 | 4091904 | 0 | 90464 | 90464 | 0 | 4018176 |  |
| testOperatorsBasicBehavior | yes | 4091904 | 4091904 | 0 | 90464 | 90464 | 0 | 4018176 |  |
| testDynamicStorageLibraryRamMode | yes | 4091904 | 4091904 | 0 | 92512 | 92512 | 0 | 4018176 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4091904 | 4091904 | 0 | 92512 | 92512 | 0 | 4018176 |  |
| testSimpleMutexFallbackBehavior | yes | 4091904 | 4161536 | 69632 | 92512 | 92512 | 0 | 4018176 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4161536 | 4161536 | 0 | 92512 | 92512 | 0 | 4018176 |  |
| testSimpleRWLockFallbackBehavior | yes | 4161536 | 4161536 | 0 | 92512 | 92704 | 192 | 4018176 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4161536 | 4161536 | 0 | 92704 | 92816 | 112 | 4018176 |  |

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
