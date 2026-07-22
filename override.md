# Health Routing Overrides

This file is read by the cross-repo health workflow.

Rules:
- Put a library label under a category section to force that routed bucket.
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
