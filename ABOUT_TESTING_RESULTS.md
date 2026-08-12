# Host Simulation Testing Report

## Summary

- Total runs: 2
- Passed runs: 2
- Failed runs: 0
- Max peak bytes: 4030464
- Avg peak bytes: 3997696
- Compile smoke objects found: 1
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
- Expected compile smoke objects: 1
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
| optional-sd | yes | SD | 4030464 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 3964928 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3657728 | 3657728 | 0 | 77040 | 77312 | 272 | 3575808 |  |
| testHashtableBasicBehavior | yes | 3657728 | 3665920 | 8192 | 77456 | 79648 | 2192 | 3575808 |  |
| testSDListMemoryMode | yes | 3665920 | 3665920 | 0 | 79648 | 79744 | 96 | 3575808 |  |
| testSDListFileIOMode | yes | 3665920 | 3858432 | 192512 | 79744 | 80848 | 1104 | 3768320 |  |
| testCustomStringBehavior | yes | 3858432 | 3858432 | 0 | 80848 | 80848 | 0 | 3768320 |  |
| testJSONRoundTrip | yes | 3858432 | 3993600 | 135168 | 80848 | 82320 | 1472 | 3899392 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3993600 | 3997696 | 4096 | 82320 | 85456 | 3136 | 3899392 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3997696 | 4104192 | 106496 | 85456 | 87120 | 1664 | 4030464 |  |
| testSimpleVectorLifecycleStress | yes | 4104192 | 4104192 | 0 | 87120 | 87120 | 0 | 4030464 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4104192 | 4104192 | 0 | 89184 | 89184 | 0 | 4030464 |  |
| testJSONFileRoundTrip | yes | 4104192 | 4104192 | 0 | 89184 | 89664 | 480 | 4030464 |  |
| testSDListPersistenceStress | yes | 4104192 | 4104192 | 0 | 89616 | 89680 | 64 | 4030464 |  |
| testJSONPersistenceStress | yes | 4104192 | 4104192 | 0 | 89680 | 90384 | 704 | 4030464 |  |
| testAVLTreeBasicBehavior | yes | 4104192 | 4104192 | 0 | 90384 | 90544 | 160 | 4030464 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4104192 | 4104192 | 0 | 90544 | 90784 | 240 | 4030464 |  |
| testPredicatesBasicBehavior | yes | 4104192 | 4104192 | 0 | 90784 | 90784 | 0 | 4030464 |  |
| testOperatorsBasicBehavior | yes | 4104192 | 4104192 | 0 | 90784 | 90784 | 0 | 4030464 |  |
| testDynamicStorageLibraryRamMode | yes | 4104192 | 4104192 | 0 | 92832 | 92832 | 0 | 4030464 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4104192 | 4104192 | 0 | 92832 | 92832 | 0 | 4030464 |  |
| testSimpleMutexFallbackBehavior | yes | 4104192 | 4173824 | 69632 | 92832 | 92832 | 0 | 4030464 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92832 | 92832 | 0 | 4030464 |  |
| testSimpleRWLockFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92832 | 92976 | 144 | 4030464 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4173824 | 4173824 | 0 | 92976 | 93088 | 112 | 4030464 |  |
| testAVLTreeErrorCodes | yes | 4173824 | 4173824 | 0 | 93088 | 93136 | 48 | 4030464 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3592192 | 3592192 | 0 | 76928 | 77200 | 272 | 3510272 |  |
| testHashtableBasicBehavior | yes | 3592192 | 3600384 | 8192 | 77344 | 79536 | 2192 | 3510272 |  |
| testSDListMemoryMode | yes | 3600384 | 3600384 | 0 | 79536 | 79632 | 96 | 3510272 |  |
| testSDListFileIOMode | yes | 3600384 | 3792896 | 192512 | 79632 | 80752 | 1120 | 3702784 |  |
| testCustomStringBehavior | yes | 3792896 | 3792896 | 0 | 80752 | 80752 | 0 | 3702784 |  |
| testJSONRoundTrip | yes | 3792896 | 3928064 | 135168 | 80752 | 82272 | 1520 | 3833856 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3928064 | 3932160 | 4096 | 82272 | 85312 | 3040 | 3833856 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3932160 | 4038656 | 106496 | 85312 | 86832 | 1520 | 3964928 |  |
| testSimpleVectorLifecycleStress | yes | 4038656 | 4038656 | 0 | 86832 | 86832 | 0 | 3964928 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4038656 | 4038656 | 0 | 88896 | 88896 | 0 | 3964928 |  |
| testJSONFileRoundTrip | yes | 4038656 | 4038656 | 0 | 88896 | 89152 | 256 | 3964928 |  |
| testSDListPersistenceStress | yes | 4038656 | 4038656 | 0 | 89152 | 89504 | 352 | 3964928 |  |
| testJSONPersistenceStress | yes | 4038656 | 4038656 | 0 | 89504 | 90320 | 816 | 3964928 |  |
| testAVLTreeBasicBehavior | yes | 4038656 | 4038656 | 0 | 90320 | 90368 | 48 | 3964928 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4038656 | 4038656 | 0 | 90368 | 90464 | 96 | 3964928 |  |
| testPredicatesBasicBehavior | yes | 4038656 | 4038656 | 0 | 90464 | 90464 | 0 | 3964928 |  |
| testOperatorsBasicBehavior | yes | 4038656 | 4038656 | 0 | 90464 | 90464 | 0 | 3964928 |  |
| testDynamicStorageLibraryRamMode | yes | 4038656 | 4038656 | 0 | 92512 | 92512 | 0 | 3964928 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4038656 | 4038656 | 0 | 92512 | 92512 | 0 | 3964928 |  |
| testSimpleMutexFallbackBehavior | yes | 4038656 | 4108288 | 69632 | 92512 | 92512 | 0 | 3964928 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4108288 | 4108288 | 0 | 92512 | 92512 | 0 | 3964928 |  |
| testSimpleRWLockFallbackBehavior | yes | 4108288 | 4108288 | 0 | 92512 | 92704 | 192 | 3964928 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4108288 | 4108288 | 0 | 92704 | 92816 | 112 | 3964928 |  |
