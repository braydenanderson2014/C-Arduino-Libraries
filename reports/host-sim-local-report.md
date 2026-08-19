# Host Simulation Testing Report

## Summary

- Total runs: 8
- Passed runs: 8
- Failed runs: 0
- Max peak bytes: 4120576
- Avg peak bytes: 4093952
- Compile smoke objects found: 0
- Experimental compile results found: 0
- Experimental compile successes: 0
- Experimental compile failures: 0
- Experimental libraries covered: 0
- Experimental backends covered: 0
- Experimental optional modes covered: 0
- Memory profile runs: 0
- Runs that exceeded limit: 0
- Runs with limit enforcement enabled: 8
- Runs with capacity probe enabled: 0
- Runs where capacity probe reached limit: 0
- Runs with first limit-crossing test identified: 0
- Stress test runs loaded: 4

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
| json-optional-off-littlefs | no | no | no |  |
| json-optional-off-sd | no | no | no |  |
| json-optional-on-littlefs | no | no | no |  |
| json-optional-on-sd | no | no | no |  |
| off-littlefs | no | no | no |  |
| off-sd | no | no | no |  |
| on-littlefs | no | no | no |  |
| on-sd | no | no | no |  |

## Run Results

| Run | Success | Backend | PeakBytes | LimitBytes | LimitExceeded | LimitEnforced | FirstLimitCrossingTest | CrossingPeakBytes | ProbeElementsAtStop | ProbeCurrentBytesAtStop |
| --- | --- | --- | ---: | ---: | --- | --- | --- | ---: | ---: | ---: |
| json-optional-off-littlefs | yes |  | 0 | 0 | no | yes |  | 0 | 0 | 0 |
| json-optional-off-sd | yes |  | 0 | 0 | no | yes |  | 0 | 0 | 0 |
| json-optional-on-littlefs | yes |  | 0 | 0 | no | yes |  | 0 | 0 | 0 |
| json-optional-on-sd | yes |  | 0 | 0 | no | yes |  | 0 | 0 | 0 |
| off-littlefs | yes | LittleFS | 4120576 | 8388608 | no | yes |  | 0 | 0 | 0 |
| off-sd | yes | SD | 4075520 | 8388608 | no | yes |  | 0 | 0 | 0 |
| on-littlefs | yes | LittleFS | 4071424 | 8388608 | no | yes |  | 0 | 0 | 0 |
| on-sd | yes | SD | 4108288 | 8388608 | no | yes |  | 0 | 0 | 0 |

### Per-test Memory Stats: off-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3694592 | 3702784 | 8192 | 675840 | 675840 | 0 | 3706880 |  |
| testHashtableBasicBehavior | yes | 3702784 | 3715072 | 12288 | 675840 | 675840 | 0 | 3719168 |  |
| testSDListMemoryMode | yes | 3719168 | 3731456 | 12288 | 675840 | 675840 | 0 | 3735552 |  |
| testSDListFileIOMode | yes | 3731456 | 3854336 | 122880 | 675840 | 696320 | 20480 | 3858432 |  |
| testCustomStringBehavior | yes | 3854336 | 3858432 | 4096 | 696320 | 696320 | 0 | 3862528 |  |
| testJSONRoundTrip | yes | 3858432 | 3932160 | 73728 | 696320 | 696320 | 0 | 3936256 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3932160 | 3940352 | 8192 | 696320 | 704512 | 8192 | 3944448 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3940352 | 3969024 | 28672 | 704512 | 733184 | 28672 | 4087808 |  |
| testSimpleVectorLifecycleStress | yes | 3969024 | 3981312 | 12288 | 733184 | 745472 | 12288 | 4087808 |  |
| testJSONOptionalFeatureGateBehavior | yes | 3981312 | 3985408 | 4096 | 745472 | 745472 | 0 | 4087808 |  |
| testJSONFileRoundTrip | yes | 3985408 | 4059136 | 73728 | 745472 | 815104 | 69632 | 4087808 |  |
| testSDListPersistenceStress | yes | 4059136 | 4075520 | 16384 | 815104 | 815104 | 0 | 4087808 |  |
| testJSONPersistenceStress | yes | 4075520 | 4091904 | 16384 | 815104 | 815104 | 0 | 4096000 |  |
| testAVLTreeBasicBehavior | yes | 4091904 | 4100096 | 8192 | 815104 | 815104 | 0 | 4104192 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4100096 | 4112384 | 12288 | 815104 | 860160 | 45056 | 4116480 |  |
| testPredicatesBasicBehavior | yes | 4112384 | 4112384 | 0 | 860160 | 860160 | 0 | 4116480 |  |
| testOperatorsBasicBehavior | yes | 4112384 | 4112384 | 0 | 860160 | 860160 | 0 | 4116480 |  |
| testDynamicStorageLibraryRamMode | yes | 4112384 | 4112384 | 0 | 860160 | 860160 | 0 | 4116480 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4112384 | 4112384 | 0 | 860160 | 860160 | 0 | 4116480 |  |
| testSimpleMutexFallbackBehavior | yes | 4112384 | 4116480 | 4096 | 860160 | 860160 | 0 | 4120576 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4116480 | 4116480 | 0 | 860160 | 860160 | 0 | 4120576 |  |
| testSimpleRWLockFallbackBehavior | yes | 4116480 | 4116480 | 0 | 860160 | 860160 | 0 | 4120576 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4116480 | 4116480 | 0 | 860160 | 860160 | 0 | 4120576 |  |

