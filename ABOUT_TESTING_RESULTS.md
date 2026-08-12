# Host Simulation Testing Report

## Summary

- Total runs: 2
- Passed runs: 2
- Failed runs: 0
- Max peak bytes: 3985408
- Avg peak bytes: 3952640
- Compile smoke objects found: 3
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
- Expected compile smoke objects: 3
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
| optional-sd | yes | SD | 3919872 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 3985408 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3612672 | 3612672 | 0 | 77040 | 77312 | 272 | 3465216 |  |
| testHashtableBasicBehavior | yes | 3612672 | 3620864 | 8192 | 77456 | 79648 | 2192 | 3465216 |  |
| testSDListMemoryMode | yes | 3620864 | 3620864 | 0 | 79648 | 79744 | 96 | 3465216 |  |
| testSDListFileIOMode | yes | 3620864 | 3813376 | 192512 | 79744 | 80848 | 1104 | 3657728 |  |
| testCustomStringBehavior | yes | 3813376 | 3813376 | 0 | 80848 | 80848 | 0 | 3657728 |  |
| testJSONRoundTrip | yes | 3813376 | 3948544 | 135168 | 80848 | 82320 | 1472 | 3788800 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3948544 | 3952640 | 4096 | 82320 | 85456 | 3136 | 3788800 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3952640 | 4059136 | 106496 | 85456 | 87120 | 1664 | 3919872 |  |
| testSimpleVectorLifecycleStress | yes | 4059136 | 4059136 | 0 | 87120 | 87120 | 0 | 3919872 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4059136 | 4059136 | 0 | 89184 | 89184 | 0 | 3919872 |  |
| testJSONFileRoundTrip | yes | 4059136 | 4059136 | 0 | 89184 | 89664 | 480 | 3919872 |  |
| testSDListPersistenceStress | yes | 4059136 | 4059136 | 0 | 89616 | 89680 | 64 | 3919872 |  |
| testJSONPersistenceStress | yes | 4059136 | 4059136 | 0 | 89680 | 90384 | 704 | 3919872 |  |
| testAVLTreeBasicBehavior | yes | 4059136 | 4059136 | 0 | 90384 | 90544 | 160 | 3919872 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4059136 | 4059136 | 0 | 90544 | 90784 | 240 | 3919872 |  |
| testPredicatesBasicBehavior | yes | 4059136 | 4059136 | 0 | 90784 | 90784 | 0 | 3919872 |  |
| testOperatorsBasicBehavior | yes | 4059136 | 4059136 | 0 | 90784 | 90784 | 0 | 3919872 |  |
| testDynamicStorageLibraryRamMode | yes | 4059136 | 4059136 | 0 | 92832 | 92832 | 0 | 3919872 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4059136 | 4059136 | 0 | 92832 | 92832 | 0 | 3919872 |  |
| testSimpleMutexFallbackBehavior | yes | 4059136 | 4128768 | 69632 | 92832 | 92832 | 0 | 3919872 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92832 | 92832 | 0 | 3919872 |  |
| testSimpleRWLockFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92832 | 92976 | 144 | 3919872 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4128768 | 4128768 | 0 | 92976 | 93088 | 112 | 3919872 |  |
| testAVLTreeErrorCodes | yes | 4128768 | 4128768 | 0 | 93088 | 93136 | 48 | 3919872 |  |

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
