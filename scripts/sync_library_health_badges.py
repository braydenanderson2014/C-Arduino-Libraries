#!/usr/bin/env python3
"""Sync per-library README health/testing badges from library health report.

This script reads the generated health report table and updates/inserts a
badge block in each library README under lib/*/README.md.
"""

from __future__ import annotations

import argparse
import os
import re
from pathlib import Path
from typing import Dict, Optional, Tuple

HEALTH_BADGES_START = "<!-- HEALTH_BADGES_START -->"
HEALTH_BADGES_END = "<!-- HEALTH_BADGES_END -->"

DEFAULT_HEALTH_BADGE = "https://img.shields.io/badge/Health-Unknown-9e9e9e?style=flat-square"
DEFAULT_TESTING_BADGE = "https://img.shields.io/badge/Testing-Unmanaged-9e9e9e?style=flat-square"


def normalize(value: str) -> str:
    return "".join(ch for ch in value.lower() if ch.isalnum())


def find_report_path(repo_root: Path, explicit: Optional[str]) -> Path:
    if explicit:
        path = Path(explicit)
        if not path.is_absolute():
            path = repo_root / path
        if not path.exists():
            raise FileNotFoundError(f"Report not found: {path}")
        return path

    candidates = [
        repo_root / "reports" / "library-health-report.md",
        repo_root / "library-health-report.md",
    ]
    existing = [p for p in candidates if p.exists()]
    if not existing:
        raise FileNotFoundError(
            "Could not find health report. Checked reports/library-health-report.md and library-health-report.md"
        )
    return existing[0]


def extract_badge(cell: str, fallback_url: str) -> Tuple[str, str]:
    match = re.search(r"!\[([^\]]+)\]\(([^)]+)\)", cell)
    if match:
        return match.group(1), match.group(2)

    text = re.sub(r"`", "", cell).strip() or "Unknown"
    safe_text = text.replace(" ", "%20")
    return text, f"https://img.shields.io/badge/{safe_text}-9e9e9e?style=flat-square"


def parse_health_table(report_text: str) -> Dict[str, Dict[str, str]]:
    lines = report_text.splitlines()
    header_idx = -1

    for idx, line in enumerate(lines):
        if line.strip().startswith("| Library Label |") and "| Health |" in line and "| Testing |" in line:
            header_idx = idx
            break

    status_map: Dict[str, Dict[str, str]] = {}

    if header_idx >= 0:
        for raw in lines[header_idx + 2 :]:
            line = raw.strip()
            if not line.startswith("|"):
                break

            parts = [part.strip() for part in line.strip("|").split("|")]
            if len(parts) < 4:
                continue

            library_label = parts[0]
            health_cell = parts[2]
            testing_cell = parts[3]

            health_alt, health_url = extract_badge(health_cell, DEFAULT_HEALTH_BADGE)
            testing_alt, testing_url = extract_badge(testing_cell, DEFAULT_TESTING_BADGE)

            status_map[normalize(library_label)] = {
                "label": library_label,
                "health_alt": health_alt,
                "health_url": health_url or DEFAULT_HEALTH_BADGE,
                "testing_alt": testing_alt,
                "testing_url": testing_url or DEFAULT_TESTING_BADGE,
            }

        if status_map:
            return status_map

    # Legacy fallback format: Results table with a "Routed Bucket" column.
    legacy_header_idx = -1
    for idx, line in enumerate(lines):
        if line.strip().startswith("| Library Label |") and "| Routed Bucket |" in line:
            legacy_header_idx = idx
            break

    if legacy_header_idx < 0:
        raise RuntimeError("Could not find a supported health report table format")

    bucket_badges = {
        "stable": ("Health: Stable", "https://img.shields.io/badge/Health-Stable-2ea44f?style=flat-square"),
        "potentially unhealthy": ("Health: Potentially Unhealthy", "https://img.shields.io/badge/Health-Potentially%20Unhealthy-d73a49?style=flat-square"),
        "unstable": ("Health: Unstable", "https://img.shields.io/badge/Health-Unstable-f59e0b?style=flat-square"),
        "unsure": ("Health: Unsure", "https://img.shields.io/badge/Health-Unsure-9e9e9e?style=flat-square"),
    }

    for raw in lines[legacy_header_idx + 2 :]:
        line = raw.strip()
        if not line.startswith("|"):
            break

        parts = [part.strip() for part in line.strip("|").split("|")]
        if len(parts) < 5:
            continue

        library_label = parts[0]
        routed_bucket = parts[4].strip().lower()
        health_alt, health_url = bucket_badges.get(routed_bucket, ("Health: Unknown", DEFAULT_HEALTH_BADGE))

        status_map[normalize(library_label)] = {
            "label": library_label,
            "health_alt": health_alt,
            "health_url": health_url,
            "testing_alt": "Testing: Unmanaged",
            "testing_url": DEFAULT_TESTING_BADGE,
        }

    if not status_map:
        raise RuntimeError("Legacy health report table was found, but no rows were parsed")

    return status_map


