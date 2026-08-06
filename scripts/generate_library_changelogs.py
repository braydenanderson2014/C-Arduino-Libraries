#!/usr/bin/env python3
"""Generate per-library changelog markdown files from git history."""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Optional


@dataclass
class CommitEntry:
    sha: str
    date: str
    author: str
    subject: str


SECTION_ORDER = ["features", "bug_fixes", "docs", "maintenance"]
SECTION_TITLES = {
    "features": "Features",
    "bug_fixes": "Bug Fixes",
    "docs": "Documentation",
    "maintenance": "Maintenance",
}


def run_git(repo_root: Path, args: List[str]) -> str:
    completed = subprocess.run(
        ["git", *args],
        cwd=repo_root,
        check=True,
        text=True,
        capture_output=True,
    )
    return completed.stdout.strip()


def list_library_dirs(lib_root: Path) -> Iterable[Path]:
    for entry in sorted(lib_root.iterdir(), key=lambda p: p.name.lower()):
        if not entry.is_dir():
            continue
        if entry.name.startswith("__"):
            continue
        yield entry


def parse_git_log(output: str) -> List[CommitEntry]:
    entries: List[CommitEntry] = []
    if not output.strip():
        return entries

    for line in output.splitlines():
        parts = line.split("\x1f")
        if len(parts) != 4:
            continue
        sha, date, author, subject = parts
        entries.append(CommitEntry(sha=sha, date=date, author=author, subject=subject))
    return entries


def get_commit_entries(repo_root: Path, rel_path: str, limit: int) -> List[CommitEntry]:
    output = run_git(
        repo_root,
        [
            "log",
            f"--max-count={limit}",
            "--date=short",
            "--pretty=format:%H%x1f%ad%x1f%an%x1f%s",
            "--",
            rel_path,
        ],
    )
    return parse_git_log(output)


def get_commit_entries_in_range(repo_root: Path, rel_path: str, revision_range: str, limit: int) -> List[CommitEntry]:
    output = run_git(
        repo_root,
        [
            "log",
            revision_range,
            f"--max-count={limit}",
            "--date=short",
            "--pretty=format:%H%x1f%ad%x1f%an%x1f%s",
            "--",
            rel_path,
        ],
    )
    return parse_git_log(output)


def get_commit_count(repo_root: Path, rel_path: str) -> int:
    output = run_git(repo_root, ["rev-list", "--count", "HEAD", "--", rel_path])
    try:
        return int(output)
    except ValueError:
        return 0


def get_current_branch(repo_root: Path) -> str:
    return run_git(repo_root, ["rev-parse", "--abbrev-ref", "HEAD"])


def find_manifest_path(library_dir: Path) -> Optional[Path]:
    for name in ("library.json", "Library.json"):
        candidate = library_dir / name
        if candidate.exists():
            return candidate
    return None


def read_manifest_version(manifest_path: Optional[Path]) -> Optional[str]:
    if manifest_path is None:
        return None
    try:
        data = json.loads(manifest_path.read_text(encoding="utf-8"))
    except Exception:
        return None
    version = data.get("version")
    return str(version) if version else None


def get_latest_version_boundary_commit(repo_root: Path, manifest_rel_path: str) -> Optional[CommitEntry]:
    try:
        output = run_git(
            repo_root,
            [
                "log",
                "-G",
                '"version"',
                "--date=short",
                "--pretty=format:%H%x1f%ad%x1f%an%x1f%s",
                "--",
                manifest_rel_path,
            ],
        )
    except subprocess.CalledProcessError:
        return None

    entries = parse_git_log(output)
    return entries[0] if entries else None


def classify_commit(subject: str) -> str:
    lowered = subject.lower().strip()

    if lowered.startswith("feat") or " add " in f" {lowered} " or " feature" in lowered:
        return "features"
    if lowered.startswith("fix") or "bug" in lowered or "resolve" in lowered or "repair" in lowered:
        return "bug_fixes"
    if lowered.startswith("docs") or "readme" in lowered or "document" in lowered:
        return "docs"
    return "maintenance"


def cleanup_subject(subject: str) -> str:
    cleaned = re.sub(r"^(feat|fix|docs|chore|refactor|test|perf)(\([^)]*\))?:\s*", "", subject, flags=re.IGNORECASE)
    return cleaned.strip() or subject.strip()


def build_next_release_sections(entries: List[CommitEntry]) -> Dict[str, List[CommitEntry]]:
    grouped: Dict[str, List[CommitEntry]] = {key: [] for key in SECTION_ORDER}
    for entry in entries:
        grouped[classify_commit(entry.subject)].append(entry)
    return grouped


