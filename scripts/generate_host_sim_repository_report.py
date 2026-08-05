#!/usr/bin/env python3
import argparse
import json
from pathlib import Path
from typing import Any, Dict, Optional


def load_json(path: Path) -> Dict[str, Any]:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except Exception:
        return {}


def first_matching_file(root: Path, expected_name: str) -> Optional[Path]:
    if not root.exists() or not root.is_dir():
        return None
    for candidate in root.rglob(expected_name):
        if candidate.is_file():
            return candidate
    return None


def load_text(path: Optional[Path]) -> str:
    if not path:
        return ""
    try:
        return path.read_text(encoding="utf-8")
    except Exception:
        return ""


def as_int(value: Any, default: int = 0) -> int:
    if isinstance(value, bool):
        return int(value)
    if isinstance(value, int):
        return value
    return default


def summarize_standard_payload(payload: Dict[str, Any]) -> Dict[str, int]:
    summary = payload.get("summary", {}) if isinstance(payload, dict) else {}
    return {
        "totalRuns":                      as_int(summary.get("totalRuns", 0)),
        "passedRuns":                     as_int(summary.get("passedRuns", 0)),
        "failedRuns":                     as_int(summary.get("failedRuns", 0)),
        "maxPeakBytes":                   as_int(summary.get("maxPeakBytes", 0)),
        "avgPeakBytes":                   as_int(summary.get("avgPeakBytes", 0)),
        "limitExceededRuns":              as_int(summary.get("limitExceededRuns", 0)),
        "runsWithFirstLimitCrossingTest": as_int(summary.get("runsWithFirstLimitCrossingTest", 0)),
        "experimentalCompileResultCount": as_int(summary.get("experimentalCompileResultCount", 0)),
        "experimentalCompileSuccessCount": as_int(summary.get("experimentalCompileSuccessCount", 0)),
        "experimentalCompileFailureCount": as_int(summary.get("experimentalCompileFailureCount", 0)),
        "experimentalCompileLibraryCount": as_int(summary.get("experimentalCompileLibraryCount", 0)),
        "experimentalCompileBackendCount": as_int(summary.get("experimentalCompileBackendCount", 0)),
        "experimentalCompileOptionalModeCount": as_int(summary.get("experimentalCompileOptionalModeCount", 0)),
    }


def summarize_stress_payload(payload: Dict[str, Any]) -> Dict[str, int]:
    summary = payload.get("summary", {}) if isinstance(payload, dict) else {}
    return {
        "totalRuns":                 as_int(summary.get("totalRuns", 0)),
        "boardsProfiled":            as_int(summary.get("boardsProfiled", 0)),
        "totalInstanceProbeResults": as_int(summary.get("totalInstanceProbeResults", 0)),
        "totalElementFillResults":   as_int(summary.get("totalElementFillResults", 0)),
    }


def strip_leading_title(markdown: str) -> str:
    if not markdown:
        return ""

    lines = markdown.splitlines()
    if lines and lines[0].startswith("# "):
        lines = lines[1:]
        while lines and not lines[0].strip():
            lines = lines[1:]

    return "\n".join(lines).strip()


