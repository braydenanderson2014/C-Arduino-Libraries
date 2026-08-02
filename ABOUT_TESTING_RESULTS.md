# Host Simulation Testing Report

## Summary

- Total runs: 4
- Passed runs: 4
- Failed runs: 0
- Max peak bytes: 4337664
- Avg peak bytes: 4161536
- Compile smoke objects found: 2
- Experimental compile results found: 1
- Experimental compile successes: 1
- Experimental compile failures: 0
- Experimental libraries covered: 1
- Experimental backends covered: 1
- Experimental optional modes covered: 1
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
| optional-littlefs | yes | LittleFS | 4227072 | 8388608 | no | yes |  | 0 | 0 | 0 |
| optional-sd | yes | SD | 4096000 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 3985408 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3964928 | 3964928 | 0 | 77056 | 77328 | 272 | 3813376 |  |
| testHashtableBasicBehavior | yes | 3964928 | 3973120 | 8192 | 77472 | 79664 | 2192 | 3813376 |  |
| testSDListMemoryMode | yes | 3973120 | 3973120 | 0 | 79664 | 79760 | 96 | 3813376 |  |
| testSDListFileIOMode | yes | 3973120 | 4169728 | 196608 | 79760 | 81008 | 1248 | 4075520 |  |
| testCustomStringBehavior | yes | 4169728 | 4169728 | 0 | 81008 | 81008 | 0 | 4075520 |  |
| testJSONRoundTrip | yes | 4169728 | 4304896 | 135168 | 81008 | 82480 | 1472 | 4206592 |  |
| testArrayListChurnAndBoundaryHealth | yes | 4304896 | 4308992 | 4096 | 82480 | 85600 | 3120 | 4206592 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4308992 | 4415488 | 106496 | 85600 | 87040 | 1440 | 4337664 |  |
| testSimpleVectorLifecycleStress | yes | 4415488 | 4415488 | 0 | 87040 | 87040 | 0 | 4337664 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4415488 | 4415488 | 0 | 89104 | 89152 | 48 | 4337664 |  |
| testJSONFileRoundTrip | yes | 4415488 | 4415488 | 0 | 89152 | 89472 | 320 | 4337664 |  |
| testSDListPersistenceStress | yes | 4415488 | 4415488 | 0 | 89472 | 89760 | 288 | 4337664 |  |
| testJSONPersistenceStress | yes | 4415488 | 4415488 | 0 | 89760 | 90576 | 816 | 4337664 |  |
| testAVLTreeBasicBehavior | yes | 4415488 | 4415488 | 0 | 90576 | 90672 | 96 | 4337664 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4415488 | 4415488 | 0 | 90672 | 90720 | 48 | 4337664 |  |
| testPredicatesBasicBehavior | yes | 4415488 | 4415488 | 0 | 90720 | 90720 | 0 | 4337664 |  |
| testOperatorsBasicBehavior | yes | 4415488 | 4415488 | 0 | 90720 | 90720 | 0 | 4337664 |  |
| testDynamicStorageLibraryRamMode | yes | 4415488 | 4415488 | 0 | 92768 | 92768 | 0 | 4337664 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4415488 | 4415488 | 0 | 92768 | 92768 | 0 | 4337664 |  |
| testSimpleMutexFallbackBehavior | yes | 4415488 | 4485120 | 69632 | 92768 | 92768 | 0 | 4337664 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4485120 | 4485120 | 0 | 92768 | 92768 | 0 | 4337664 |  |
| testSimpleRWLockFallbackBehavior | yes | 4485120 | 4485120 | 0 | 92768 | 92912 | 144 | 4337664 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4485120 | 4485120 | 0 | 92912 | 92912 | 0 | 4337664 |  |

