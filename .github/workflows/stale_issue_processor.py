#!/usr/bin/env python3
"""
Stale Issue Processor
Handles stale issue detection and automated cleanup
"""

import os
import requests
import argparse
from datetime import datetime, timedelta
from typing import List, Dict

class StaleIssueProcessor:
    """Processes stale and inactive issues"""
    
    def __init__(self, github_token: str, repo_owner: str, repo_name: str):
        self.github_token = github_token
        self.repo_owner = repo_owner
        self.repo_name = repo_name
        self.base_url = "https://api.github.com"
        
        # Configuration
        self.config = {
            'STALE_DAYS': 30,
            'AUTO_CLOSE_DAYS': 7,  # Days after stale warning
            'LABELS': {
                'STALE': 'stale',
                'NEEDS_INFO': 'needs-more-info',
                'KEEP_OPEN': 'keep-open',
                'AUTO_CLOSED': 'auto-closed'
            }
        }
        
        # Templates
        self.stale_message = '''⏰ **Stale Issue Notice**

This issue has been automatically marked as stale because it has been inactive for **{days} days**.

**To keep this issue active:**
- [ ] Add new information or updates
- [ ] Respond to any pending questions  
- [ ] Confirm the issue is still relevant

**This issue will be automatically closed in {auto_close_days} days** if no activity occurs.

**For maintainers:** Add the `{keep_open_label}` label to prevent auto-closure.

---
*This is an automated message from our issue management system.*'''

        self.auto_close_message = '''🤖 **Issue Automatically Closed**

This issue has been automatically closed due to inactivity. It was marked as stale {stale_days} days ago and has had no activity since then.

**To reopen this issue:**
- [ ] Add new relevant information
- [ ] Confirm the issue is still valid
- [ ] Remove the `{stale_label}` label
- [ ] Tag a maintainer for review

**Why was this closed?**
- Helps maintain a clean issue tracker
- Focuses attention on active issues
- Prevents outdated issues from cluttering searches

If this issue is still relevant, please don't hesitate to reopen it with updated information!

---
*This action was performed automatically by our issue management system.*'''
    
    def process_stale_issues(self):
        """Main function to process stale issues"""
        print("🔍 Scanning for stale issues...")
        
        # Get issues that need stale checking
        issues_needing_info = self._get_issues_with_label(self.config['LABELS']['NEEDS_INFO'])
        old_open_issues = self._get_old_open_issues()
        
        all_candidates = issues_needing_info + old_open_issues
        
        print(f"📊 Found {len(all_candidates)} potential stale issues to process")
        
        stale_count = 0
        closed_count = 0
        
        for issue in all_candidates:
            if self._should_skip_issue(issue):
                continue
                
            days_inactive = self._get_days_since_activity(issue)
            
            if self._is_already_stale(issue):
                # Check if should be closed
                if days_inactive >= self.config['STALE_DAYS'] + self.config['AUTO_CLOSE_DAYS']:
                    self._auto_close_issue(issue, days_inactive)
                    closed_count += 1
            elif days_inactive >= self.config['STALE_DAYS']:
                # Mark as stale
                self._mark_as_stale(issue, days_inactive)
                stale_count += 1
        
        print(f"✅ Processed stale issues: {stale_count} marked stale, {closed_count} auto-closed")
    
    def _get_issues_with_label(self, label: str) -> List[Dict]:
        """Get all open issues with a specific label"""
        try:
            headers = {'Authorization': f'token {self.github_token}'}
            url = f"{self.base_url}/repos/{self.repo_owner}/{self.repo_name}/issues"
            
            params = {
                'state': 'open',
                'labels': label,
                'per_page': 100,
                'sort': 'updated',
                'direction': 'asc'  # Oldest first
            }
            
            response = requests.get(url, headers=headers, params=params)
            response.raise_for_status()
            
            return response.json()
            
        except Exception as e:
            print(f"❌ Error fetching issues with label '{label}': {e}")
            return []
    
    def _get_old_open_issues(self) -> List[Dict]:
        """Get old open issues without recent activity"""
        try:
            headers = {'Authorization': f'token {self.github_token}'}
            url = f"{self.base_url}/repos/{self.repo_owner}/{self.repo_name}/issues"
            
            # Calculate date threshold
            threshold_date = datetime.now() - timedelta(days=self.config['STALE_DAYS'])
            
            params = {
                'state': 'open',
                'per_page': 100,
                'sort': 'updated',
                'direction': 'asc'  # Oldest first
            }
            
            response = requests.get(url, headers=headers, params=params)
            response.raise_for_status()
            
            issues = response.json()
            
            # Filter by date
            old_issues = []
            for issue in issues:
                updated_date = datetime.fromisoformat(issue['updated_at'].replace('Z', '+00:00'))
                if updated_date.replace(tzinfo=None) < threshold_date:
                    old_issues.append(issue)
            
            return old_issues
            
        except Exception as e:
            print(f"❌ Error fetching old open issues: {e}")
            return []
    
    def _should_skip_issue(self, issue: Dict) -> bool:
        """Check if issue should be skipped from stale processing"""
        # Skip pull requests
        if 'pull_request' in issue:
            return True
        
        # Skip if has keep-open label
        labels = [label['name'] for label in issue.get('labels', [])]
        if self.config['LABELS']['KEEP_OPEN'] in labels:
            return True
        
        # Skip if recently created (less than stale days old)
        created_date = datetime.fromisoformat(issue['created_at'].replace('Z', '+00:00'))
        days_since_created = (datetime.now(created_date.tzinfo) - created_date).days
        
        if days_since_created < self.config['STALE_DAYS']:
            return True
        
        return False
    
    def _get_days_since_activity(self, issue: Dict) -> int:
        """Get number of days since last activity on issue"""
        updated_date = datetime.fromisoformat(issue['updated_at'].replace('Z', '+00:00'))
        return (datetime.now(updated_date.tzinfo) - updated_date).days
    
    def _is_already_stale(self, issue: Dict) -> bool:
        """Check if issue is already marked as stale"""
        labels = [label['name'] for label in issue.get('labels', [])]
        return self.config['LABELS']['STALE'] in labels
    
    def _mark_as_stale(self, issue: Dict, days_inactive: int):
        """Mark an issue as stale"""
        try:
            issue_number = issue['number']
            
            # Add stale label
            self._add_labels(issue_number, [self.config['LABELS']['STALE']])
            
            # Add stale comment
            stale_comment = self.stale_message.format(
                days=days_inactive,
                auto_close_days=self.config['AUTO_CLOSE_DAYS'],
                keep_open_label=self.config['LABELS']['KEEP_OPEN']
            )
            
            self._add_comment(issue_number, stale_comment)
            
            print(f"🏷️  Marked issue #{issue_number} as stale ({days_inactive} days inactive)")
            
        except Exception as e:
            print(f"❌ Error marking issue #{issue['number']} as stale: {e}")
    
    def _auto_close_issue(self, issue: Dict, days_inactive: int):
        """Automatically close a stale issue"""
        try:
            issue_number = issue['number']
            
            # Add auto-closed label
            self._add_labels(issue_number, [self.config['LABELS']['AUTO_CLOSED']])
            
            # Add closure comment
            close_comment = self.auto_close_message.format(
                stale_days=self.config['AUTO_CLOSE_DAYS'],
                stale_label=self.config['LABELS']['STALE']
            )
            
            self._add_comment(issue_number, close_comment)
            
            # Close the issue
            self._close_issue(issue_number)
            
            print(f"🔒 Auto-closed issue #{issue_number} ({days_inactive} days inactive)")
            
        except Exception as e:
            print(f"❌ Error auto-closing issue #{issue['number']}: {e}")
    
    def _add_labels(self, issue_number: int, labels: List[str]):
        """Add labels to an issue"""
        try:
            headers = {
                'Authorization': f'token {self.github_token}',
                'Content-Type': 'application/json'
            }
            url = f"{self.base_url}/repos/{self.repo_owner}/{self.repo_name}/issues/{issue_number}/labels"
            
            response = requests.post(url, headers=headers, json={'labels': labels})
            response.raise_for_status()
            
        except Exception as e:
            print(f"❌ Error adding labels to #{issue_number}: {e}")
    
    def _add_comment(self, issue_number: int, comment: str):
        """Add a comment to an issue"""
        try:
            headers = {
                'Authorization': f'token {self.github_token}',
                'Content-Type': 'application/json'
            }
            url = f"{self.base_url}/repos/{self.repo_owner}/{self.repo_name}/issues/{issue_number}/comments"
            
            response = requests.post(url, headers=headers, json={'body': comment})
            response.raise_for_status()
            
        except Exception as e:
            print(f"❌ Error adding comment to #{issue_number}: {e}")
    
    def _close_issue(self, issue_number: int):
        """Close an issue"""
        try:
            headers = {
                'Authorization': f'token {self.github_token}',
                'Content-Type': 'application/json'
            }
            url = f"{self.base_url}/repos/{self.repo_owner}/{self.repo_name}/issues/{issue_number}"
            
            response = requests.patch(url, headers=headers, json={'state': 'closed'})
            response.raise_for_status()
            
        except Exception as e:
            print(f"❌ Error closing issue #{issue_number}: {e}")

def main():
    parser = argparse.ArgumentParser(description='Process stale issues')
    parser.add_argument('--token', required=True, help='GitHub token')
    parser.add_argument('--owner', required=True, help='Repository owner')
    parser.add_argument('--repo', required=True, help='Repository name')
    parser.add_argument('--dry-run', action='store_true', help='Show what would be done without making changes')
    
    args = parser.parse_args()
    
    if args.dry_run:
        print("🧪 DRY RUN MODE - No changes will be made")
    
    processor = StaleIssueProcessor(args.token, args.owner, args.repo)
    processor.process_stale_issues()
    
    print("✅ Stale issue processing completed")

if __name__ == "__main__":
    main()
