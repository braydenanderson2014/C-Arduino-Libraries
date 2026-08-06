# Host Simulation Testing Report

## Summary

- Total runs: 8
- Passed runs: 8
- Failed runs: 0
- Max peak bytes: 9330688
- Avg peak bytes: 9279488
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
- Runs with first limit-crossing test identified: 4
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
| off-littlefs | no | no | no | testArrayListBasicBehavior |
| off-sd | no | no | no | testArrayListBasicBehavior |
| on-littlefs | no | no | no | testArrayListBasicBehavior |
| on-sd | no | no | no | testArrayListBasicBehavior |

## Run Results

| Run | Success | Backend | PeakBytes | LimitBytes | LimitExceeded | LimitEnforced | FirstLimitCrossingTest | CrossingPeakBytes | ProbeElementsAtStop | ProbeCurrentBytesAtStop |
| --- | --- | --- | ---: | ---: | --- | --- | --- | ---: | ---: | ---: |
| json-optional-off-littlefs | yes |  | 0 | 0 | no | yes |  | 0 | 0 | 0 |
| json-optional-off-sd | yes |  | 0 | 0 | no | yes |  | 0 | 0 | 0 |
| json-optional-on-littlefs | yes |  | 0 | 0 | no | yes |  | 0 | 0 | 0 |
| json-optional-on-sd | yes |  | 0 | 0 | no | yes |  | 0 | 0 | 0 |
| off-littlefs | yes | LittleFS | 9285632 | 8388608 | no | yes | testArrayListBasicBehavior | 8671232 | 0 | 0 |
| off-sd | yes | SD | 9256960 | 8388608 | no | yes | testArrayListBasicBehavior | 8642560 | 0 | 0 |
| on-littlefs | yes | LittleFS | 9330688 | 8388608 | no | yes | testArrayListBasicBehavior | 8663040 | 0 | 0 |
| on-sd | yes | SD | 9244672 | 8388608 | no | yes | testArrayListBasicBehavior | 8663040 | 0 | 0 |

### Per-test Memory Stats: off-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 8667136 | 8671232 | 4096 | 3620864 | 3620864 | 0 | 8671232 |  |
| testHashtableBasicBehavior | yes | 8675328 | 8683520 | 8192 | 3620864 | 3620864 | 0 | 8683520 |  |
| testSDListMemoryMode | yes | 8683520 | 8695808 | 12288 | 3620864 | 3620864 | 0 | 8695808 |  |
| testSDListFileIOMode | yes | 8695808 | 8929280 | 233472 | 3620864 | 3629056 | 8192 | 8929280 |  |
| testCustomStringBehavior | yes | 8929280 | 8941568 | 12288 | 3629056 | 3629056 | 0 | 8941568 |  |
| testJSONRoundTrip | yes | 8941568 | 9019392 | 77824 | 3629056 | 3629056 | 0 | 9019392 |  |
| testArrayListChurnAndBoundaryHealth | yes | 9019392 | 9035776 | 16384 | 3629056 | 3637248 | 8192 | 9035776 |  |
| testHashtableChurnAndBoundaryHealth | yes | 9035776 | 9097216 | 61440 | 3637248 | 3735552 | 98304 | 9187328 |  |
| testSimpleVectorLifecycleStress | yes | 9097216 | 9101312 | 4096 | 3735552 | 3764224 | 28672 | 9187328 |  |
| testJSONOptionalFeatureGateBehavior | yes | 9101312 | 9101312 | 0 | 3764224 | 3764224 | 0 | 9187328 |  |
| testJSONFileRoundTrip | yes | 9101312 | 9166848 | 65536 | 3764224 | 3764224 | 0 | 9187328 |  |
| testSDListPersistenceStress | yes | 9166848 | 9232384 | 65536 | 3764224 | 3764224 | 0 | 9232384 |  |
| testJSONPersistenceStress | yes | 9232384 | 9248768 | 16384 | 3764224 | 3764224 | 0 | 9248768 |  |
| testAVLTreeBasicBehavior | yes | 9248768 | 9256960 | 8192 | 3764224 | 3764224 | 0 | 9256960 |  |
| testAVLTreeChurnAndHeightHealth | yes | 9256960 | 9273344 | 16384 | 3764224 | 3764224 | 0 | 9273344 |  |
| testPredicatesBasicBehavior | yes | 9273344 | 9277440 | 4096 | 3764224 | 3764224 | 0 | 9277440 |  |
| testOperatorsBasicBehavior | yes | 9277440 | 9277440 | 0 | 3764224 | 3764224 | 0 | 9277440 |  |
| testDynamicStorageLibraryRamMode | yes | 9277440 | 9281536 | 4096 | 3764224 | 3764224 | 0 | 9281536 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 9281536 | 9281536 | 0 | 3764224 | 3764224 | 0 | 9281536 |  |
| testSimpleMutexFallbackBehavior | yes | 9281536 | 9281536 | 0 | 3764224 | 3764224 | 0 | 9281536 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 9281536 | 9285632 | 4096 | 3764224 | 3764224 | 0 | 9285632 |  |
| testSimpleRWLockFallbackBehavior | yes | 9285632 | 9285632 | 0 | 3764224 | 3764224 | 0 | 9285632 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 9285632 | 9285632 | 0 | 3764224 | 3764224 | 0 | 9285632 |  |

