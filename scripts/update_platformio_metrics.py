#!/usr/bin/env python3
"""Generate a markdown report with PlatformIO registry metrics."""

from __future__ import annotations

import argparse
from difflib import get_close_matches
import json
import sys
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List
from urllib.error import HTTPError, URLError
from urllib.parse import quote, urlencode
from urllib.request import Request, urlopen


API_BASE = "https://api.registry.platformio.org/v3"
REGISTRY_BADGE_BASE = "https://badges.registry.platformio.org/packages"
REGISTRY_PAGE_BASE = "https://registry.platformio.org/libraries"

README_BADGES_START = "<!-- PLATFORMIO_BADGES_START -->"
README_BADGES_END = "<!-- PLATFORMIO_BADGES_END -->"


def fetch_json(url: str) -> Dict[str, Any]:
    request = Request(url, headers={"User-Agent": "C-Arduino-Libraries/metrics-bot"})
    try:
        with urlopen(request, timeout=30) as response:
            payload = response.read().decode("utf-8")
            return json.loads(payload)
    except HTTPError as exc:
        raise RuntimeError(f"HTTP {exc.code} for {url}") from exc
    except URLError as exc:
        raise RuntimeError(f"Network error for {url}: {exc}") from exc


def fetch_owner_packages(owner: str) -> List[Dict[str, Any]]:
    items: List[Dict[str, Any]] = []
    page = 1
    limit = 50

    while True:
        query = urlencode({"query": f"owner:{owner}", "limit": limit, "page": page})
        url = f"{API_BASE}/search?{query}"
        data = fetch_json(url)

        page_items = data.get("items") or []
        if not page_items:
            break

        items.extend(page_items)
        total = int(data.get("total") or 0)

        if len(items) >= total or len(page_items) < limit:
            break
        page += 1

    return [pkg for pkg in items if pkg.get("type") == "library"]


def fetch_search_total(query_text: str) -> int:
    query = urlencode({"query": query_text, "limit": 1, "page": 1})
    url = f"{API_BASE}/search?{query}"
    data = fetch_json(url)
    return int(data.get("total") or 0)


def fetch_package_details(owner: str, name: str) -> Dict[str, Any]:
    owner_encoded = quote(owner, safe="")
    name_encoded = quote(name, safe="")
    url = f"{API_BASE}/packages/{owner_encoded}/library/{name_encoded}"
    return fetch_json(url)


def format_num(value: Any) -> str:
    if value is None:
        return "N/A"
    return str(value)


def format_trend(value: Any) -> str:
    if value is None:
        return "N/A"
    try:
        number = int(value)
    except (TypeError, ValueError):
        return str(value)
    if number > 0:
        return f"+{number}"
    return str(number)


def registry_urls(owner: str, package_name: str) -> Dict[str, str]:
    owner_encoded = quote(owner, safe="")
    package_encoded = quote(package_name, safe="")
    badge = f"{REGISTRY_BADGE_BASE}/{owner_encoded}/library/{package_encoded}.svg"
    page = f"{REGISTRY_PAGE_BASE}/{owner_encoded}/{package_encoded}"
    return {"badge": badge, "page": page}


def normalize_library_key(name: str) -> str:
    return "".join(char for char in name.lower() if char.isalnum())


def discover_library_folders(repo_root: Path) -> Dict[str, str]:
    lib_root = repo_root / "lib"
    if not lib_root.exists() or not lib_root.is_dir():
        return {}

    lookup: Dict[str, str] = {}
    for entry in lib_root.iterdir():
        if not entry.is_dir():
            continue
        if entry.name.startswith("__"):
            continue

        key = normalize_library_key(entry.name)
        if key and key not in lookup:
            lookup[key] = entry.name

    return lookup


def resolve_local_library_path(library_name: str, folder_lookup: Dict[str, str]) -> str:
    if not folder_lookup:
        return ""

    key = normalize_library_key(library_name)
    if not key:
        return ""

    if key in folder_lookup:
        return f"lib/{folder_lookup[key]}"

    contain_matches = [folder_key for folder_key in folder_lookup if key in folder_key or folder_key in key]
    if contain_matches:
        best = sorted(contain_matches, key=lambda folder_key: abs(len(folder_key) - len(key)))[0]
        return f"lib/{folder_lookup[best]}"

    close = get_close_matches(key, list(folder_lookup.keys()), n=1, cutoff=0.82)
    if close:
        return f"lib/{folder_lookup[close[0]]}"

    return ""


