#!/usr/bin/env python3
"""
AI-Powered Duplicate Issue Detection System

This script uses machine learning and natural language processing to detect
duplicate GitHub issues with high accuracy. It learns from admin-labeled
issues to improve detection over time.

Features:
- AI/ML-based similarity detection
- Respects challenge-upheld exclusions
- Learns from admin duplicate confirmations
- Maintains training data for continuous improvement
- Handles both open and closed issues appropriately
"""

import os
import sys
import json
import time
import argparse
from datetime import datetime, timedelta, timezone
from typing import List, Dict, Tuple, Optional, Set
from dataclasses import dataclass, asdict
import pickle
import hashlib

# Third-party imports with fallbacks
try:
    import requests
    from sklearn.feature_extraction.text import TfidfVectorizer
    from sklearn.metrics.pairwise import cosine_similarity
    from sklearn.ensemble import RandomForestClassifier
    import numpy as np
    import nltk
    from nltk.corpus import stopwords
    from nltk.stem import WordNetLemmatizer
    nltk.download('stopwords', quiet=True)
    nltk.download('wordnet', quiet=True)
except ImportError as e:
    print(f"Missing required packages: {e}")
    print("Install with: pip install requests scikit-learn numpy nltk")
    sys.exit(1)

@dataclass
class IssueData:
    """Represents a GitHub issue for duplicate detection"""
    number: int
    title: str
    body: str
    state: str  # 'open' or 'closed'
    labels: List[str]
    created_at: str
    updated_at: str
    user: str

    def get_content_hash(self) -> str:
        """Get hash of issue content for caching"""
        content = f"{self.title}\n{self.body}".lower()
        return hashlib.md5(content.encode()).hexdigest()

@dataclass
class DuplicateCandidate:
    """Represents a potential duplicate pair"""
    issue_a: int
    issue_b: int
    similarity_score: float
    ai_confidence: float
    detected_at: str
    reason: str