def build_badge_block(status: Dict[str, str], report_link: str) -> str:
    return "\n".join(
        [
            HEALTH_BADGES_START,
            f"[![{status['health_alt']}]({status['health_url']})]({report_link})",
            f"[![{status['testing_alt']}]({status['testing_url']})]({report_link})",
            HEALTH_BADGES_END,
        ]
    )


def insert_or_replace_block(content: str, block: str) -> str:
    pattern = re.compile(
        rf"{re.escape(HEALTH_BADGES_START)}.*?{re.escape(HEALTH_BADGES_END)}",
        re.DOTALL,
    )
    if pattern.search(content):
        return pattern.sub(block, content)

    lines = content.splitlines()
    if not lines:
        return block + "\n"

    insert_at = 1
    if lines[0].startswith("#"):
        insert_at = 1
        while insert_at < len(lines) and lines[insert_at].strip() == "":
            insert_at += 1
        if insert_at < len(lines) and lines[insert_at].strip().lower().startswith("reference version:"):
            insert_at += 1
            while insert_at < len(lines) and lines[insert_at].strip() == "":
                insert_at += 1

    new_lines = lines[:insert_at] + ["", block, ""] + lines[insert_at:]
    return "\n".join(new_lines).rstrip() + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description="Sync per-library README badges from health report")
    parser.add_argument("--report", help="Path to health report markdown (optional)")
    parser.add_argument("--lib-root", default="lib", help="Library root directory")
    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parents[1]
    report_path = find_report_path(repo_root, args.report)
    report_text = report_path.read_text(encoding="utf-8")
    status_map = parse_health_table(report_text)

    lib_root = repo_root / args.lib_root
    if not lib_root.exists():
        raise FileNotFoundError(f"Library root not found: {lib_root}")

    updated = 0
    skipped = 0

    for lib_dir in sorted(p for p in lib_root.iterdir() if p.is_dir()):
        if lib_dir.name.startswith("__"):
            continue

        readme_path = lib_dir / "README.md"
        if not readme_path.exists():
            continue

        key = normalize(lib_dir.name)
        status = status_map.get(key)
        if status is None:
            status = {
                "label": lib_dir.name,
                "health_alt": "Health: Unknown",
                "health_url": DEFAULT_HEALTH_BADGE,
                "testing_alt": "Testing: Unmanaged",
                "testing_url": DEFAULT_TESTING_BADGE,
            }
            skipped += 1

        # Compute a stable relative link from each library README.
        report_rel_text = os.path.relpath(report_path, start=readme_path.parent).replace("\\", "/")

        block = build_badge_block(status, report_rel_text)
        current = readme_path.read_text(encoding="utf-8")
        rewritten = insert_or_replace_block(current, block)

        if rewritten != current:
            readme_path.write_text(rewritten, encoding="utf-8")
            updated += 1

    print(f"Report source: {report_path.relative_to(repo_root).as_posix()}")
    print(f"Readme files updated: {updated}")
    print(f"Libraries with fallback status (not found in table): {skipped}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