def categorize_library(name: str) -> str:
    lowered = name.lower()

    if any(token in lowered for token in ("tree", "map", "list", "queue", "stack", "vector", "dict", "hash", "array")):
        return "Data Structures"

    if any(token in lowered for token in ("optional", "variant", "typetraits", "traits", "predicate", "operator", "iterator", "limits", "initializer")):
        return "Language and Utility Helpers"

    if any(token in lowered for token in ("math", "matrix", "timer", "color")):
        return "Math and Runtime Helpers"

    if any(token in lowered for token in ("sd", "io", "stream", "storage", "fs", "file", "property", "properties")):
        return "Storage and IO"

    return "Other Libraries"


def build_markdown(owner: str, packages: List[Dict[str, Any]], owner_total: int, global_library_total: int) -> str:
    generated_at = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")
    owner_library_total = len(packages)
    if global_library_total > 0:
        share = (owner_library_total / global_library_total) * 100.0
        share_text = f"{share:.2f}%"
    else:
        share_text = "N/A"

    lines = [
        "# PlatformIO Metrics Report",
        "",
        f"Source owner query: `{owner}`",
        f"Generated: `{generated_at}`",
        "",
        "This file is auto-generated by `.github/workflows/update-platformio-metrics.yml`.",
        "",
        "## Registry Comparison",
        "",
        f"- Global libraries in PlatformIO Registry: **{global_library_total}**",
        f"- Owner packages matched by query (`owner:{owner}`): **{owner_total}**",
        f"- Owner libraries in this report: **{owner_library_total}**",
        f"- Owner library share of global libraries: **{share_text}**",
        "",
        "## Notes",
        "",
        "- PlatformIO public API currently exposes rank/trend style metrics reliably.",
        "- The API does not provide a simple public `monthly_downloads` and `total_downloads` pair per package in this report.",
        "- `Download Trend` is included from `downloads_trend` where available.",
        "- Registry badge links below are generated from owner/package names.",
        "",
        "## Libraries",
        "",
        "| Library | Version | Registry Badge | Registry URL | Popularity Rank | Rank Trend | Download Trend | Dependents | Stars | Last Updated |",
        "|---|---|---|---|---:|---:|---:|---:|---:|---|",
    ]

    for pkg in packages:
        name = str(pkg.get("name") or "Unknown")
        version = str((pkg.get("version") or {}).get("name") or "N/A")
        rank = format_num(pkg.get("popularity_rank"))
        rank_trend = format_trend(pkg.get("popularity_trend"))
        download_trend = format_trend(pkg.get("downloads_trend"))
        dependents = format_num(pkg.get("dependents_count"))
        stars = format_num(pkg.get("stars_count"))
        updated = str(pkg.get("updated_at") or "N/A")

        urls = registry_urls(owner, name)
        badge_md = f"[![{name}](" + urls["badge"] + ")](" + urls["page"] + ")"
        link_md = f"[{name}]({urls['page']})"

        lines.append(
            f"| {name} | {version} | {badge_md} | {link_md} | {rank} | {rank_trend} | {download_trend} | {dependents} | {stars} | {updated} |"
        )

    lines.extend(
        [
            "",
            "## Raw API Endpoints Used",
            "",
            f"- Global libraries total: `{API_BASE}/search?query=type:library&limit=1&page=1`",
            f"- Search: `{API_BASE}/search?query=owner:{owner}&limit=50&page=1`",
            f"- Package details: `{API_BASE}/packages/{owner}/library/<name>`",
            "",
        ]
    )

    return "\n".join(lines)


