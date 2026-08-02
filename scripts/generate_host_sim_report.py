#!/usr/bin/env python3
import argparse
import json
from pathlib import Path
from statistics import mean
from typing import Any, Dict, List, Optional, Tuple


def load_json(path: Path) -> Dict[str, Any]:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except Exception:
        return {}


# ─── Stress-mode helpers ──────────────────────────────────────────────────────

def load_stress_runs(artifacts_dir: Path) -> List[Dict[str, Any]]:
    """Load all stress-*.json files from any subdirectory of artifacts_dir."""
    runs: List[Dict[str, Any]] = []
    for path in sorted(artifacts_dir.rglob("stress-*.json")):
        if path.is_file():
            data = load_json(path)
            if data:
                runs.append(data)
    return runs


def load_experimental_compile_results(artifacts_dir: Path) -> List[Dict[str, Any]]:
    """Load all experimental-compile-result.json files from any artifact subdirectory."""
    results: List[Dict[str, Any]] = []
    for path in sorted(artifacts_dir.rglob("experimental-compile-result.json")):
        if path.is_file():
            data = load_json(path)
            if data:
                results.append(data)
    return results


def _aggregate_stress_by_board(runs: List[Dict[str, Any]]) -> Dict[str, Dict[str, Any]]:
    """Per-board: keep the minimum (most conservative) count across all variants."""
    board_data: Dict[str, Dict[str, Any]] = {}
    for run in runs:
        board     = str(run.get("board", "unknown"))
        sram      = int(run.get("sramBytes", 0))
        limit     = int(run.get("limitBytes", 0))

        if board not in board_data:
            board_data[board] = {
                "sramBytes":           sram,
                "limitBytes":          limit,
                "instanceCounts":      {},
                "instanceLimitReached":{},
                "fillCounts":          {},
                "fillLimitReached":    {},
            }

        for p in run.get("instanceCountProbes", []):
            t   = str(p.get("type", ""))
            cnt = int(p.get("maxInstances", 0))
            if not t:
                continue
            existing = board_data[board]["instanceCounts"].get(t)
            if existing is None or cnt < existing:
                board_data[board]["instanceCounts"][t]       = cnt
                board_data[board]["instanceLimitReached"][t] = bool(p.get("limitReached", False))

        for p in run.get("elementFillProbes", []):
            container = str(p.get("container", ""))
            elem      = str(p.get("elementType", ""))
            key       = f"{container}[{elem}]"
            cnt       = int(p.get("maxElements", 0))
            if not (container and elem):
                continue
            existing = board_data[board]["fillCounts"].get(key)
            if existing is None or cnt < existing:
                board_data[board]["fillCounts"][key]       = cnt
                board_data[board]["fillLimitReached"][key] = bool(p.get("limitReached", False))

    return board_data


