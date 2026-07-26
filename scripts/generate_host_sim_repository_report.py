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


def first_matching_json(root: Path, expected_name: str) -> Optional[Path]:
    if not root.exists() or not root.is_dir():
        return None
    for candidate in root.rglob(expected_name):
        if candidate.is_file():
            return candidate
    return None


def as_int(value: Any, default: int = 0) -> int:
    if isinstance(value, bool):
        return int(value)
    if isinstance(value, int):
        return value
    return default


def summarize_payload(payload: Dict[str, Any]) -> Dict[str, int]:
    summary = payload.get("summary", {}) if isinstance(payload, dict) else {}
    return {
        "totalRuns": as_int(summary.get("totalRuns", 0)),
        "passedRuns": as_int(summary.get("passedRuns", 0)),
        "failedRuns": as_int(summary.get("failedRuns", 0)),
        "maxPeakBytes": as_int(summary.get("maxPeakBytes", 0)),
        "avgPeakBytes": as_int(summary.get("avgPeakBytes", 0)),
        "memoryProfileRuns": as_int(summary.get("memoryProfileRuns", 0)),
        "limitExceededRuns": as_int(summary.get("limitExceededRuns", 0)),
        "capacityProbeEnabledRuns": as_int(summary.get("capacityProbeEnabledRuns", 0)),
        "capacityProbeLimitReachedRuns": as_int(summary.get("capacityProbeLimitReachedRuns", 0)),
        "runsWithFirstLimitCrossingTest": as_int(summary.get("runsWithFirstLimitCrossingTest", 0)),
    }


def build_markdown(
    standard_payload: Dict[str, Any],
    memory_payload: Dict[str, Any],
    standard_path: Optional[Path],
    memory_path: Optional[Path],
) -> str:
    std = summarize_payload(standard_payload)
    mem = summarize_payload(memory_payload)

    combined_total_runs = std["totalRuns"] + mem["totalRuns"]
    combined_passed_runs = std["passedRuns"] + mem["passedRuns"]
    combined_failed_runs = std["failedRuns"] + mem["failedRuns"]
    combined_limit_exceeded = std["limitExceededRuns"] + mem["limitExceededRuns"]
    combined_probe_enabled = std["capacityProbeEnabledRuns"] + mem["capacityProbeEnabledRuns"]
    combined_probe_reached = std["capacityProbeLimitReachedRuns"] + mem["capacityProbeLimitReachedRuns"]
    combined_first_crossing = std["runsWithFirstLimitCrossingTest"] + mem["runsWithFirstLimitCrossingTest"]

    lines = []
    lines.append("# Host Simulation Repository Report")
    lines.append("")
    lines.append("## Summary")
    lines.append("")
    lines.append(f"- Combined total runs: {combined_total_runs}")
    lines.append(f"- Combined passed runs: {combined_passed_runs}")
    lines.append(f"- Combined failed runs: {combined_failed_runs}")
    lines.append(f"- Combined runs that exceeded limit: {combined_limit_exceeded}")
    lines.append(f"- Combined runs with capacity probe enabled: {combined_probe_enabled}")
    lines.append(f"- Combined runs where capacity probe reached limit: {combined_probe_reached}")
    lines.append(f"- Combined runs with first limit-crossing test identified: {combined_first_crossing}")
    lines.append("")
    lines.append("## Understanding")
    lines.append("")
    lines.append("- This report merges the latest available host test report and memory profile report artifacts.")
    lines.append("- If one source is missing, this report is partial and will say which source was not found.")
    lines.append("- Use the Standard section to read compile/integration run behavior.")
    lines.append("- Use the Memory Profiles section to read board-profile limit and probe behavior.")
    lines.append("- First limit-crossing test means the earliest test whose memory crossed the configured run limit.")
    lines.append("")
    lines.append("## Source Status")
    lines.append("")
    lines.append(f"- Standard report source: {standard_path.as_posix() if standard_path else 'missing'}")
    lines.append(f"- Memory profile report source: {memory_path.as_posix() if memory_path else 'missing'}")
    lines.append("")
    lines.append("## Standard Host Report")
    lines.append("")
    lines.append(f"- Total runs: {std['totalRuns']}")
    lines.append(f"- Passed runs: {std['passedRuns']}")
    lines.append(f"- Failed runs: {std['failedRuns']}")
    lines.append(f"- Max peak bytes: {std['maxPeakBytes']}")
    lines.append(f"- Avg peak bytes: {std['avgPeakBytes']}")
    lines.append(f"- Runs that exceeded limit: {std['limitExceededRuns']}")
    lines.append(f"- Runs with first limit-crossing test identified: {std['runsWithFirstLimitCrossingTest']}")
    lines.append("")
    lines.append("## Memory Profiles Report")
    lines.append("")
    lines.append(f"- Total runs: {mem['totalRuns']}")
    lines.append(f"- Passed runs: {mem['passedRuns']}")
    lines.append(f"- Failed runs: {mem['failedRuns']}")
    lines.append(f"- Max peak bytes: {mem['maxPeakBytes']}")
    lines.append(f"- Avg peak bytes: {mem['avgPeakBytes']}")
    lines.append(f"- Memory profile runs: {mem['memoryProfileRuns']}")
    lines.append(f"- Runs that exceeded limit: {mem['limitExceededRuns']}")
    lines.append(f"- Runs with capacity probe enabled: {mem['capacityProbeEnabledRuns']}")
    lines.append(f"- Runs where capacity probe reached limit: {mem['capacityProbeLimitReachedRuns']}")
    lines.append(f"- Runs with first limit-crossing test identified: {mem['runsWithFirstLimitCrossingTest']}")
    lines.append("")

    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate repository-level host simulation report")
    parser.add_argument("--standard-dir", required=True)
    parser.add_argument("--memory-dir", required=True)
    parser.add_argument("--output-md", required=True)
    parser.add_argument("--output-json", required=True)
    args = parser.parse_args()

    standard_dir = Path(args.standard_dir)
    memory_dir = Path(args.memory_dir)
    output_md = Path(args.output_md)
    output_json = Path(args.output_json)

    standard_path = first_matching_json(standard_dir, "host-sim-test-report.json")
    memory_path = first_matching_json(memory_dir, "host-sim-memory-profile-report.json")

    standard_payload = load_json(standard_path) if standard_path else {}
    memory_payload = load_json(memory_path) if memory_path else {}

    markdown = build_markdown(standard_payload, memory_payload, standard_path, memory_path)

    payload = {
        "standardReportPath": standard_path.as_posix() if standard_path else None,
        "memoryReportPath": memory_path.as_posix() if memory_path else None,
        "standardReport": standard_payload,
        "memoryProfileReport": memory_payload,
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