### Per-test Memory Stats: off-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 8638464 | 8642560 | 4096 | 3629056 | 3629056 | 0 | 8642560 |  |
| testHashtableBasicBehavior | yes | 8646656 | 8654848 | 8192 | 3629056 | 3629056 | 0 | 8654848 |  |
| testSDListMemoryMode | yes | 8654848 | 8667136 | 12288 | 3629056 | 3629056 | 0 | 8667136 |  |
| testSDListFileIOMode | yes | 8667136 | 8871936 | 204800 | 3629056 | 3629056 | 0 | 8871936 |  |
| testCustomStringBehavior | yes | 8871936 | 8880128 | 8192 | 3629056 | 3629056 | 0 | 8880128 |  |
| testJSONRoundTrip | yes | 8880128 | 8957952 | 77824 | 3629056 | 3629056 | 0 | 8957952 |  |
| testArrayListChurnAndBoundaryHealth | yes | 8957952 | 8978432 | 20480 | 3629056 | 3641344 | 12288 | 8978432 |  |
| testHashtableChurnAndBoundaryHealth | yes | 8978432 | 9056256 | 77824 | 3641344 | 3735552 | 94208 | 9134080 |  |
| testSimpleVectorLifecycleStress | yes | 9056256 | 9056256 | 0 | 3735552 | 3735552 | 0 | 9134080 |  |
| testJSONOptionalFeatureGateBehavior | yes | 9056256 | 9056256 | 0 | 3735552 | 3735552 | 0 | 9134080 |  |
| testJSONFileRoundTrip | yes | 9056256 | 9129984 | 73728 | 3735552 | 3743744 | 8192 | 9134080 |  |
| testSDListPersistenceStress | yes | 9129984 | 9183232 | 53248 | 3743744 | 3743744 | 0 | 9183232 |  |
| testJSONPersistenceStress | yes | 9183232 | 9220096 | 36864 | 3743744 | 3743744 | 0 | 9220096 |  |
| testAVLTreeBasicBehavior | yes | 9220096 | 9228288 | 8192 | 3743744 | 3743744 | 0 | 9228288 |  |
| testAVLTreeChurnAndHeightHealth | yes | 9228288 | 9240576 | 12288 | 3743744 | 3768320 | 24576 | 9240576 |  |
| testPredicatesBasicBehavior | yes | 9240576 | 9244672 | 4096 | 3768320 | 3768320 | 0 | 9244672 |  |
| testOperatorsBasicBehavior | yes | 9244672 | 9244672 | 0 | 3768320 | 3768320 | 0 | 9244672 |  |
| testDynamicStorageLibraryRamMode | yes | 9244672 | 9252864 | 8192 | 3768320 | 3768320 | 0 | 9252864 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 9252864 | 9252864 | 0 | 3768320 | 3768320 | 0 | 9252864 |  |
| testSimpleMutexFallbackBehavior | yes | 9252864 | 9252864 | 0 | 3768320 | 3768320 | 0 | 9252864 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 9252864 | 9256960 | 4096 | 3768320 | 3768320 | 0 | 9256960 |  |
| testSimpleRWLockFallbackBehavior | yes | 9256960 | 9256960 | 0 | 3768320 | 3768320 | 0 | 9256960 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 9256960 | 9256960 | 0 | 3768320 | 3768320 | 0 | 9256960 |  |