def generate_markdown_stress(runs: List[Dict[str, Any]]) -> str:
    lines: List[str] = []
    lines.append("# Host Simulation Stress Test Report")
    lines.append("")

    if not runs:
        lines.append("_No stress test results found._")
        lines.append("")
        return "\n".join(lines)

    board_data = _aggregate_stress_by_board(runs)
    boards     = sorted(board_data.keys())

    # Collect probe type lists in insertion order, deduped.
    instance_types: List[str] = []
    fill_keys:      List[str] = []
    for run in runs:
        for p in run.get("instanceCountProbes", []):
            t = str(p.get("type", ""))
            if t and t not in instance_types:
                instance_types.append(t)
        for p in run.get("elementFillProbes", []):
            container = str(p.get("container", ""))
            elem      = str(p.get("elementType", ""))
            key       = f"{container}[{elem}]"
            if container and elem and key not in fill_keys:
                fill_keys.append(key)

    # Summary
    lines.append("## Summary")
    lines.append("")
    lines.append(f"- Boards profiled: {len(boards)}")
    lines.append(f"- Total runs processed: {len(runs)}")
    lines.append(f"- Instance probe types: {len(instance_types)}")
    lines.append(f"- Element fill probe types: {len(fill_keys)}")
    lines.append("")
    lines.append("## Understanding")
    lines.append("")
    lines.append("- Counts shown are the **minimum** across all run variants for that board (most conservative).")
    lines.append("- Heap delta from baseline is used for measurement (not absolute process RSS).")
    lines.append("- ✓ means the probe stopped because the configured budget was reached.")
    lines.append("- A count equal to the configured cap (HOST_STRESS_MAX_INSTANCES / HOST_STRESS_MAX_ELEMENTS) means the probe finished without hitting the budget.")
    lines.append("- Budget is taken from each run's `limitBytes` field (CI typically sets it to `sramBytes × 1024`).")
    lines.append("")

    # Instance count table
    if instance_types:
        lines.append("## Instance Count Probes")
        lines.append("")
        lines.append("_How many simultaneously-alive empty instances fit within each board's SRAM budget._")
        lines.append("")
        header = "| Board | SRAM (bytes) |"
        sep    = "| --- | ---: |"
        for t in instance_types:
            header += f" {t} |"
            sep    += " ---: |"
        lines.append(header)
        lines.append(sep)
        for board in boards:
            bd  = board_data[board]
            row = f"| {board} | {bd['sramBytes']} |"
            for t in instance_types:
                cnt     = bd["instanceCounts"].get(t)
                reached = bd["instanceLimitReached"].get(t, False)
                marker  = " ✓" if reached else ""
                row    += f" {cnt}{marker} |" if cnt is not None else " — |"
            lines.append(row)
        lines.append("")
        lines.append("✓ = budget reached during probe (count is the boundary value)")
        lines.append("")

    # Element fill table
    if fill_keys:
        lines.append("## Element Fill Probes")
        lines.append("")
        lines.append("_How many elements fit in a single container instance within each board's SRAM budget._")
        lines.append("")
        header = "| Board | SRAM (bytes) |"
        sep    = "| --- | ---: |"
        for k in fill_keys:
            header += f" {k} |"
            sep    += " ---: |"
        lines.append(header)
        lines.append(sep)
        for board in boards:
            bd  = board_data[board]
            row = f"| {board} | {bd['sramBytes']} |"
            for k in fill_keys:
                cnt     = bd["fillCounts"].get(k)
                reached = bd["fillLimitReached"].get(k, False)
                marker  = " ✓" if reached else ""
                row    += f" {cnt}{marker} |" if cnt is not None else " — |"
            lines.append(row)
        lines.append("")
        lines.append("✓ = budget reached during probe (count is the boundary value)")
        lines.append("")

    # Per-board detail
    lines.append("## Per-Board Details")
    lines.append("")
    for board in boards:
        bd = board_data[board]
        lines.append(f"### {board}")
        lines.append("")
        lines.append(f"- SRAM: {bd['sramBytes']} bytes")
        lines.append(f"- Host budget: {bd['limitBytes']} bytes ({bd['sramBytes']} × 1024)")
        lines.append("")

    return "\n".join(lines)


def classify_paths(root: Path) -> Tuple[List[Path], List[Path], List[Path]]:
    report_files: List[Path] = []
    stats_files: List[Path] = []
    smoke_objects: List[Path] = []

    for path in root.rglob("*"):
        if not path.is_file():
            continue
        name = path.name
        if name.startswith("report") and name.endswith(".json"):
            report_files.append(path)
        elif name.startswith("stats") and name.endswith(".json"):
            stats_files.append(path)
        elif name == "smoke.o":
            smoke_objects.append(path)

    return sorted(report_files), sorted(stats_files), sorted(smoke_objects)


def key_from_filename(path: Path) -> str:
    stem = path.stem
    for prefix in ("report-", "stats-"):
        if stem.startswith(prefix):
            return stem[len(prefix):]
    return stem


def build_run_index(report_files: List[Path], stats_files: List[Path]) -> Dict[str, Dict[str, Any]]:
    runs: Dict[str, Dict[str, Any]] = {}

    for p in report_files:
        key = key_from_filename(p)
        runs.setdefault(key, {})["report"] = load_json(p)
        runs[key]["reportPath"] = p.as_posix()

    for p in stats_files:
        key = key_from_filename(p)
        runs.setdefault(key, {})["stats"] = load_json(p)
        runs[key]["statsPath"] = p.as_posix()

    return runs