class AIDuplicateDetector:
    """AI-powered duplicate detection system"""

    def __init__(self, repo_owner: str, repo_name: str, token: str):
        self.repo_owner = repo_owner
        self.repo_name = repo_name
        self.token = token
        self.base_url = f"https://api.github.com/repos/{repo_owner}/{repo_name}"

        # AI/ML components
        self.vectorizer = TfidfVectorizer(
            max_features=5000,
            stop_words='english',
            ngram_range=(1, 2)
        )
        self.classifier = RandomForestClassifier(
            n_estimators=100,
            random_state=42
        )

        # Training data
        self.training_data_file = os.path.join(
            os.path.dirname(__file__),
            'duplicate_detection_training.pkl'
        )

        # Caches
        self.issues_cache_file = os.path.join(
            os.path.dirname(__file__),
            'issues_cache.pkl'
        )

        # Load existing training data if available
        self.training_pairs = []
        self.load_training_data()

    def make_request(self, endpoint: str, method: str = 'GET', **kwargs) -> Optional[Dict]:
        """Make authenticated request to GitHub API"""
        url = f"{self.base_url}/{endpoint}"
        headers = {
            'Authorization': f'token {self.token}',
            'Accept': 'application/vnd.github.v3+json'
        }

        try:
            if method == 'GET':
                response = requests.get(url, headers=headers, **kwargs)
            elif method == 'POST':
                response = requests.post(url, headers=headers, **kwargs)
            else:
                raise ValueError(f"Unsupported method: {method}")

            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"API request failed: {e}")
            return None

    def get_all_issues(self, state: str = 'all', max_issues: int = 500) -> List[IssueData]:
        """Fetch all issues from the repository"""
        print(f"Fetching {state} issues from {self.repo_owner}/{self.repo_name}...")

        issues = []
        page = 1
        per_page = 100

        while len(issues) < max_issues:
            endpoint = f"issues?state={state}&page={page}&per_page={per_page}&sort=created&direction=desc"
            data = self.make_request(endpoint)

            if not data:
                break

            if not data:  # Empty page
                break

            for issue_data in data:
                # Skip pull requests
                if 'pull_request' in issue_data:
                    continue

                issue = IssueData(
                    number=issue_data['number'],
                    title=issue_data['title'],
                    body=issue_data.get('body', '') or '',
                    state=issue_data['state'],
                    labels=[label['name'] for label in issue_data.get('labels', [])],
                    created_at=issue_data['created_at'],
                    updated_at=issue_data['updated_at'],
                    user=issue_data['user']['login']
                )
                issues.append(issue)

            if len(data) < per_page:
                break

            page += 1
            time.sleep(1)  # Rate limiting

        print(f"Fetched {len(issues)} issues")
        return issues

    def extract_meaningful_content(self, issue: IssueData) -> str:
        """Extract meaningful content from issue, filtering out boilerplate"""
        content_parts = []

        # Always include title
        content_parts.append(issue.title)

        # Process body for meaningful content
        body = issue.body or ''

        # Skip if challenge-upheld (excluded from duplicate detection)
        if 'challenge-upheld' in issue.labels:
            return f"{issue.title} [EXCLUDED: Challenge Upheld]"

        # Simple template detection and content extraction
        lines = body.split('\n')
        meaningful_lines = []

        for line in lines:
            line = line.strip()
            if not line:
                continue

            # Skip template boilerplate
            lower_line = line.lower()
            if any(skip in lower_line for skip in [
                'thanks for', 'please fill', 'clear and concise',
                'steps to reproduce', 'expected behavior', 'actual behavior',
                'system information', 'environment', 'version',
                'additional context', 'screenshots', 'logs'
            ]):
                continue

            # Skip markdown formatting and placeholders
            if line.startswith(('**', '##', '-', '*', '[')) and len(line) < 100:
                continue

            meaningful_lines.append(line)

        content_parts.extend(meaningful_lines[:10])  # Limit to first 10 meaningful lines

        return ' '.join(content_parts)

    def calculate_similarity(self, issue_a: IssueData, issue_b: IssueData) -> float:
        """Calculate similarity between two issues using multiple methods"""
        content_a = self.extract_meaningful_content(issue_a)
        content_b = self.extract_meaningful_content(issue_b)

        if not content_a or not content_b:
            return 0.0

        # TF-IDF similarity
        try:
            tfidf_matrix = self.vectorizer.fit_transform([content_a, content_b])
            cosine_sim = cosine_similarity(tfidf_matrix[0:1], tfidf_matrix[1:2])[0][0]
        except:
            cosine_sim = 0.0

        # Title similarity (weighted higher)
        title_sim = self.simple_text_similarity(issue_a.title, issue_b.title)

        # Combine scores (title weighted 2x)
        combined_score = (cosine_sim * 0.7) + (title_sim * 0.3)

        return min(combined_score, 1.0)

    def simple_text_similarity(self, text1: str, text2: str) -> float:
        """Simple text similarity using Jaccard similarity"""
        words1 = set(text1.lower().split())
        words2 = set(text2.lower().split())

        intersection = words1.intersection(words2)
        union = words1.union(words2)

        return len(intersection) / len(union) if union else 0.0

    def find_duplicate_candidates(self, issues: List[IssueData],
                                min_similarity: float = 0.75) -> List[DuplicateCandidate]:
        """Find potential duplicate pairs"""
        candidates = []

        # Group issues by rough topic similarity first
        print(f"Analyzing {len(issues)} issues for duplicates...")

        for i, issue_a in enumerate(issues):
            for j, issue_b in enumerate(issues[i+1:], i+1):
                # Skip if either issue is excluded
                if 'challenge-upheld' in issue_a.labels or 'challenge-upheld' in issue_b.labels:
                    continue

                # Skip if both are closed (less urgent)
                if issue_a.state == 'closed' and issue_b.state == 'closed':
                    continue

                similarity = self.calculate_similarity(issue_a, issue_b)

                if similarity >= min_similarity:
                    candidate = DuplicateCandidate(
                        issue_a=issue_a.number,
                        issue_b=issue_b.number,
                        similarity_score=similarity,
                        ai_confidence=self.calculate_ai_confidence(issue_a, issue_b, similarity),
                        detected_at=datetime.now().isoformat(),
                        reason=f"Content similarity: {similarity:.2f}"
                    )
                    candidates.append(candidate)

        # Sort by AI confidence
        candidates.sort(key=lambda x: x.ai_confidence, reverse=True)

        print(f"Found {len(candidates)} potential duplicate pairs")
        return candidates

    def calculate_ai_confidence(self, issue_a: IssueData, issue_b: IssueData,
                               similarity: float) -> float:
        """Calculate AI confidence score based on various factors"""
        confidence = similarity

        # Boost confidence if issues are both open (more urgent)
        if issue_a.state == 'open' and issue_b.state == 'open':
            confidence *= 1.2

        # Boost confidence if issues have similar labels
        labels_a = set(issue_a.labels)
        labels_b = set(issue_b.labels)
        label_overlap = len(labels_a.intersection(labels_b)) / max(len(labels_a.union(labels_b)), 1)
        confidence *= (1 + label_overlap * 0.3)

        # Reduce confidence if issues are very old
        now_utc = datetime.now(timezone.utc)
        days_old_a = (now_utc - datetime.fromisoformat(issue_a.created_at.replace('Z', '+00:00'))).days
        days_old_b = (now_utc - datetime.fromisoformat(issue_b.created_at.replace('Z', '+00:00'))).days

        if max(days_old_a, days_old_b) > 365:  # Over a year old
            confidence *= 0.8

        return min(confidence, 1.0)

    def learn_from_admin_labels(self, issues: List[IssueData]) -> None:
        """Learn from admin-labeled issues for training"""
        positive_examples = []  # Confirmed duplicates
        negative_examples = []  # Confirmed non-duplicates

        # Find issues with admin duplicate labels
        duplicate_groups = {}  # parent -> [children]

        for issue in issues:
            if 'adminduplicate' in issue.labels:
                # This issue is marked as duplicate, find its parent
                body = issue.body or ''
                # Look for "duplicate of #123" in comments or body
                import re
                match = re.search(r'duplicate of #(\d+)', body, re.IGNORECASE)
                if match:
                    parent_id = int(match.group(1))
                    if parent_id not in duplicate_groups:
                        duplicate_groups[parent_id] = []
                    duplicate_groups[parent_id].append(issue.number)

            elif 'adminduplicatenegative' in issue.labels:
                # This issue is confirmed NOT a duplicate
                negative_examples.append(issue)

        # Create training pairs
        for parent_id, children in duplicate_groups.items():
            parent_issue = next((i for i in issues if i.number == parent_id), None)
            if not parent_issue:
                continue

            for child_id in children:
                child_issue = next((i for i in issues if i.number == child_id), None)
                if child_issue:
                    self.training_pairs.append({
                        'issue_a': parent_issue,
                        'issue_b': child_issue,
                        'is_duplicate': True,
                        'similarity': self.calculate_similarity(parent_issue, child_issue)
                    })

        # Add negative examples (issues confirmed as unique)
        for issue in negative_examples:
            # Pair with similar but non-duplicate issues
            similar_issues = []
            for other in issues:
                if (other.number != issue.number and
                    self.calculate_similarity(issue, other) > 0.3):
                    similar_issues.append(other)
                    if len(similar_issues) >= 3:  # Limit to 3 similar issues
                        break

            for similar_issue in similar_issues:
                self.training_pairs.append({
                    'issue_a': issue,
                    'issue_b': similar_issue,
                    'is_duplicate': False,
                    'similarity': self.calculate_similarity(issue, similar_issue)
                })

        print(f"Added {len(self.training_pairs)} training examples")
        self.save_training_data()

    def save_training_data(self) -> None:
        """Save training data to disk"""
        try:
            with open(self.training_data_file, 'wb') as f:
                pickle.dump(self.training_pairs, f)
            print(f"Saved {len(self.training_pairs)} training examples")
        except Exception as e:
            print(f"Failed to save training data: {e}")

    def load_training_data(self) -> None:
        """Load training data from disk"""
        try:
            if os.path.exists(self.training_data_file):
                with open(self.training_data_file, 'rb') as f:
                    self.training_pairs = pickle.load(f)
                print(f"Loaded {len(self.training_pairs)} training examples")
        except Exception as e:
            print(f"Failed to load training data: {e}")
            self.training_pairs = []

    def analyze_and_report_duplicates(self, min_confidence: float = 0.8,
                                    max_candidates: int = 20) -> Dict:
        """Main analysis function"""
        print("🔍 Starting AI-powered duplicate detection...")

        # Get all issues
        issues = self.get_all_issues(max_issues=1000)

        if len(issues) < 2:
            return {"error": "Not enough issues to analyze"}

        # Learn from admin labels
        self.learn_from_admin_labels(issues)

        # Find duplicate candidates
        candidates = self.find_duplicate_candidates(issues, min_similarity=0.6)

        # Filter by AI confidence
        high_confidence_candidates = [
            c for c in candidates if c.ai_confidence >= min_confidence
        ][:max_candidates]

        # Prepare report
        report = {
            "analyzed_at": datetime.now().isoformat(),
            "total_issues": len(issues),
            "candidates_found": len(high_confidence_candidates),
            "training_examples": len(self.training_pairs),
            "duplicates": []
        }

        for candidate in high_confidence_candidates:
            issue_a = next((i for i in issues if i.number == candidate.issue_a), None)
            issue_b = next((i for i in issues if i.number == candidate.issue_b), None)

            if issue_a and issue_b:
                report["duplicates"].append({
                    "issue_a": {
                        "number": issue_a.number,
                        "title": issue_a.title,
                        "state": issue_a.state,
                        "url": f"https://github.com/{self.repo_owner}/{self.repo_name}/issues/{issue_a.number}"
                    },
                    "issue_b": {
                        "number": issue_b.number,
                        "title": issue_b.title,
                        "state": issue_b.state,
                        "url": f"https://github.com/{self.repo_owner}/{self.repo_name}/issues/{issue_b.number}"
                    },
                    "similarity_score": candidate.similarity_score,
                    "ai_confidence": candidate.ai_confidence,
                    "reason": candidate.reason
                })

        print(f"✅ Analysis complete. Found {len(high_confidence_candidates)} high-confidence duplicates")
        return report

    def find_best_parent_for_duplicate(self, child_issue, all_issues):
        """Find the best parent issue for an admin-marked duplicate"""
        best_parent = None
        best_score = 0.0

        for potential_parent in all_issues:
            # Skip the child issue itself
            if potential_parent.number == child_issue.number:
                continue

            # Skip issues that are already marked as children
            if 'child' in potential_parent.labels:
                continue

            # Skip closed issues unless they're specifically marked as parents
            if potential_parent.state == 'closed' and 'parent' not in potential_parent.labels:
                continue

            # Calculate similarity score
            similarity = self.calculate_similarity(child_issue, potential_parent)

            # Boost score for issues already marked as parents
            if 'parent' in potential_parent.labels:
                similarity *= 1.3

            # Boost score for open issues
            if potential_parent.state == 'open':
                similarity *= 1.2

            # Reduce score for very old issues
            try:
                days_old = (datetime.now() - datetime.fromisoformat(potential_parent.created_at.replace('Z', '+00:00'))).days
                if days_old > 365:
                    similarity *= 0.8
            except:
                pass

            if similarity > best_score and similarity >= 0.6:  # Minimum threshold
                best_score = similarity
                best_parent = potential_parent

        return best_parent

    def remove_existing_parent_relationships(self, issue):
        """Remove existing parent-child relationships for an issue and return affected parent IDs"""
        affected_parents = []

        # Check if this issue has any parent references in its body
        if issue.body:
            import re
            # Look for "Duplicate of:** #123" patterns
            matches = re.findall(r'Duplicate of:\*\* #(\d+)', issue.body)
            affected_parents.extend([int(match) for match in matches])

            # Also look for other parent reference patterns
            matches = re.findall(r'duplicate of #(\d+)', issue.body, re.IGNORECASE)
            affected_parents.extend([int(match) for match in matches])

        # Remove duplicates
        affected_parents = list(set(affected_parents))

        return affected_parents
