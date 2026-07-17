#!/usr/bin/env python3
"""
GitHub Actions environment validator for Advanced Issue Manager
Checks dependencies and environment before running the main script
"""

import sys
import os

def check_environment():
    """Check if the environment is ready for the advanced issue manager"""
    print("=== GitHub Actions Environment Check ===")
    
    # Check Python version
    print(f"Python version: {sys.version}")
    print(f"Python executable: {sys.executable}")
    
    # Check if we're using the right Python command
    import subprocess
    try:
        result = subprocess.run(['py', '--version'], capture_output=True, text=True, timeout=10)
        if result.returncode == 0:
            print(f"py command available: {result.stdout.strip()}")
        else:
            print(f"py command not available (exit code: {result.returncode})")
    except (subprocess.TimeoutExpired, FileNotFoundError):
        print("py command not found, using python instead")
    
    # Check required environment variables
    required_env_vars = ['GITHUB_TOKEN', 'ISSUE_NUMBER']
    for var in required_env_vars:
        value = os.environ.get(var)
        if value:
            print(f"✓ {var}: {'*' * min(len(value), 10)}...")
        else:
            print(f"✗ {var}: Not set")
    
    # Check script exists
    script_path = os.path.join('scripts', 'advanced_issue_manager.py')
    if os.path.exists(script_path):
        print(f"✓ Script exists: {script_path}")
        print(f"  Size: {os.path.getsize(script_path)} bytes")
    else:
        print(f"✗ Script not found: {script_path}")
        return False
    
    # Test imports
    dependencies = {
        'requests': 'HTTP client for GitHub API',
        'json': 'JSON processing',
        'datetime': 'Date/time handling',
        'sklearn': 'Machine learning (optional)',
        'nltk': 'Natural language processing (optional)',
        'numpy': 'Numerical computing (optional)'
    }
    
    for dep, description in dependencies.items():
        try:
            __import__(dep)
            print(f"✓ {dep}: Available - {description}")
        except ImportError:
            if dep in ['sklearn', 'nltk', 'numpy']:
                print(f"⚠ {dep}: Optional dependency not available - {description}")
            else:
                print(f"✗ {dep}: Required dependency missing - {description}")
                return False
    
    # Test basic functionality
    try:
        sys.path.append('scripts')
        from advanced_issue_manager import IssueAutoManager
        
        # Test with fake credentials
        manager = IssueAutoManager("fake_token", "test_owner", "test_repo")
        print("✓ Advanced Issue Manager can be instantiated")
        
        return True
        
    except Exception as e:
        print(f"✗ Error testing Advanced Issue Manager: {e}")
        import traceback
        traceback.print_exc()
        return False

if __name__ == "__main__":
    success = check_environment()
    
    if success:
        print("\n✅ Environment check passed! Ready to run Advanced Issue Manager.")
        sys.exit(0)
    else:
        print("\n❌ Environment check failed! Check the errors above.")
        sys.exit(1)