def build_readme_badges_block(owner: str, packages: List[Dict[str, Any]], repo_root: Path) -> str:
    generated_at = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")
    folder_lookup = discover_library_folders(repo_root)
    lines = [
        README_BADGES_START,
        "## PlatformIO Registry Badges",
        "",
        f"Auto-generated from PlatformIO registry package data ({generated_at}).",
        "",
        "Quick expectation notes: data structures, utility helpers (`Optional`, `Variant`, `TypeTraits`), and storage-facing support (`SD`/`SDList` and repo-level `LittleFS` integration paths).",
        "",
    ]

    category_order = [
        "Data Structures",
        "Language and Utility Helpers",
        "Storage and IO",
        "Math and Runtime Helpers",
        "Other Libraries",
    ]
    grouped: Dict[str, List[str]] = {category: [] for category in category_order}

    sorted_packages = sorted(packages, key=lambda pkg: str(pkg.get("name") or ""))
    for pkg in sorted_packages:
        name = str(pkg.get("name") or "Unknown")
        urls = registry_urls(owner, name)
        category = categorize_library(name)
        local_path = resolve_local_library_path(name, folder_lookup)
        if local_path:
            local_link = f"[{name}]({local_path})"
        else:
            local_link = f"`{name}`"
        grouped.setdefault(category, []).append(f"- {local_link} [![{name}]({urls['badge']})]({urls['page']})")

    for category in category_order:
        entries = grouped.get(category) or []
        if not entries:
            continue
        lines.append(f"### {category}")
        lines.append("")
        lines.extend(entries)
        lines.append("")

    lines.append(README_BADGES_END)
    return "\n".join(lines)


def update_readme_badges(readme_path: Path, badges_block: str) -> bool:
    text = readme_path.read_text(encoding="utf-8")
    start = text.find(README_BADGES_START)
    end = text.find(README_BADGES_END)

    if start != -1 and end != -1 and end > start:
        end += len(README_BADGES_END)
        updated = text[:start] + badges_block + text[end:]
    else:
        updated = text.rstrip() + "\n\n" + badges_block + "\n"

    if updated == text:
        return False

    readme_path.write_text(updated, encoding="utf-8")
    return True


def parse_existing_metrics_rows(metrics_path: Path) -> List[Dict[str, Any]]:
    if not metrics_path.exists():
        return []

    by_name: Dict[str, Dict[str, Any]] = {}
    for line in metrics_path.read_text(encoding="utf-8").splitlines():
        if not line.startswith("|"):
            continue
        parts = [p.strip() for p in line.strip().split("|")]
        if len(parts) < 4:
            continue
        library = parts[1]
        version = parts[2]
        if library in {"Library", "---"}:
            continue
        by_name[library] = {"name": library, "version": {"name": version}}
    return list(by_name.values())


def enrich_packages(owner: str, packages: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    enriched: List[Dict[str, Any]] = []

    for item in packages:
        name = item.get("name")
        if not name:
            continue

        merged = dict(item)
        try:
            details = fetch_package_details(owner, str(name))
            for key in ("downloads_trend", "dependents_count", "updated_at", "stars_count", "popularity_rank", "popularity_trend"):
                if key in details:
                    merged[key] = details.get(key)
        except RuntimeError as exc:
            print(f"Warning: failed to fetch details for {name}: {exc}", file=sys.stderr)

        enriched.append(merged)

    enriched.sort(key=lambda pkg: (pkg.get("popularity_rank") is None, pkg.get("popularity_rank") or 10**9, str(pkg.get("name") or "")))
    return enriched


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate PlatformIO metrics markdown report")
    parser.add_argument("--owner", default="braydenanderson2014", help="PlatformIO owner username")
    parser.add_argument(
        "--output",
        default="PLATFORMIO_METRICS.md",
        help="Output markdown file path",
    )
    parser.add_argument(
        "--readme",
        default="",
        help="Optional README.md path to refresh PlatformIO badge block",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    output_path = Path(args.output)

    enriched: List[Dict[str, Any]]
    try:
        owner_total = fetch_search_total(f"owner:{args.owner}")
        global_library_total = fetch_search_total("type:library")
        packages = fetch_owner_packages(args.owner)
        enriched = enrich_packages(args.owner, packages)
        markdown = build_markdown(args.owner, enriched, owner_total, global_library_total)
        output_path.write_text(markdown + "\n", encoding="utf-8")
        print(f"Wrote {len(enriched)} libraries to {output_path}")
    except RuntimeError as exc:
        print(f"Warning: live PlatformIO fetch failed: {exc}", file=sys.stderr)
        enriched = parse_existing_metrics_rows(output_path)
        if not enriched:
            raise
        print(f"Using {len(enriched)} libraries from existing {output_path}")

    if args.readme:
        readme_path = Path(args.readme)
        badges_block = build_readme_badges_block(args.owner, enriched, readme_path.parent)
        changed = update_readme_badges(readme_path, badges_block)
        if changed:
            print(f"Updated README badge block in {readme_path}")
        else:
            print(f"README badge block already up to date in {readme_path}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