### Per-test Memory Stats: optional-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3915776 | 3915776 | 0 | 77232 | 77504 | 272 | 3833856 |  |
| testHashtableBasicBehavior | yes | 3915776 | 3923968 | 8192 | 77648 | 79776 | 2128 | 3833856 |  |
| testSDListMemoryMode | yes | 3923968 | 3923968 | 0 | 79776 | 79872 | 96 | 3833856 |  |
| testSDListFileIOMode | yes | 3923968 | 4120576 | 196608 | 79872 | 81120 | 1248 | 3964928 |  |
| testCustomStringBehavior | yes | 4120576 | 4120576 | 0 | 81120 | 81120 | 0 | 3964928 |  |
| testJSONRoundTrip | yes | 4120576 | 4255744 | 135168 | 81120 | 82592 | 1472 | 4096000 |  |
| testArrayListChurnAndBoundaryHealth | yes | 4255744 | 4263936 | 8192 | 82592 | 85728 | 3136 | 4096000 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4263936 | 4366336 | 102400 | 85728 | 87264 | 1536 | 4227072 |  |
| testSimpleVectorLifecycleStress | yes | 4366336 | 4366336 | 0 | 87264 | 87264 | 0 | 4227072 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4366336 | 4366336 | 0 | 89328 | 89328 | 0 | 4227072 |  |
| testJSONFileRoundTrip | yes | 4366336 | 4366336 | 0 | 89328 | 89808 | 480 | 4227072 |  |
| testSDListPersistenceStress | yes | 4366336 | 4366336 | 0 | 89760 | 90128 | 368 | 4227072 |  |
| testJSONPersistenceStress | yes | 4366336 | 4366336 | 0 | 90128 | 91168 | 1040 | 4227072 |  |
| testAVLTreeBasicBehavior | yes | 4366336 | 4366336 | 0 | 91168 | 91344 | 176 | 4227072 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4366336 | 4366336 | 0 | 91344 | 91504 | 160 | 4227072 |  |
| testPredicatesBasicBehavior | yes | 4366336 | 4366336 | 0 | 91504 | 91504 | 0 | 4227072 |  |
| testOperatorsBasicBehavior | yes | 4366336 | 4366336 | 0 | 91504 | 91504 | 0 | 4227072 |  |
| testDynamicStorageLibraryRamMode | yes | 4366336 | 4366336 | 0 | 93552 | 93552 | 0 | 4227072 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4366336 | 4366336 | 0 | 93552 | 93552 | 0 | 4227072 |  |
| testSimpleMutexFallbackBehavior | yes | 4366336 | 4435968 | 69632 | 93552 | 93552 | 0 | 4227072 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4435968 | 4435968 | 0 | 93552 | 93552 | 0 | 4227072 |  |
| testSimpleRWLockFallbackBehavior | yes | 4435968 | 4435968 | 0 | 93552 | 93696 | 144 | 4227072 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4435968 | 4435968 | 0 | 93696 | 93696 | 0 | 4227072 |  |
| testAVLTreeErrorCodes | yes | 4435968 | 4435968 | 0 | 93696 | 93744 | 48 | 4227072 |  |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3661824 | 3661824 | 0 | 77040 | 77312 | 272 | 3510272 |  |
| testHashtableBasicBehavior | yes | 3661824 | 3670016 | 8192 | 77456 | 79648 | 2192 | 3510272 |  |
| testSDListMemoryMode | yes | 3670016 | 3670016 | 0 | 79648 | 79744 | 96 | 3510272 |  |
| testSDListFileIOMode | yes | 3670016 | 3862528 | 192512 | 79744 | 80848 | 1104 | 3702784 |  |
| testCustomStringBehavior | yes | 3862528 | 3862528 | 0 | 80848 | 80848 | 0 | 3702784 |  |
| testJSONRoundTrip | yes | 3862528 | 3997696 | 135168 | 80848 | 82320 | 1472 | 3833856 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3997696 | 4001792 | 4096 | 82320 | 85456 | 3136 | 3833856 |  |
| testHashtableChurnAndBoundaryHealth | yes | 4001792 | 4108288 | 106496 | 85456 | 87120 | 1664 | 3964928 |  |
| testSimpleVectorLifecycleStress | yes | 4108288 | 4108288 | 0 | 87120 | 87120 | 0 | 3964928 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4108288 | 4108288 | 0 | 89184 | 89184 | 0 | 3964928 |  |
| testJSONFileRoundTrip | yes | 4108288 | 4108288 | 0 | 89184 | 89664 | 480 | 3964928 |  |
| testSDListPersistenceStress | yes | 4108288 | 4108288 | 0 | 89616 | 89680 | 64 | 3964928 |  |
| testJSONPersistenceStress | yes | 4108288 | 4108288 | 0 | 89680 | 90384 | 704 | 3964928 |  |
| testAVLTreeBasicBehavior | yes | 4108288 | 4108288 | 0 | 90384 | 90544 | 160 | 3964928 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4108288 | 4108288 | 0 | 90544 | 90784 | 240 | 3964928 |  |
| testPredicatesBasicBehavior | yes | 4108288 | 4108288 | 0 | 90784 | 90784 | 0 | 3964928 |  |
| testOperatorsBasicBehavior | yes | 4108288 | 4108288 | 0 | 90784 | 90784 | 0 | 3964928 |  |
| testDynamicStorageLibraryRamMode | yes | 4108288 | 4108288 | 0 | 92832 | 92832 | 0 | 3964928 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4108288 | 4108288 | 0 | 92832 | 92832 | 0 | 3964928 |  |
| testSimpleMutexFallbackBehavior | yes | 4108288 | 4177920 | 69632 | 92832 | 92832 | 0 | 4096000 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4177920 | 4177920 | 0 | 92832 | 92832 | 0 | 4096000 |  |
| testSimpleRWLockFallbackBehavior | yes | 4177920 | 4177920 | 0 | 92832 | 92976 | 144 | 4096000 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4177920 | 4177920 | 0 | 92976 | 93088 | 112 | 4096000 |  |
| testAVLTreeErrorCodes | yes | 4177920 | 4177920 | 0 | 93088 | 93136 | 48 | 4096000 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3616768 | 3616768 | 0 | 76928 | 77200 | 272 | 3530752 |  |
| testHashtableBasicBehavior | yes | 3616768 | 3624960 | 8192 | 77344 | 79536 | 2192 | 3530752 |  |
| testSDListMemoryMode | yes | 3624960 | 3624960 | 0 | 79536 | 79632 | 96 | 3530752 |  |
| testSDListFileIOMode | yes | 3624960 | 3817472 | 192512 | 79632 | 80752 | 1120 | 3723264 |  |
| testCustomStringBehavior | yes | 3817472 | 3817472 | 0 | 80752 | 80752 | 0 | 3723264 |  |
| testJSONRoundTrip | yes | 3817472 | 3952640 | 135168 | 80752 | 82272 | 1520 | 3854336 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3952640 | 3956736 | 4096 | 82272 | 85312 | 3040 | 3854336 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3956736 | 4063232 | 106496 | 85312 | 86832 | 1520 | 3985408 |  |
| testSimpleVectorLifecycleStress | yes | 4063232 | 4063232 | 0 | 86832 | 86832 | 0 | 3985408 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4063232 | 4063232 | 0 | 88896 | 88896 | 0 | 3985408 |  |
| testJSONFileRoundTrip | yes | 4063232 | 4063232 | 0 | 88896 | 89152 | 256 | 3985408 |  |
| testSDListPersistenceStress | yes | 4063232 | 4063232 | 0 | 89152 | 89504 | 352 | 3985408 |  |
| testJSONPersistenceStress | yes | 4063232 | 4063232 | 0 | 89504 | 90320 | 816 | 3985408 |  |
| testAVLTreeBasicBehavior | yes | 4063232 | 4063232 | 0 | 90320 | 90368 | 48 | 3985408 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4063232 | 4063232 | 0 | 90368 | 90464 | 96 | 3985408 |  |
| testPredicatesBasicBehavior | yes | 4063232 | 4063232 | 0 | 90464 | 90464 | 0 | 3985408 |  |
| testOperatorsBasicBehavior | yes | 4063232 | 4063232 | 0 | 90464 | 90464 | 0 | 3985408 |  |
| testDynamicStorageLibraryRamMode | yes | 4063232 | 4063232 | 0 | 92512 | 92512 | 0 | 3985408 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4063232 | 4063232 | 0 | 92512 | 92512 | 0 | 3985408 |  |
| testSimpleMutexFallbackBehavior | yes | 4063232 | 4132864 | 69632 | 92512 | 92512 | 0 | 3985408 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92512 | 92512 | 0 | 3985408 |  |
| testSimpleRWLockFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92512 | 92704 | 192 | 3985408 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4132864 | 4132864 | 0 | 92704 | 92816 | 112 | 3985408 |  |

## Experimental Compile Results

_Each row is one experimental compile matrix entry (library + backend + optional mode)._

| Library | Backend | Optional | Success | ReturnCode | Artifact |
| --- | --- | --- | --- | ---: | --- |
| lib/__ArduinoUnoQLibraryDevelopment__Experimental/SimpleCoreRuntime | sd | off | yes | 0 | test/host_arduino_sim/out/experimental-library-smoke/lib___ArduinoUnoQLibraryDevelopment__Experimental_SimpleCoreRuntime/sd/off |
