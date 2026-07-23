# Health Routing Overrides

This file is read by the cross-repo health workflow.

Rules:
- Put a library label under a category section to force that routed bucket.
- A library label may appear under more than one category section if you intentionally want to reuse it across health states.
- When a label appears in multiple category sections, the last matching section in this file wins.
- Tested and Not Tested are separate from health buckets; any library not listed under Tested is treated as Not Tested.
- Add a row to the manual count table to override issue counts for legacy issues.
- If source issues plus PlatformIO produce an unstable lock, category override is ignored and the library stays Unstable.
- Library names must match existing label names in the private repository.

## Stable
- Example Library Name

## Unstable

## Tested

## Not Tested

## Unsure

## Potentially Unhealthy

## Manual Issue Count Overrides
Use exact label names from the private repository.

| Library Label | Confirmed Issues | Open Unconfirmed Issues |
|---|---:|---:|
| Example Library Name | 0 | 0 |