def build_markdown(
    standard_payload: Dict[str, Any],
    stress_payload: Dict[str, Any],
    standard_path: Optional[Path],
    stress_path: Optional[Path],
    standard_markdown: str,
    stress_markdown: str,
) -> str:
    std    = summarize_standard_payload(standard_payload)
    stress = summarize_stress_payload(stress_payload)

    combined_total_runs   = std["totalRuns"]   + stress["totalRuns"]
    combined_passed_runs  = std["passedRuns"]
    combined_failed_runs  = std["failedRuns"]

    lines = []
    lines.append("# Host Simulation Repository Report")
    lines.append("")
    lines.append("## Summary")
    lines.append("")
    lines.append(f"- Combined total runs: {combined_total_runs}")
    lines.append(f"- Test runs passed: {combined_passed_runs}")
    lines.append(f"- Test runs failed: {combined_failed_runs}")
    lines.append(f"- Boards profiled by stress tests: {stress['boardsProfiled']}")
    lines.append(f"- Total instance probe results: {stress['totalInstanceProbeResults']}")
    lines.append(f"- Total element fill results: {stress['totalElementFillResults']}")
    lines.append(f"- Experimental compile results: {std['experimentalCompileResultCount']}")
    lines.append(f"- Experimental compile libraries covered: {std['experimentalCompileLibraryCount']}")
    lines.append(f"- Experimental compile failures: {std['experimentalCompileFailureCount']}")
    lines.append("")
    lines.append("## Understanding")
    lines.append("")
    lines.append("- This report merges the latest available host test report and stress report artifacts.")
    lines.append("- If one source is missing, this report is partial and will say which source was not found.")
    lines.append("- Use the Standard section to read compile/integration run correctness results.")
    lines.append("- Use the Stress Test section to read per-board capacity metrics.")
    lines.append("- RSS (resident set size) means physical memory pages currently resident for the process.")
    lines.append("- Peak bytes is the full-run RSS high-water mark; average peak bytes is the average of run peak RSS values.")
    lines.append("- Experimental compile metrics are matrix-entry counts over experimental library/backend/optional combinations.")
    lines.append("")
    lines.append("## Source Status")
    lines.append("")
    lines.append(f"- Standard report source: {standard_path.as_posix() if standard_path else 'missing'}")
    lines.append(f"- Stress report source: {stress_path.as_posix() if stress_path else 'missing'}")
    lines.append("")
    lines.append("## Standard Host Report")
    lines.append("")
    if standard_markdown:
        lines.append(strip_leading_title(standard_markdown))
        lines.append("")
    else:
        lines.append(f"- Total runs: {std['totalRuns']}")
        lines.append(f"- Passed runs: {std['passedRuns']}")
        lines.append(f"- Failed runs: {std['failedRuns']}")
        lines.append(f"- Max peak bytes: {std['maxPeakBytes']}")
        lines.append(f"- Avg peak bytes: {std['avgPeakBytes']}")
        lines.append(f"- Runs that exceeded informational limit: {std['limitExceededRuns']}")
        lines.append(f"- Experimental compile results: {std['experimentalCompileResultCount']}")
        lines.append(f"- Experimental compile successes: {std['experimentalCompileSuccessCount']}")
        lines.append(f"- Experimental compile failures: {std['experimentalCompileFailureCount']}")
        lines.append(f"- Experimental libraries covered: {std['experimentalCompileLibraryCount']}")
        lines.append(f"- Experimental backends covered: {std['experimentalCompileBackendCount']}")
        lines.append(f"- Experimental optional modes covered: {std['experimentalCompileOptionalModeCount']}")
        lines.append("")

    lines.append("## Stress Test Report")
    lines.append("")
    if stress_markdown:
        lines.append(strip_leading_title(stress_markdown))
        lines.append("")
    else:
        lines.append(f"- Total stress runs: {stress['totalRuns']}")
        lines.append(f"- Boards profiled: {stress['boardsProfiled']}")
        lines.append(f"- Instance probe results: {stress['totalInstanceProbeResults']}")
        lines.append(f"- Element fill results: {stress['totalElementFillResults']}")
        lines.append("")

    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate repository-level host simulation report")
    parser.add_argument("--standard-dir", required=True)
    parser.add_argument("--stress-dir",   required=True)
    parser.add_argument("--output-md",    required=True)
    parser.add_argument("--output-json",  required=True)
    args = parser.parse_args()

    standard_dir = Path(args.standard_dir)
    stress_dir   = Path(args.stress_dir)
    output_md    = Path(args.output_md)
    output_json  = Path(args.output_json)

    standard_path = first_matching_file(standard_dir, "host-sim-test-report.json")
    stress_path   = first_matching_file(stress_dir,   "host-sim-stress-report.json")
    standard_md_path = first_matching_file(standard_dir, "host-sim-test-report.md")
    stress_md_path   = first_matching_file(stress_dir,   "host-sim-stress-report.md")

    standard_payload = load_json(standard_path) if standard_path else {}
    stress_payload   = load_json(stress_path)   if stress_path   else {}
    standard_markdown = load_text(standard_md_path)
    stress_markdown   = load_text(stress_md_path)

    markdown = build_markdown(
        standard_payload,
        stress_payload,
        standard_path,
        stress_path,
        standard_markdown,
        stress_markdown,
    )

    payload = {
        "standardReportPath": standard_path.as_posix() if standard_path else None,
        "stressReportPath":   stress_path.as_posix()   if stress_path   else None,
        "standardMarkdownPath": standard_md_path.as_posix() if standard_md_path else None,
        "stressMarkdownPath":   stress_md_path.as_posix()   if stress_md_path   else None,
        "standardReport":     standard_payload,
        "stressReport":       stress_payload,
    }

    output_md.parent.mkdir(parents=True, exist_ok=True)
    output_json.parent.mkdir(parents=True, exist_ok=True)
    output_md.write_text(markdown, encoding="utf-8")
    output_json.write_text(json.dumps(payload, indent=2), encoding="utf-8")

    print(f"Wrote markdown report: {output_md.as_posix()}")
    print(f"Wrote json report: {output_json.as_posix()}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