### Per-test Memory Stats: off-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3653632 | 3661824 | 8192 | 626688 | 626688 | 0 | 3665920 |  |
| testHashtableBasicBehavior | yes | 3661824 | 3682304 | 20480 | 626688 | 630784 | 4096 | 3686400 |  |
| testSDListMemoryMode | yes | 3686400 | 3698688 | 12288 | 630784 | 630784 | 0 | 3702784 |  |
| testSDListFileIOMode | yes | 3698688 | 3813376 | 114688 | 630784 | 643072 | 12288 | 3817472 |  |
| testCustomStringBehavior | yes | 3813376 | 3817472 | 4096 | 643072 | 643072 | 0 | 3821568 |  |
| testJSONRoundTrip | yes | 3817472 | 3891200 | 73728 | 643072 | 643072 | 0 | 3895296 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3891200 | 3903488 | 12288 | 643072 | 655360 | 12288 | 3907584 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3903488 | 3928064 | 24576 | 655360 | 679936 | 24576 | 4050944 |  |
| testSimpleVectorLifecycleStress | yes | 3928064 | 3928064 | 0 | 679936 | 692224 | 12288 | 4050944 |  |
| testJSONOptionalFeatureGateBehavior | yes | 3928064 | 3932160 | 4096 | 692224 | 692224 | 0 | 4050944 |  |
| testJSONFileRoundTrip | yes | 3932160 | 4005888 | 73728 | 692224 | 765952 | 73728 | 4050944 |  |
| testSDListPersistenceStress | yes | 4005888 | 4022272 | 16384 | 765952 | 765952 | 0 | 4050944 |  |
| testJSONPersistenceStress | yes | 4022272 | 4038656 | 16384 | 765952 | 765952 | 0 | 4050944 |  |
| testAVLTreeBasicBehavior | yes | 4038656 | 4046848 | 8192 | 765952 | 765952 | 0 | 4050944 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4050944 | 4063232 | 12288 | 765952 | 765952 | 0 | 4067328 |  |
| testPredicatesBasicBehavior | yes | 4063232 | 4063232 | 0 | 765952 | 765952 | 0 | 4067328 |  |
| testOperatorsBasicBehavior | yes | 4063232 | 4063232 | 0 | 765952 | 765952 | 0 | 4067328 |  |
| testDynamicStorageLibraryRamMode | yes | 4063232 | 4063232 | 0 | 765952 | 765952 | 0 | 4067328 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4063232 | 4067328 | 4096 | 765952 | 765952 | 0 | 4071424 |  |
| testSimpleMutexFallbackBehavior | yes | 4067328 | 4071424 | 4096 | 765952 | 765952 | 0 | 4075520 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4071424 | 4071424 | 0 | 765952 | 765952 | 0 | 4075520 |  |
| testSimpleRWLockFallbackBehavior | yes | 4071424 | 4071424 | 0 | 765952 | 765952 | 0 | 4075520 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4071424 | 4059136 | 0 | 765952 | 753664 | 0 | 4075520 |  |