def write_library_report(
    repo_root: Path,
    output_dir: Path,
    library_dir: Path,
    rel_path: str,
    entries: List[CommitEntry],
    total_count: int,
    branch: str,
    manifest_version: Optional[str],
    version_boundary: Optional[CommitEntry],
    unreleased_entries: List[CommitEntry],
) -> None:
    output_path = output_dir / f"{library_dir.name}.md"
    lines: List[str] = []
    lines.append(f"# {library_dir.name} Changelog")
    lines.append("")
    lines.append(f"- Source path: `{rel_path}`")
    lines.append(f"- Branch analyzed: `{branch}`")
    lines.append(f"- Current manifest version: **{manifest_version or 'Unknown'}**")
    if version_boundary is not None:
        lines.append(f"- Release boundary commit: `{version_boundary.sha[:8]}` ({version_boundary.date})")
    else:
        lines.append("- Release boundary commit: _not found_")
    lines.append(f"- Total commits touching this library on current history: **{total_count}**")
    lines.append(f"- Recent commits shown: **{len(entries)}**")
    lines.append("")

    lines.append("## Next Release (Experimental / Unreleased)")
    lines.append("")
    lines.append("Changes listed here are auto-generated from commits after the latest manifest version change and are experimental until officially released.")
    lines.append("")

    if version_boundary is None:
        lines.append("_No manifest version boundary was found for this library, so unreleased changes cannot be determined automatically yet._")
        lines.append("")
    elif not unreleased_entries:
        lines.append(f"_No unreleased commits detected after version {manifest_version or 'current'}._")
        lines.append("")
    else:
        grouped = build_next_release_sections(unreleased_entries)
        for section in SECTION_ORDER:
            if not grouped[section]:
                continue
            lines.append(f"### {SECTION_TITLES[section]}")
            lines.append("")
            for entry in grouped[section]:
                lines.append(f"- {cleanup_subject(entry.subject)} (`{entry.sha[:8]}`, {entry.date})")
            lines.append("")

    if not entries:
        lines.append("_No commits found for this library path._")
        lines.append("")
    else:
        lines.append("| Date | Commit | Author | Summary |")
        lines.append("| --- | --- | --- | --- |")
        for entry in entries:
            short_sha = entry.sha[:8]
            lines.append(f"| {entry.date} | `{short_sha}` | {entry.author} | {entry.subject.replace('|', '/')} |")
        lines.append("")

    output_path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_index(output_dir: Path, rows: List[tuple[str, str, int, int, int]]) -> None:
    index_path = output_dir / "README.md"
    lines: List[str] = []
    lines.append("# Library Changelog Index")
    lines.append("")
    lines.append("Auto-generated from git history for each library folder under `lib/`.")
    lines.append("")
    lines.append("| Library | Last Change | Total Commits | Recent Entries | Unreleased Entries |")
    lines.append("| --- | --- | ---: | ---: | ---: |")
    for library_name, last_change, total_count, recent_count, unreleased_count in rows:
        lines.append(
            f"| [{library_name}]({library_name}.md) | {last_change or 'N/A'} | {total_count} | {recent_count} | {unreleased_count} |"
        )
    lines.append("")
    index_path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate per-library changelog markdown files")
    parser.add_argument("--lib-root", default="lib")
    parser.add_argument("--output-dir", default="reports/library-changelogs")
    parser.add_argument("--max-entries", type=int, default=30)
    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parents[1]
    lib_root = repo_root / args.lib_root
    output_dir = repo_root / args.output_dir
    output_dir.mkdir(parents=True, exist_ok=True)

    branch = get_current_branch(repo_root)
    index_rows: List[tuple[str, str, int, int, int]] = []

    for library_dir in list_library_dirs(lib_root):
        rel_path = library_dir.relative_to(repo_root).as_posix()
        entries = get_commit_entries(repo_root, rel_path, args.max_entries)
        total_count = get_commit_count(repo_root, rel_path)
        last_change = entries[0].date if entries else ""
        manifest_path = find_manifest_path(library_dir)
        manifest_version = read_manifest_version(manifest_path)
        version_boundary = None
        unreleased_entries: List[CommitEntry] = []
        if manifest_path is not None:
            manifest_rel_path = manifest_path.relative_to(repo_root).as_posix()
            version_boundary = get_latest_version_boundary_commit(repo_root, manifest_rel_path)
            if version_boundary is not None:
                unreleased_entries = get_commit_entries_in_range(
                    repo_root,
                    rel_path,
                    f"{version_boundary.sha}..HEAD",
                    args.max_entries,
                )
        write_library_report(
            repo_root,
            output_dir,
            library_dir,
            rel_path,
            entries,
            total_count,
            branch,
            manifest_version,
            version_boundary,
            unreleased_entries,
        )
        index_rows.append((library_dir.name, last_change, total_count, len(entries), len(unreleased_entries)))

    write_index(output_dir, index_rows)
    print(f"Generated changelog reports for {len(index_rows)} libraries in {output_dir.relative_to(repo_root).as_posix()}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
