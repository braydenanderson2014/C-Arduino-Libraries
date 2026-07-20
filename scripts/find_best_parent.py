#!/usr/bin/env python3
"""
AI-Powered Best Parent Finder for Admin-Marked Duplicates

This script uses the AI detector to find the best parent issue for an
admin-confirmed duplicate.
"""

import sys
import os
import json
import argparse
from ai_duplicate_detector import AIDuplicateDetector, IssueData

def main():
    parser = argparse.ArgumentParser(description='Find best parent for admin-marked duplicate')
    parser.add_argument('--input', required=True, help='Input JSON file with child issue and all issues')
    parser.add_argument('--repo-owner', required=True, help='GitHub repository owner')
    parser.add_argument('--repo-name', required=True, help='GitHub repository name')
    parser.add_argument('--token', required=True, help='GitHub API token')

    args = parser.parse_args()

    # Load input data
    with open(args.input, 'r') as f:
        data = json.load(f)

    # Convert to IssueData objects
    child_issue = IssueData(
        number=data['child_issue']['number'],
        title=data['child_issue']['title'],
        body=data['child_issue']['body'],
        state=data['child_issue']['state'],
        labels=data['child_issue']['labels'],
        created_at=data['child_issue']['created_at'],
        updated_at=data['child_issue']['updated_at'],
        user='unknown'
    )

    all_issues = []
    for issue_data in data['all_issues']:
        issue = IssueData(
            number=issue_data['number'],
            title=issue_data['title'],
            body=issue_data['body'],
            state=issue_data['state'],
            labels=issue_data['labels'],
            created_at=issue_data['created_at'],
            updated_at=issue_data['updated_at'],
            user='unknown'
        )
        all_issues.append(issue)

    # Initialize AI detector
    detector = AIDuplicateDetector(args.repo_owner, args.repo_name, args.token)

    # Find best parent
    best_parent = detector.find_best_parent_for_duplicate(child_issue, all_issues)

    if best_parent:
        result = {
            'best_parent': {
                'number': best_parent.number,
                'title': best_parent.title,
                'similarity_score': detector.calculate_similarity(child_issue, best_parent)
            },
            'confidence': 'high' if detector.calculate_similarity(child_issue, best_parent) > 0.8 else 'medium'
        }
        print(json.dumps(result))
    else:
        print(json.dumps({'best_parent': None, 'confidence': 'none'}))

if __name__ == '__main__':
    main()
