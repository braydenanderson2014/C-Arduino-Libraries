#!/usr/bin/env python3
"""
AI Duplicate Detector for GitHub Issues.

Uses TF-IDF vectorization and cosine similarity to detect potential
duplicate issues, outputting results in JSON format for consumption
by the GitHub Actions workflow.
"""

import argparse
import json
import math
import re
import sys
from collections import Counter

try:
    import requests
except ImportError:
    print("ERROR: 'requests' package is required. Install with: pip install requests", file=sys.stderr)
    sys.exit(1)


def fetch_issues(repo_owner, repo_name, token, state="open"):
    """Fetch issues from GitHub API."""
    headers = {
        "Authorization": f"token {token}",
        "Accept": "application/vnd.github.v3+json",
    }
    issues = []
    page = 1
    while True:
        url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/issues"
        params = {"state": state, "per_page": 100, "page": page}
        response = requests.get(url, headers=headers, params=params, timeout=30)
        response.raise_for_status()
        data = response.json()
        if not data:
            break
        # Exclude pull requests
        issues.extend([i for i in data if "pull_request" not in i])
        if len(data) < 100:
            break
        page += 1
    return issues


def tokenize(text):
    """Simple tokenizer: lowercase and split on non-alphanumeric characters."""
    if not text:
        return []
    return re.findall(r"[a-z0-9]+", text.lower())


def compute_tfidf(documents):
    """Compute TF-IDF vectors for a list of tokenized documents."""
    n = len(documents)
    if n == 0:
        return [], {}

    # Document frequency
    df = Counter()
    for tokens in documents:
        df.update(set(tokens))

    # IDF with +1 smoothing on both numerator and denominator to avoid
    # division-by-zero and to reduce the weight of very common terms.
    idf = {term: math.log((n + 1) / (freq + 1)) + 1 for term, freq in df.items()}

    # TF-IDF vectors
    vectors = []
    for tokens in documents:
        tf = Counter(tokens)
        total = len(tokens) or 1
        vec = {term: (count / total) * idf[term] for term, count in tf.items()}
        vectors.append(vec)

    return vectors, idf


def cosine_similarity(vec_a, vec_b):
    """Compute cosine similarity between two sparse TF-IDF vectors."""
    if not vec_a or not vec_b:
        return 0.0
    common = set(vec_a) & set(vec_b)
    dot = sum(vec_a[t] * vec_b[t] for t in common)
    norm_a = math.sqrt(sum(v * v for v in vec_a.values()))
    norm_b = math.sqrt(sum(v * v for v in vec_b.values()))
    if norm_a == 0 or norm_b == 0:
        return 0.0
    return dot / (norm_a * norm_b)


def detect_duplicates(issues, min_confidence, max_candidates):
    """
    Detect duplicate issue pairs using TF-IDF cosine similarity.

    Returns a list of dicts with keys: issue_a, issue_b, similarity_score, ai_confidence.
    """
    texts = []
    for issue in issues:
        title = issue.get("title", "") or ""
        body = issue.get("body", "") or ""
        texts.append(tokenize(title + " " + body))

    vectors, _ = compute_tfidf(texts)

    duplicates = []
    processed_pairs = set()
    n = len(issues)
    for i in range(n):
        candidates = []
        for j in range(n):
            if i == j:
                continue
            sim = cosine_similarity(vectors[i], vectors[j])
            if sim >= min_confidence:
                candidates.append((j, sim))

        candidates.sort(key=lambda x: x[1], reverse=True)
        for j, sim in candidates[:max_candidates]:
            # Avoid adding both (i,j) and (j,i)
            pair_key = (min(issues[i]["number"], issues[j]["number"]),
                        max(issues[i]["number"], issues[j]["number"]))
            if pair_key not in processed_pairs:
                processed_pairs.add(pair_key)
                duplicates.append({
                    "issue_a": pair_key[0],
                    "issue_b": pair_key[1],
                    # similarity_score is the raw cosine similarity;
                    # ai_confidence mirrors it here but could incorporate
                    # additional signals (e.g. label overlap) in future.
                    "similarity_score": round(sim, 4),
                    "ai_confidence": round(sim, 4),
                })

    return duplicates


def main():
    parser = argparse.ArgumentParser(description="AI Duplicate Issue Detector")
    parser.add_argument("--repo-owner", required=True, help="GitHub repository owner")
    parser.add_argument("--repo-name", required=True, help="GitHub repository name")
    parser.add_argument("--token", required=True, help="GitHub personal access token")
    parser.add_argument("--min-confidence", type=float, default=0.8,
                        help="Minimum similarity score to report (0.0-1.0)")
    parser.add_argument("--max-candidates", type=int, default=10,
                        help="Maximum duplicate candidates to report per issue")
    parser.add_argument("--output", default="ai_duplicates.json",
                        help="Output JSON file path")
    args = parser.parse_args()

    print(f"Fetching issues from {args.repo_owner}/{args.repo_name}...")
    try:
        issues = fetch_issues(args.repo_owner, args.repo_name, args.token)
    except requests.RequestException as exc:
        print(f"ERROR: Failed to fetch issues: {exc}", file=sys.stderr)
        sys.exit(1)

    print(f"Fetched {len(issues)} open issues. Running duplicate detection...")
    duplicates = detect_duplicates(issues, args.min_confidence, args.max_candidates)
    print(f"Found {len(duplicates)} potential duplicate pairs.")

    result = {"duplicates": duplicates}
    with open(args.output, "w", encoding="utf-8") as f:
        json.dump(result, f, indent=2)
    print(f"Results written to {args.output}")


if __name__ == "__main__":
    main()