def find_first_limit_crossing_test(tests: Any, limit_bytes: int) -> Optional[Dict[str, Any]]:
    if not isinstance(tests, list) or limit_bytes <= 0:
        return None

    for test in tests:
        if not isinstance(test, dict):
            continue
        peak_after = test.get("peakBytesAfterTest", 0)
        after_current = test.get("afterCurrentBytes", 0)
        if isinstance(peak_after, int) and peak_after >= limit_bytes:
            return {
                "name": str(test.get("name", "")),
                "peakBytesAfterTest": peak_after,
                "afterCurrentBytes": after_current if isinstance(after_current, int) else 0,
            }
        if isinstance(after_current, int) and after_current >= limit_bytes:
            return {
                "name": str(test.get("name", "")),
                "peakBytesAfterTest": peak_after if isinstance(peak_after, int) else 0,
                "afterCurrentBytes": after_current,
            }

    return None


def analyze_runs(runs: Dict[str, Dict[str, Any]]) -> None:
    for key in sorted(runs.keys()):
        run_data = runs[key]
        report = run_data.get("report", {})
        stats = run_data.get("stats", {})

        report_memory = report.get("memory", {}) if isinstance(report, dict) else {}
        stats_memory = stats.get("memory", {}) if isinstance(stats, dict) else {}
        probe = stats.get("capacityProbe", {}) if isinstance(stats, dict) else {}

        limit_bytes = report_memory.get("limitBytes", stats_memory.get("limitBytes", 0))
        if not isinstance(limit_bytes, int):
            limit_bytes = 0

        tests = stats.get("tests", []) if isinstance(stats, dict) else []
        first_crossing = find_first_limit_crossing_test(tests, limit_bytes)

        run_data["analysis"] = {
            "isMemoryProfileRun": key.startswith("memory-") or bool(probe.get("enabled", False)),
            "hasPerTestStats": isinstance(tests, list) and len(tests) > 0,
            "firstLimitCrossingTest": first_crossing,
            "limitExceeded": bool(stats_memory.get("limitExceeded", False)),
            "limitEnforced": bool(stats_memory.get("limitEnforced", True)),
            "capacityProbeEnabled": bool(probe.get("enabled", False)),
            "capacityProbeLimitReached": bool(probe.get("limitReached", False)),
        }


def summarize(runs: Dict[str, Dict[str, Any]], smoke_count: int) -> Dict[str, Any]:
    total = len(runs)
    passed = 0
    failed = 0
    peaks: List[int] = []
    limit_exceeded_runs = 0
    limit_enforced_runs = 0
    memory_profile_runs = 0
    probe_enabled_runs = 0
    probe_limit_reached_runs = 0
    first_crossing_runs = 0

    for run_data in runs.values():
        report = run_data.get("report", {})
        success = bool(report.get("success", False))
        if success:
            passed += 1
        else:
            failed += 1

        memory = report.get("memory", {})
        peak = memory.get("peakBytes")
        if isinstance(peak, int):
            peaks.append(peak)

        analysis = run_data.get("analysis", {})
        if bool(analysis.get("isMemoryProfileRun", False)):
            memory_profile_runs += 1
        if bool(analysis.get("limitExceeded", False)):
            limit_exceeded_runs += 1
        if bool(analysis.get("limitEnforced", False)):
            limit_enforced_runs += 1
        if bool(analysis.get("capacityProbeEnabled", False)):
            probe_enabled_runs += 1
        if bool(analysis.get("capacityProbeLimitReached", False)):
            probe_limit_reached_runs += 1
        if analysis.get("firstLimitCrossingTest"):
            first_crossing_runs += 1

    return {
        "totalRuns": total,
        "passedRuns": passed,
        "failedRuns": failed,
        "maxPeakBytes": max(peaks) if peaks else 0,
        "avgPeakBytes": int(mean(peaks)) if peaks else 0,
        "compileSmokeObjectCount": smoke_count,
        "memoryProfileRuns": memory_profile_runs,
        "limitExceededRuns": limit_exceeded_runs,
        "limitEnforcedRuns": limit_enforced_runs,
        "capacityProbeEnabledRuns": probe_enabled_runs,
        "capacityProbeLimitReachedRuns": probe_limit_reached_runs,
        "runsWithFirstLimitCrossingTest": first_crossing_runs,
    }