### Per-test Memory Stats: on-littlefs

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 8650752 | 8663040 | 12288 | 3612672 | 3612672 | 0 | 8663040 |  |
| testHashtableBasicBehavior | yes | 8667136 | 8671232 | 4096 | 3612672 | 3612672 | 0 | 8671232 |  |
| testSDListMemoryMode | yes | 8671232 | 8675328 | 4096 | 3612672 | 3612672 | 0 | 8675328 |  |
| testSDListFileIOMode | yes | 8675328 | 8896512 | 221184 | 3612672 | 3620864 | 8192 | 8896512 |  |
| testCustomStringBehavior | yes | 8896512 | 8912896 | 16384 | 3620864 | 3620864 | 0 | 8912896 |  |
| testJSONRoundTrip | yes | 8912896 | 8990720 | 77824 | 3620864 | 3620864 | 0 | 8990720 |  |
| testArrayListChurnAndBoundaryHealth | yes | 8990720 | 9011200 | 20480 | 3620864 | 3633152 | 12288 | 9011200 |  |
| testHashtableChurnAndBoundaryHealth | yes | 9011200 | 9089024 | 77824 | 3633152 | 3760128 | 126976 | 9175040 |  |
| testSimpleVectorLifecycleStress | yes | 9089024 | 9093120 | 4096 | 3760128 | 3760128 | 0 | 9175040 |  |
| testJSONOptionalFeatureGateBehavior | yes | 9093120 | 9093120 | 0 | 3760128 | 3760128 | 0 | 9175040 |  |
| testJSONFileRoundTrip | yes | 9093120 | 9162752 | 69632 | 3760128 | 3784704 | 24576 | 9175040 |  |
| testSDListPersistenceStress | yes | 9162752 | 9273344 | 110592 | 3784704 | 3784704 | 0 | 9273344 |  |
| testJSONPersistenceStress | yes | 9273344 | 9297920 | 24576 | 3784704 | 3784704 | 0 | 9297920 |  |
| testAVLTreeBasicBehavior | yes | 9297920 | 9306112 | 8192 | 3784704 | 3784704 | 0 | 9306112 |  |
| testAVLTreeChurnAndHeightHealth | yes | 9306112 | 9322496 | 16384 | 3784704 | 3784704 | 0 | 9322496 |  |
| testPredicatesBasicBehavior | yes | 9322496 | 9322496 | 0 | 3784704 | 3784704 | 0 | 9322496 |  |
| testOperatorsBasicBehavior | yes | 9322496 | 9322496 | 0 | 3784704 | 3784704 | 0 | 9322496 |  |
| testDynamicStorageLibraryRamMode | yes | 9322496 | 9326592 | 4096 | 3784704 | 3784704 | 0 | 9326592 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 9326592 | 9326592 | 0 | 3784704 | 3784704 | 0 | 9326592 |  |
| testSimpleMutexFallbackBehavior | yes | 9326592 | 9330688 | 4096 | 3784704 | 3784704 | 0 | 9330688 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 9330688 | 9330688 | 0 | 3784704 | 3784704 | 0 | 9330688 |  |
| testSimpleRWLockFallbackBehavior | yes | 9330688 | 9330688 | 0 | 3784704 | 3784704 | 0 | 9330688 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 9330688 | 9330688 | 0 | 3784704 | 3784704 | 0 | 9330688 |  |
| testAVLTreeErrorCodes | yes | 9330688 | 9330688 | 0 | 3784704 | 3784704 | 0 | 9330688 |  |

### Per-test Memory Stats: on-sd

| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| testArrayListBasicBehavior | yes | 8650752 | 8663040 | 12288 | 3620864 | 3620864 | 0 | 8663040 |  |
| testHashtableBasicBehavior | yes | 8667136 | 8675328 | 8192 | 3620864 | 3620864 | 0 | 8675328 |  |
| testSDListMemoryMode | yes | 8675328 | 8683520 | 8192 | 3620864 | 3620864 | 0 | 8683520 |  |
| testSDListFileIOMode | yes | 8683520 | 8892416 | 208896 | 3620864 | 3620864 | 0 | 8892416 |  |
| testCustomStringBehavior | yes | 8892416 | 8904704 | 12288 | 3620864 | 3620864 | 0 | 8904704 |  |
| testJSONRoundTrip | yes | 8904704 | 8982528 | 77824 | 3620864 | 3620864 | 0 | 8982528 |  |
| testArrayListChurnAndBoundaryHealth | yes | 8982528 | 9003008 | 20480 | 3620864 | 3633152 | 12288 | 9003008 |  |
| testHashtableChurnAndBoundaryHealth | yes | 9003008 | 9068544 | 65536 | 3633152 | 3723264 | 90112 | 9150464 |  |
| testSimpleVectorLifecycleStress | yes | 9068544 | 9076736 | 8192 | 3723264 | 3727360 | 4096 | 9150464 |  |
| testJSONOptionalFeatureGateBehavior | yes | 9076736 | 9076736 | 0 | 3727360 | 3727360 | 0 | 9150464 |  |
| testJSONFileRoundTrip | yes | 9076736 | 9138176 | 61440 | 3727360 | 3735552 | 8192 | 9150464 |  |
| testSDListPersistenceStress | yes | 9138176 | 9195520 | 57344 | 3735552 | 3735552 | 0 | 9195520 |  |
| testJSONPersistenceStress | yes | 9195520 | 9211904 | 16384 | 3735552 | 3735552 | 0 | 9211904 |  |
| testAVLTreeBasicBehavior | yes | 9211904 | 9220096 | 8192 | 3735552 | 3735552 | 0 | 9220096 |  |
| testAVLTreeChurnAndHeightHealth | yes | 9220096 | 9232384 | 12288 | 3735552 | 3735552 | 0 | 9232384 |  |
| testPredicatesBasicBehavior | yes | 9232384 | 9232384 | 0 | 3735552 | 3735552 | 0 | 9232384 |  |
| testOperatorsBasicBehavior | yes | 9232384 | 9232384 | 0 | 3735552 | 3735552 | 0 | 9232384 |  |
| testDynamicStorageLibraryRamMode | yes | 9232384 | 9240576 | 8192 | 3735552 | 3735552 | 0 | 9240576 |  |
| testSimpleThreadManagerFallbackBehavior | yes | 9240576 | 9240576 | 0 | 3735552 | 3735552 | 0 | 9240576 |  |
| testSimpleMutexFallbackBehavior | yes | 9240576 | 9244672 | 4096 | 3735552 | 3735552 | 0 | 9244672 |  |
| testSimpleSemaphoreFallbackBehavior | yes | 9244672 | 9244672 | 0 | 3735552 | 3735552 | 0 | 9244672 |  |
| testSimpleRWLockFallbackBehavior | yes | 9244672 | 9244672 | 0 | 3735552 | 3735552 | 0 | 9244672 |  |
| testSimpleCoreRuntimeFallbackBehavior | yes | 9244672 | 9244672 | 0 | 3735552 | 3735552 | 0 | 9244672 |  |
| testAVLTreeErrorCodes | yes | 9244672 | 9244672 | 0 | 3735552 | 3735552 | 0 | 9244672 |  |

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
| uno | 2048 | 16385 ✓ | 20000 | 5473 ✓ | 9153 ✓ | 16385 ✓ | 6401 ✓ | 20000 | 20000 | 14001 ✓ | 11105 ✓ | 20000 | 5089 ✓ | 20000 | 20000 |

✓ = budget reached during probe (count is the boundary value)

## Element Fill Probes

_How many elements fit in a single container instance within each board's SRAM budget._

| Board | SRAM (bytes) | ArrayList[int] | ArrayList[float] | ArrayList[double] | ArrayList[String] | ArrayList[Optional_int] | SimpleVector[int] | SimpleVector[float] | SimpleVector[double] | SimpleVector[Optional_int] | Hashtable[int_int] | Hashtable[String_String] | AVLTree[int] | DynamicStorage[String_int] | Stack[int] | Stack[float] | Stack[double] | Stack[String] | Queue[int] | Queue[float] | Queue[double] | Queue[String] | LinkedList[int] | LinkedList[float] | LinkedList[double] | LinkedList[String] | OrderedMap[int_int] | OrderedMap[String_String] |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| uno | 2048 | 297985 ✓ | 132609 ✓ | 198657 ✓ | 26369 ✓ | 132609 ✓ | 524289 ✓ | 262145 ✓ | 131073 ✓ | 131073 ✓ | 46081 ✓ | 11521 ✓ | 25345 ✓ | 9985 ✓ | 163841 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 327681 ✓ | 327681 ✓ | 163841 ✓ | 40961 ✓ | 86017 ✓ | 53505 ✓ | 40449 ✓ | 26881 ✓ | 88321 ✓ | 17665 ✓ |

✓ = budget reached during probe (count is the boundary value)

## Per-Board Details

### uno

- SRAM: 2048 bytes
- Host budget: 2097152 bytes (2048 × 1024)