### Per-test Memory Stats: on-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3661824 | 3674112 | 12288 | 638976 | 638976 | 0 | 3678208 |  |
| testHashtableBasicBehavior | yes | 3678208 | 3686400 | 8192 | 638976 | 638976 | 0 | 3690496 |  |
| testSDListMemoryMode | yes | 3686400 | 3702784 | 16384 | 638976 | 638976 | 0 | 3706880 |  |
| testSDListFileIOMode | yes | 3702784 | 3813376 | 110592 | 638976 | 647168 | 8192 | 3817472 |  |
| testCustomStringBehavior | yes | 3813376 | 3817472 | 4096 | 647168 | 647168 | 0 | 3821568 |  |
| testJSONRoundTrip | yes | 3817472 | 3887104 | 69632 | 647168 | 647168 | 0 | 3891200 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3887104 | 3895296 | 8192 | 647168 | 655360 | 8192 | 3899392 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3895296 | 3919872 | 24576 | 655360 | 675840 | 20480 | 4042752 |  |
| testSimpleVectorLifecycleStress | yes | 3919872 | 3923968 | 4096 | 675840 | 675840 | 0 | 4042752 |  |
| testJSONOptionalFeatureGateBehavior | yes | 3923968 | 3928064 | 4096 | 675840 | 675840 | 0 | 4042752 |  |
| testJSONFileRoundTrip | yes | 3928064 | 4001792 | 73728 | 675840 | 761856 | 86016 | 4042752 |  |
| testSDListPersistenceStress | yes | 4001792 | 4022272 | 20480 | 761856 | 761856 | 0 | 4042752 |  |
| testJSONPersistenceStress | yes | 4022272 | 4038656 | 16384 | 761856 | 761856 | 0 | 4042752 |  |
| testAVLTreeBasicBehavior | yes | 4038656 | 4042752 | 4096 | 761856 | 761856 | 0 | 4046848 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4042752 | 4063232 | 20480 | 761856 | 806912 | 45056 | 4067328 |  |
| testPredicatesBasicBehavior | yes | 4063232 | 4063232 | 0 | 806912 | 806912 | 0 | 4067328 |  |
| testOperatorsBasicBehavior | yes | 4063232 | 4063232 | 0 | 806912 | 806912 | 0 | 4067328 |  |
| testDynamicStorageLibraryRamMode | yes | 4063232 | 4063232 | 0 | 806912 | 806912 | 0 | 4067328 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4063232 | 4063232 | 0 | 806912 | 806912 | 0 | 4067328 |  |
| testSimpleMutexFallbackBehavior | yes | 4063232 | 4067328 | 4096 | 806912 | 806912 | 0 | 4071424 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4067328 | 4067328 | 0 | 806912 | 806912 | 0 | 4071424 |  |
| testSimpleRWLockFallbackBehavior | yes | 4067328 | 4067328 | 0 | 806912 | 806912 | 0 | 4071424 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4067328 | 4067328 | 0 | 806912 | 806912 | 0 | 4071424 |  |
| testAVLTreeErrorCodes | yes | 4067328 | 4067328 | 0 | 806912 | 806912 | 0 | 4071424 |  |

