# Host Simulation Testing Report

## Summary

- Total runs: 2
- Passed runs: 2
- Failed runs: 0
- Max peak bytes: 4087808
- Avg peak bytes: 4065280
- Compile smoke objects found: 2
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
- Expected compile smoke objects: 2
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
| optional-sd | yes | SD | 4087808 | 8388608 | no | yes |  | 0 | 0 | 0 |
| sd | yes | SD | 4042752 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: optional-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3653632 | 3653632 | 0 | 77040 | 77312 | 272 | 3502080 |  |
| testHashtableBasicBehavior | yes | 3653632 | 3661824 | 8192 | 77456 | 79648 | 2192 | 3502080 |  |
| testSDListMemoryMode | yes | 3661824 | 3661824 | 0 | 79648 | 79744 | 96 | 3502080 |  |
| testSDListFileIOMode | yes | 3661824 | 3854336 | 192512 | 79744 | 80848 | 1104 | 3694592 |  |
| testCustomStringBehavior | yes | 3854336 | 3854336 | 0 | 80848 | 80848 | 0 | 3694592 |  |
| testJSONRoundTrip | yes | 3854336 | 3989504 | 135168 | 80848 | 82320 | 1472 | 3825664 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3989504 | 3993600 | 4096 | 82320 | 85456 | 3136 | 3825664 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3993600 | 4100096 | 106496 | 85456 | 87120 | 1664 | 3956736 |  |
| testSimpleVectorLifecycleStress | yes | 4100096 | 4100096 | 0 | 87120 | 87120 | 0 | 3956736 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4100096 | 4100096 | 0 | 89184 | 89184 | 0 | 3956736 |  |
| testJSONFileRoundTrip | yes | 4100096 | 4100096 | 0 | 89184 | 89664 | 480 | 3956736 |  |
| testSDListPersistenceStress | yes | 4100096 | 4100096 | 0 | 89616 | 89680 | 64 | 3956736 |  |
| testJSONPersistenceStress | yes | 4100096 | 4100096 | 0 | 89680 | 90384 | 704 | 3956736 |  |
| testAVLTreeBasicBehavior | yes | 4100096 | 4100096 | 0 | 90384 | 90544 | 160 | 3956736 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4100096 | 4100096 | 0 | 90544 | 90784 | 240 | 3956736 |  |
| testPredicatesBasicBehavior | yes | 4100096 | 4100096 | 0 | 90784 | 90784 | 0 | 3956736 |  |
| testOperatorsBasicBehavior | yes | 4100096 | 4100096 | 0 | 90784 | 90784 | 0 | 3956736 |  |
| testDynamicStorageLibraryRamMode | yes | 4100096 | 4100096 | 0 | 92832 | 92832 | 0 | 3956736 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4100096 | 4100096 | 0 | 92832 | 92832 | 0 | 3956736 |  |
| testSimpleMutexFallbackBehavior | yes | 4100096 | 4169728 | 69632 | 92832 | 92832 | 0 | 4087808 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92832 | 92832 | 0 | 4087808 |  |
| testSimpleRWLockFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92832 | 92976 | 144 | 4087808 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4169728 | 4169728 | 0 | 92976 | 93088 | 112 | 4087808 |  |
| testAVLTreeErrorCodes | yes | 4169728 | 4169728 | 0 | 93088 | 93136 | 48 | 4087808 |  |

### Per-test Memory Stats: sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3604480 | 3604480 | 0 | 76928 | 77200 | 272 | 3457024 |  |
| testHashtableBasicBehavior | yes | 3604480 | 3612672 | 8192 | 77344 | 79536 | 2192 | 3457024 |  |
| testSDListMemoryMode | yes | 3612672 | 3612672 | 0 | 79536 | 79632 | 96 | 3457024 |  |
| testSDListFileIOMode | yes | 3612672 | 3805184 | 192512 | 79632 | 80752 | 1120 | 3649536 |  |
| testCustomStringBehavior | yes | 3805184 | 3805184 | 0 | 80752 | 80752 | 0 | 3649536 |  |
| testJSONRoundTrip | yes | 3805184 | 3940352 | 135168 | 80752 | 82272 | 1520 | 3780608 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3940352 | 3944448 | 4096 | 82272 | 85312 | 3040 | 3780608 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3944448 | 4050944 | 106496 | 85312 | 86832 | 1520 | 3911680 |  |
| testSimpleVectorLifecycleStress | yes | 4050944 | 4050944 | 0 | 86832 | 86832 | 0 | 3911680 |  |
| testJSONOptionalFeatureGateBehavior | yes | 4050944 | 4050944 | 0 | 88896 | 88896 | 0 | 3911680 |  |
| testJSONFileRoundTrip | yes | 4050944 | 4050944 | 0 | 88896 | 89152 | 256 | 3911680 |  |
| testSDListPersistenceStress | yes | 4050944 | 4050944 | 0 | 89152 | 89504 | 352 | 3911680 |  |
| testJSONPersistenceStress | yes | 4050944 | 4050944 | 0 | 89504 | 90320 | 816 | 3911680 |  |
| testAVLTreeBasicBehavior | yes | 4050944 | 4050944 | 0 | 90320 | 90368 | 48 | 3911680 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4050944 | 4050944 | 0 | 90368 | 90464 | 96 | 3911680 |  |
| testPredicatesBasicBehavior | yes | 4050944 | 4050944 | 0 | 90464 | 90464 | 0 | 3911680 |  |
| testOperatorsBasicBehavior | yes | 4050944 | 4050944 | 0 | 90464 | 90464 | 0 | 3911680 |  |
| testDynamicStorageLibraryRamMode | yes | 4050944 | 4050944 | 0 | 92512 | 92512 | 0 | 3911680 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4050944 | 4050944 | 0 | 92512 | 92512 | 0 | 3911680 |  |
| testSimpleMutexFallbackBehavior | yes | 4050944 | 4120576 | 69632 | 92512 | 92512 | 0 | 4042752 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4120576 | 4120576 | 0 | 92512 | 92512 | 0 | 4042752 |  |
| testSimpleRWLockFallbackBehavior | yes | 4120576 | 4120576 | 0 | 92512 | 92704 | 192 | 4042752 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4120576 | 4120576 | 0 | 92704 | 92816 | 112 | 4042752 |  |
