#!/usr/bin/env python3
import argparse
import json
from pathlib import Path
from statistics import mean
from typing import Any, Dict, List, Tuple


def load_json(path: Path) -> Dict[str, Any]:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except Exception:
        return {}


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


def summarize(runs: Dict[str, Dict[str, Any]], smoke_count: int) -> Dict[str, Any]:
    total = len(runs)
    passed = 0
    failed = 0
    peaks: List[int] = []

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

    return {
        "totalRuns": total,
        "passedRuns": passed,
        "failedRuns": failed,
        "maxPeakBytes": max(peaks) if peaks else 0,
        "avgPeakBytes": int(mean(peaks)) if peaks else 0,
        "compileSmokeObjectCount": smoke_count,
    }


def fmt_bool(value: Any) -> str:
    return "yes" if bool(value) else "no"


def generate_markdown(
    mode: str,
    summary: Dict[str, Any],
    runs: Dict[str, Dict[str, Any]],
    expected_library_count: int,
    compile_backend_count: int,
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

    if expected_library_count > 0:
        expected_smoke = expected_library_count * max(compile_backend_count, 1)
        lines.append(f"- Expected compile smoke objects: {expected_smoke}")

    lines.append("")
    lines.append("## Run Results")
    lines.append("")
    lines.append("| Run | Success | Backend | PeakBytes | LimitBytes | LimitExceeded | LimitEnforced | ProbeElementsAtStop | ProbeCurrentBytesAtStop |")
    lines.append("| --- | --- | --- | ---: | ---: | --- | --- | ---: | ---: |")

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

        lines.append(
            f"| {key} | {fmt_bool(success)} | {backend} | {peak} | {limit} | {fmt_bool(limit_exceeded)} | {fmt_bool(limit_enforced)} | {probe_elements} | {probe_bytes} |"
        )

    for key in sorted(runs.keys()):
        stats = runs[key].get("stats", {})
        tests = stats.get("tests", [])
        if not isinstance(tests, list) or not tests:
            continue

        lines.append("")
        lines.append(f"### Per-test Memory Stats: {key}")
        lines.append("")
        lines.append("| Test | Passed | BeforeBytes | AfterBytes | DeltaBytes | PeakAfterTest | Error |")
        lines.append("| --- | --- | ---: | ---: | ---: | ---: | --- |")

        for test in tests:
            name = test.get("name", "")
            passed = fmt_bool(test.get("passed", False))
            before = test.get("beforeCurrentBytes", 0)
            after = test.get("afterCurrentBytes", 0)
            delta = test.get("deltaCurrentBytes", 0)
            peak_after = test.get("peakBytesAfterTest", 0)
            error = str(test.get("error", "")).replace("|", " ")
            lines.append(f"| {name} | {passed} | {before} | {after} | {delta} | {peak_after} | {error} |")

    lines.append("")
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate host simulation test and memory report")
    parser.add_argument("--mode", choices=["standard", "memory-profiles"], required=True)
    parser.add_argument("--artifacts-dir", required=True)
    parser.add_argument("--output-md", required=True)
    parser.add_argument("--output-json", required=True)
    parser.add_argument("--expected-library-count", type=int, default=0)
    parser.add_argument("--compile-backend-count", type=int, default=2)
    args = parser.parse_args()

    artifacts_dir = Path(args.artifacts_dir)
    output_md = Path(args.output_md)
    output_json = Path(args.output_json)

    report_files, stats_files, smoke_objects = classify_paths(artifacts_dir)
    runs = build_run_index(report_files, stats_files)
    summary = summarize(runs, len(smoke_objects))

    payload = {
        "mode": args.mode,
        "summary": summary,
        "runs": runs,
        "reportFiles": [p.as_posix() for p in report_files],
        "statsFiles": [p.as_posix() for p in stats_files],
        "compileSmokeObjects": [p.as_posix() for p in smoke_objects],
        "expectedLibraryCount": args.expected_library_count,
        "compileBackendCount": args.compile_backend_count,
    }

    markdown = generate_markdown(
        args.mode,
        summary,
        runs,
        expected_library_count=args.expected_library_count,
        compile_backend_count=args.compile_backend_count,
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