### Per-test Memory Stats: on-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 3678208 | 3690496 | 12288 | 667648 | 667648 | 0 | 3694592 |  |
| testHashtableBasicBehavior | yes | 3694592 | 3702784 | 8192 | 667648 | 667648 | 0 | 3706880 |  |
| testSDListMemoryMode | yes | 3702784 | 3719168 | 16384 | 667648 | 667648 | 0 | 3723264 |  |
| testSDListFileIOMode | yes | 3719168 | 3829760 | 110592 | 667648 | 671744 | 4096 | 3833856 |  |
| testCustomStringBehavior | yes | 3829760 | 3833856 | 4096 | 671744 | 671744 | 0 | 3837952 |  |
| testJSONRoundTrip | yes | 3833856 | 3903488 | 69632 | 671744 | 671744 | 0 | 3907584 |  |
| testArrayListChurnAndBoundaryHealth | yes | 3903488 | 3919872 | 16384 | 671744 | 688128 | 16384 | 3923968 |  |
| testHashtableChurnAndBoundaryHealth | yes | 3919872 | 3944448 | 24576 | 688128 | 708608 | 20480 | 4067328 |  |
| testSimpleVectorLifecycleStress | yes | 3944448 | 3948544 | 4096 | 708608 | 708608 | 0 | 4067328 |  |
| testJSONOptionalFeatureGateBehavior | yes | 3948544 | 3952640 | 4096 | 708608 | 708608 | 0 | 4067328 |  |
| testJSONFileRoundTrip | yes | 3952640 | 4026368 | 73728 | 708608 | 794624 | 86016 | 4067328 |  |
| testSDListPersistenceStress | yes | 4026368 | 4046848 | 20480 | 794624 | 794624 | 0 | 4067328 |  |
| testJSONPersistenceStress | yes | 4046848 | 4067328 | 20480 | 794624 | 794624 | 0 | 4071424 |  |
| testAVLTreeBasicBehavior | yes | 4067328 | 4075520 | 8192 | 794624 | 794624 | 0 | 4079616 |  |
| testAVLTreeChurnAndHeightHealth | yes | 4075520 | 4100096 | 24576 | 794624 | 839680 | 45056 | 4104192 |  |
| testPredicatesBasicBehavior | yes | 4100096 | 4100096 | 0 | 839680 | 839680 | 0 | 4104192 |  |
| testOperatorsBasicBehavior | yes | 4100096 | 4100096 | 0 | 839680 | 839680 | 0 | 4104192 |  |
| testDynamicStorageLibraryRamMode | yes | 4100096 | 4100096 | 0 | 839680 | 839680 | 0 | 4104192 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 4100096 | 4100096 | 0 | 839680 | 839680 | 0 | 4104192 |  |
| testSimpleMutexFallbackBehavior | yes | 4100096 | 4104192 | 4096 | 839680 | 839680 | 0 | 4108288 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 4104192 | 4104192 | 0 | 839680 | 839680 | 0 | 4108288 |  |
| testSimpleRWLockFallbackBehavior | yes | 4104192 | 4104192 | 0 | 839680 | 839680 | 0 | 4108288 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 4104192 | 4104192 | 0 | 839680 | 839680 | 0 | 4108288 |  |
| testAVLTreeErrorCodes | yes | 4104192 | 4104192 | 0 | 839680 | 839680 | 0 | 4108288 |  |

## Stress Test Results

# Host Simulation Stress Test Report

## Summary

- Boards profiled: 1
- Total runs processed: 4
- Instance probe types: 14
- Element fill probe types: 27

## Understanding

- Counts shown are the **minimum** across all run variants for that board (most conservative).
- Heap delta from baseline is used for measurement (not absolute process RSS).
- ✓ means the probe stopped because the configured budget was reached.
- A count equal to the configured cap (HOST_STRESS_MAX_INSTANCES / HOST_STRESS_MAX_ELEMENTS) means the probe finished without hitting the budget.
- Budget is taken from each run's `limitBytes` field (CI typically sets it to `sramBytes × 1024`).

## Instance Count Probes

_How many simultaneously-alive empty instances fit within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList_int | SimpleVector_int | Hashtable_int_int | JSON | AVLTree_int | DynamicStorage_String_int | Predicates_int | Operators_int | Stack_int | Queue_int | LinkedList_int | OrderedMap_int_int | Variant_int | ExtremeVariant_int_int |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| uno | 2048 | 16321 ✓ | 20000 | 10977 ✓ | 5473 ✓ | 20000 | 8257 ✓ | 20000 | 20000 | 19697 ✓ | 13137 ✓ | 20000 | 9889 ✓ | 20000 | 20000 |

✓ = budget reached during probe (count is the boundary value)

## Element Fill Probes

_How many elements fit in a single container instance within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList[int] | ArrayList[float] | ArrayList[double] | ArrayList[String] | ArrayList[Optional_int] | SimpleVector[int] | SimpleVector[float] | SimpleVector[double] | SimpleVector[Optional_int] | Hashtable[int_int] | Hashtable[String_String] | AVLTree[int] | DynamicStorage[String_int] | Stack[int] | Stack[float] | Stack[double] | Stack[String] | Queue[int] | Queue[float] | Queue[double] | Queue[String] | LinkedList[int] | LinkedList[float] | LinkedList[double] | LinkedList[String] | OrderedMap[int_int] | OrderedMap[String_String] |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| uno | 2048 | 132609 ✓ | 446977 ✓ | 198657 ✓ | 39425 ✓ | 88321 ✓ | 524289 ✓ | 262145 ✓ | 131073 ✓ | 131073 ✓ | 46081 ✓ | 17153 ✓ | 33025 ✓ | 7681 ✓ | 163841 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 79873 ✓ | 50433 ✓ | 37377 ✓ | 26881 ✓ | 198657 ✓ | 26369 ✓ |

✓ = budget reached during probe (count is the boundary value)

## Per-Board Details

### uno

- SRAM: 2048 bytes
- Host budget: 2097152 bytes (2048 × 1024)
