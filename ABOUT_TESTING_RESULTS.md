# Host Simulation Testing Report

## Summary

- Total runs: 2
- Passed runs: 2
- Failed runs: 0
- Max peak bytes: 4096000
- Avg peak bytes: 4087808
- Compile smoke objects found: 45
- Experimental compile results found: 0
- Experimental compile successes: 0
- Experimental compile failures: 0
- Experimental libraries covered: 0
- Experimental backends covered: 0
- Experimental optional modes covered: 0
- Memory profile runs: 0
- Runs that exceeded limit: 0
- Runs with limit enforcement enabled: 2
- Runs with capacity probe enabled: 0
- Runs where capacity probe reached limit: 0
- Runs with first limit-crossing test identified: 0
- Expected compile smoke objects: 45
- Expected experimental compile results: 0
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
| optional-sd | yes | SD | 4096000 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4079616 | 8388608 | no | yes |  | 0 | 0 | 0 |

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
| testDynamicStorageLibraryRamMode | yes | 4096000 | 4104192 | 8192 | 92832 | 92832 | 0 | 3956736 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4104192 | 4104192 | 0 | 92832 | 92832 | 0 | 3956736 |  |
| testSimpleMutexFallbackBehavior | yes | 4104192 | 4173824 | 69632 | 92832 | 92832 | 0 | 4096000 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92832 | 92832 | 0 | 4096000 |  |
| testSimpleRWLockFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92832 | 92976 | 144 | 4096000 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92976 | 93088 | 112 | 4096000 |  |
| testAVLTreeErrorCodes | yes | 4173824 | 4173824 | 0 | 93088 | 93136 | 48 | 4096000 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3645440 | 3645440 | 0 | 76928 | 77200 | 272 | 3493888 |  |
| testHashtableBasicBehavior | yes | 3645440 | 3653632 | 8192 | 77344 | 79536 | 2192 | 3493888 |  |
| testSDListMemoryMode | yes | 3653632 | 3653632 | 0 | 79536 | 79632 | 96 | 3493888 |  |
| testSDListFileIOMode | yes | 3653632 | 3846144 | 192512 | 79632 | 80752 | 1120 | 3686400 |  |
| testCustomStringBehavior | yes | 3846144 | 3846144 | 0 | 80752 | 80752 | 0 | 3686400 |  |
| testJSONRoundTrip | yes | 3846144 | 3981312 | 135168 | 80752 | 82272 | 1520 | 3817472 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3981312 | 3985408 | 4096 | 82272 | 85312 | 3040 | 3817472 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3985408 | 4091904 | 106496 | 85312 | 86832 | 1520 | 3948544 |  |
| testSimpleVectorLifecycleStress | yes | 4091904 | 4091904 | 0 | 86832 | 86832 | 0 | 3948544 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4091904 | 4091904 | 0 | 88896 | 88896 | 0 | 3948544 |  |
| testJSONFileRoundTrip | yes | 4091904 | 4091904 | 0 | 88896 | 89152 | 256 | 3948544 |  |
| testSDListPersistenceStress | yes | 4091904 | 4091904 | 0 | 89152 | 89504 | 352 | 3948544 |  |
| testJSONPersistenceStress | yes | 4091904 | 4091904 | 0 | 89504 | 90320 | 816 | 3948544 |  |
| testAVLTreeBasicBehavior | yes | 4091904 | 4091904 | 0 | 90320 | 90368 | 48 | 3948544 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4091904 | 4091904 | 0 | 90368 | 90464 | 96 | 3948544 |  |
| testPredicatesBasicBehavior | yes | 4091904 | 4091904 | 0 | 90464 | 90464 | 0 | 3948544 |  |
| testOperatorsBasicBehavior | yes | 4091904 | 4091904 | 0 | 90464 | 90464 | 0 | 3948544 |  |
| testDynamicStorageLibraryRamMode | yes | 4091904 | 4091904 | 0 | 92512 | 92512 | 0 | 3948544 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4091904 | 4091904 | 0 | 92512 | 92512 | 0 | 3948544 |  |
| testSimpleMutexFallbackBehavior | yes | 4091904 | 4161536 | 69632 | 92512 | 92512 | 0 | 4079616 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4161536 | 4161536 | 0 | 92512 | 92512 | 0 | 4079616 |  |
| testSimpleRWLockFallbackBehavior | yes | 4161536 | 4161536 | 0 | 92512 | 92704 | 192 | 4079616 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4161536 | 4161536 | 0 | 92704 | 92816 | 112 | 4079616 |  |