def summarize_experimental_compile_results(results: List[Dict[str, Any]]) -> Dict[str, Any]:
    total = len(results)
    passed = 0
    failed = 0
    failures: List[Dict[str, Any]] = []

    for result in results:
        success = bool(result.get("success", False))
        if success:
            passed += 1
            continue

        failed += 1
        failures.append({
            "libraryPath": str(result.get("libraryPath", "")),
            "backend": str(result.get("backend", "")),
            "optional": str(result.get("optional", "")),
            "returnCode": int(result.get("returnCode", -1)) if isinstance(result.get("returnCode", -1), int) else -1,
        })

    return {
        "total": total,
        "passed": passed,
        "failed": failed,
        "failures": failures,
    }


def fmt_bool(value: Any) -> str:
    return "yes" if bool(value) else "no"


def generate_markdown(
    mode: str,
    summary: Dict[str, Any],
    runs: Dict[str, Dict[str, Any]],
    expected_library_count: int,
    compile_backend_count: int,
    expected_compile_smoke_count: int,
    experimental_compile_results: List[Dict[str, Any]],
    expected_experimental_compile_count: int,
) -> str:
    lines: List[str] = []

    title = "Host Simulation Testing Report" if mode == "standard" else "Host Simulation Memory Profile Report"
    lines.append(f"# {title}")
    lines.append("")
    lines.append("## Summary")
    lines.append("")
    lines.append(f"- Total runs: {summary['totalRuns']}")
    lines.append(f"- Passed runs: {summary['passedRuns']}")
    lines.append(f"- Failed runs: {summary['failedRuns']}")
    lines.append(f"- Max peak bytes: {summary['maxPeakBytes']}")
    lines.append(f"- Avg peak bytes: {summary['avgPeakBytes']}")
    lines.append(f"- Compile smoke objects found: {summary['compileSmokeObjectCount']}")
    lines.append(f"- Experimental compile results found: {summary['experimentalCompileResultCount']}")
    lines.append(f"- Experimental compile successes: {summary['experimentalCompileSuccessCount']}")
    lines.append(f"- Experimental compile failures: {summary['experimentalCompileFailureCount']}")
    lines.append(f"- Memory profile runs: {summary['memoryProfileRuns']}")
    lines.append(f"- Runs that exceeded limit: {summary['limitExceededRuns']}")
    lines.append(f"- Runs with limit enforcement enabled: {summary['limitEnforcedRuns']}")
    lines.append(f"- Runs with capacity probe enabled: {summary['capacityProbeEnabledRuns']}")
    lines.append(f"- Runs where capacity probe reached limit: {summary['capacityProbeLimitReachedRuns']}")
    lines.append(f"- Runs with first limit-crossing test identified: {summary['runsWithFirstLimitCrossingTest']}")

    if expected_compile_smoke_count >= 0:
        lines.append(f"- Expected compile smoke objects: {expected_compile_smoke_count}")
    elif expected_library_count > 0:
        expected_smoke = expected_library_count * max(compile_backend_count, 1)
        lines.append(f"- Expected compile smoke objects: {expected_smoke}")

    if expected_experimental_compile_count >= 0:
        lines.append(f"- Expected experimental compile results: {expected_experimental_compile_count}")

    lines.append("")
    lines.append("## Understanding")
    lines.append("")
    lines.append("- This report only summarizes artifacts downloaded into this workflow run.")
    lines.append("- PeakBytes is process-level peak memory for the full run.")
    lines.append("- Per-test RSS fields (BeforeRSS/AfterRSS) are process resident memory snapshots and may be page-granular.")
    lines.append("- Per-test heap fields (BeforeHeap/AfterHeap) track allocator-managed heap bytes and are better for small test-to-test differences.")
    lines.append("- LimitExceeded means run peak was above LimitBytes.")
    lines.append("- LimitEnforced tells whether exceeding the limit should fail the run.")
    lines.append("- Experimental compile failures are reported separately and do not gate the main host simulation lanes.")
    lines.append("- ProbeElementsAtStop and ProbeCurrentBytesAtStop come from the optional capacity probe.")
    lines.append("- FirstLimitCrossingTest is the first test whose per-test memory reached or exceeded the run limit.")
    lines.append("- If FirstLimitCrossingTest is blank, no per-test crossing was found (or no per-test stats were present).")

    lines.append("")
    lines.append("## Memory Profile Coverage")
    lines.append("")
    lines.append("| Run | MemoryProfileRun | ProbeEnabled | ProbeLimitReached | FirstLimitCrossingTest |")
    lines.append("| --- | --- | --- | --- | --- |")

    for key in sorted(runs.keys()):
        analysis = runs[key].get("analysis", {})
        first = analysis.get("firstLimitCrossingTest") or {}
        first_name = str(first.get("name", ""))
        lines.append(
            f"| {key} | {fmt_bool(analysis.get('isMemoryProfileRun', False))} | {fmt_bool(analysis.get('capacityProbeEnabled', False))} | {fmt_bool(analysis.get('capacityProbeLimitReached', False))} | {first_name} |"
        )

    lines.append("")
    lines.append("## Run Results")
    lines.append("")
    lines.append("| Run | Success | Backend | PeakBytes | LimitBytes | LimitExceeded | LimitEnforced | FirstLimitCrossingTest | CrossingPeakBytes | ProbeElementsAtStop | ProbeCurrentBytesAtStop |")
    lines.append("| --- | --- | --- | ---: | ---: | --- | --- | --- | ---: | ---: | ---: |")

    for key in sorted(runs.keys()):
        report = runs[key].get("report", {})
        stats = runs[key].get("stats", {})

        success = report.get("success", False)
        backend = report.get("backend", stats.get("backend", ""))
        memory = report.get("memory", stats.get("memory", {}))
        peak = memory.get("peakBytes", 0)
        limit = memory.get("limitBytes", 0)

        stats_memory = stats.get("memory", {})
        limit_exceeded = stats_memory.get("limitExceeded", False)
        limit_enforced = stats_memory.get("limitEnforced", True)

        probe = stats.get("capacityProbe", {})
        probe_elements = probe.get("elementsAtStop", 0)
        probe_bytes = probe.get("currentBytesAtStop", 0)

        analysis = runs[key].get("analysis", {})
        first_crossing = analysis.get("firstLimitCrossingTest") or {}
        first_crossing_name = str(first_crossing.get("name", ""))
        first_crossing_peak = first_crossing.get("peakBytesAfterTest", 0)
        if not isinstance(first_crossing_peak, int):
            first_crossing_peak = 0

        lines.append(
            f"| {key} | {fmt_bool(success)} | {backend} | {peak} | {limit} | {fmt_bool(limit_exceeded)} | {fmt_bool(limit_enforced)} | {first_crossing_name} | {first_crossing_peak} | {probe_elements} | {probe_bytes} |"
        )

    for key in sorted(runs.keys()):
        stats = runs[key].get("stats", {})
        tests = stats.get("tests", [])
        if not isinstance(tests, list) or not tests:
            continue

        lines.append("")
        lines.append(f"### Per-test Memory Stats: {key}")
        lines.append("")
        lines.append("| Test | Passed | BeforeRSS | AfterRSS | DeltaRSS | BeforeHeap | AfterHeap | DeltaHeap | PeakAfterTest | Error |")
        lines.append("| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | --- |")

        for test in tests:
            name = test.get("name", "")
            passed = fmt_bool(test.get("passed", False))
            before = test.get("beforeCurrentBytes", 0)
            after = test.get("afterCurrentBytes", 0)
            delta = test.get("deltaCurrentBytes", 0)
            before_heap = test.get("beforeHeapBytes", 0)
            after_heap = test.get("afterHeapBytes", 0)
            delta_heap = test.get("deltaHeapBytes", 0)
            peak_after = test.get("peakBytesAfterTest", 0)
            error = str(test.get("error", "")).replace("|", " ")
            lines.append(f"| {name} | {passed} | {before} | {after} | {delta} | {before_heap} | {after_heap} | {delta_heap} | {peak_after} | {error} |")

    if experimental_compile_results:
        lines.append("")
        lines.append("## Experimental Compile Results")
        lines.append("")
        lines.append("| Library | Backend | Optional | Success | ReturnCode | Artifact |")
        lines.append("| --- | --- | --- | --- | ---: | --- |")

        for result in experimental_compile_results:
            library_path = str(result.get("libraryPath", ""))
            backend = str(result.get("backend", ""))
            optional = str(result.get("optional", ""))
            success = fmt_bool(result.get("success", False))
            return_code = result.get("returnCode", -1)
            if not isinstance(return_code, int):
                return_code = -1
            artifact = str(result.get("artifactPath", ""))
            lines.append(f"| {library_path} | {backend} | {optional} | {success} | {return_code} | {artifact} |")

    lines.append("")
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate host simulation test and memory report")
    parser.add_argument("--mode", choices=["standard", "memory-profiles", "stress"], required=True)
    parser.add_argument("--artifacts-dir", required=True)
    parser.add_argument("--output-md", required=True)
    parser.add_argument("--output-json", required=True)
    parser.add_argument("--expected-library-count", type=int, default=0)
    parser.add_argument("--compile-backend-count", type=int, default=2)
    parser.add_argument("--expected-compile-smoke-count", type=int, default=-1)
    parser.add_argument("--expected-experimental-compile-count", type=int, default=-1)
    args = parser.parse_args()

    artifacts_dir = Path(args.artifacts_dir)
    output_md     = Path(args.output_md)
    output_json   = Path(args.output_json)

    if args.mode == "stress":
        stress_runs = load_stress_runs(artifacts_dir)

        boards                  = sorted({str(r.get("board", "unknown")) for r in stress_runs})
        total_instance_results  = sum(len(r.get("instanceCountProbes", [])) for r in stress_runs)
        total_fill_results      = sum(len(r.get("elementFillProbes",   [])) for r in stress_runs)

        summary = {
            "totalRuns":                  len(stress_runs),
            "boardsProfiled":             len(boards),
            "totalInstanceProbeResults":  total_instance_results,
            "totalElementFillResults":    total_fill_results,
        }

        payload  = {"mode": "stress", "summary": summary, "runs": stress_runs}
        markdown = generate_markdown_stress(stress_runs)

        output_md.parent.mkdir(parents=True, exist_ok=True)
        output_json.parent.mkdir(parents=True, exist_ok=True)
        output_md.write_text(markdown, encoding="utf-8")
        output_json.write_text(json.dumps(payload, indent=2), encoding="utf-8")

        print(f"Wrote stress markdown report: {output_md.as_posix()}")
        print(f"Wrote stress json report:     {output_json.as_posix()}")
        return 0

    # Standard and memory-profiles modes use the existing artifact-based path.
    report_files, stats_files, smoke_objects = classify_paths(artifacts_dir)
    experimental_compile_results = load_experimental_compile_results(artifacts_dir)
    runs = build_run_index(report_files, stats_files)
    analyze_runs(runs)
    summary = summarize(runs, len(smoke_objects))
    experimental_summary = summarize_experimental_compile_results(experimental_compile_results)
    summary["experimentalCompileResultCount"] = experimental_summary["total"]
    summary["experimentalCompileSuccessCount"] = experimental_summary["passed"]
    summary["experimentalCompileFailureCount"] = experimental_summary["failed"]

    payload = {
        "mode": args.mode,
        "summary": summary,
        "runs": runs,
        "reportFiles": [p.as_posix() for p in report_files],
        "statsFiles": [p.as_posix() for p in stats_files],
        "compileSmokeObjects": [p.as_posix() for p in smoke_objects],
        "experimentalCompileResults": experimental_compile_results,
        "expectedLibraryCount": args.expected_library_count,
        "compileBackendCount": args.compile_backend_count,
        "expectedCompileSmokeCount": args.expected_compile_smoke_count,
        "expectedExperimentalCompileCount": args.expected_experimental_compile_count,
    }

    markdown = generate_markdown(
        args.mode,
        summary,
        runs,
        expected_library_count=args.expected_library_count,
        compile_backend_count=args.compile_backend_count,
        expected_compile_smoke_count=args.expected_compile_smoke_count,
        experimental_compile_results=experimental_compile_results,
        expected_experimental_compile_count=args.expected_experimental_compile_count,
    )

    output_md.parent.mkdir(parents=True, exist_ok=True)
    output_json.parent.mkdir(parents=True, exist_ok=True)
    output_md.write_text(markdown, encoding="utf-8")
    output_json.write_text(json.dumps(payload, indent=2), encoding="utf-8")

    print(f"Wrote markdown report: {output_md.as_posix()}")
    print(f"Wrote json report: {output_json.as_posix()}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
