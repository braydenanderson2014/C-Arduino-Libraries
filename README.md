# **C-Arduino-Libraries**  
🚀 **A collection of optimized C++ libraries for Arduino and embedded systems**  

## 📑 **Table of Contents**

- [Project Structure](#-project-structure)
- [Support My Work](#-support-my-work)
- [Special Thanks](#-special-thanks-for-those-who-sponser-my-work)
- [Library Testing & Stability](#-library-testing--stability)
- [Available Libraries](#-available-libraries)
- [Stable & Tested Libraries](#-stable--tested-libraries-)
- [Active Development Libraries](#-active-development-libraries-)
- [Currently Testing Libraries](#-currently-testing-libraries-)
- [Unpublished Libraries (In Development)](#-unpublished-libraries-in-development)
- [Libraries Published on Arduino Library Manager](#-libraries-published-on-arduino-library-manager)
- [Contributing & Issues](#-contributing--issues)
- [Automation, Templates, and Cross-Repo Intake](#-automation-templates-and-cross-repo-intake)
- [Automation: What Gets Copied Here From Other Library Repositories](#-what-gets-copied-here-from-other-library-repositories)
- [Automation: Issue Templates In This Repository](#-issue-templates-in-this-repository)
- [Automation: Main Issue/PR Automation Workflows](#-main-issuepr-automation-workflows)
- [Automation: If Automation Does Something You Do Not Understand](#-if-automation-does-something-you-do-not-understand)
- [Final Notes](#-final-notes)
- [Quick Links](#-quick-links)
- [All Libraries Include A README.md](#-all-libraries-include-a-readmemd-)
- [PlatformIO Leaderboard for These Libraries](#-platformio-leaderboard-for-these-libraries)
- [Information About This Document](#-information-about-this-document-)

## 📁 **Project Structure**  
All libraries are located under the `lib` folder. For best results, open the **parent folder** containing this `README.md` to access the full project.  

- 📌 The **`src`** folder contains the main project file (`main.cpp`) for testing libraries.  
- 🧪 The **`Test`** folder allows unit testing using the **Unity** framework. Enable it in `platformio.ini` when needed.  
- 📖 Each library has its own README file with setup instructions.  
- 🔍 Some libraries are also available on **Arduino Library Manager**. See the list below for version details.  

---
## 💖 **Support My Work**  
If you find these libraries useful, consider supporting me:  

🔗 **[Donate via GoFundMe](https://gofund.me/923e5f10)**  
💰 **Click the Sponsor Button** at the top of this repository!  
 - **Patreon:** [Support on Patreon](https://www.patreon.com/posts/122298248)
 - **Kofi:** [Support on Kofi](https://ko-fi.com/joesupercool15673)
 - **Venmo:** [Support on Venmo](https://venmo.com/Brayden-Anderson-20)
---
## SPECIAL THANKS FOR THOSE WHO SPONSER MY WORK:
* Brayden Anderson (Developer)



## ⚠ **Library Testing & Stability**  
Most libraries are **[PARTIALLY-TESTED]** or **[MOSTLY-TESTED]**. Even **[STABLE]** libraries may contain unknown bugs. If you encounter issues, **please report them** on this repository.  

⚠ **Note:** Some libraries have different published names. For example:  
- **Properties** → Published as **SimpleProperties**  
- **iostream** → Published as **BasicIOStream**  

📌 **[Development Branch]:** Libraries here are in **active development** and may not compile. Use **main** branch versions for stable releases.

---
## 📚 **Available Libraries**  
* [Dictionary](lib/Dictionary/README.md) [DEVELOPMENT-COMPLETE] (PURE VIRTUAL : No Concrete Implementation in this Library), [TESTING-NOT-NECESSARY],[IMPLEMENTED IN ANOTHER LIBRARY]
* [Hashtable](lib/Hashtable/README.md) [PASSIVE-DEVELOPMENT], [TESTED], [STABLE]
* [iostream](lib/iostream/README.md) [PASSIVE-DEVELOPMENT] (Published as : BasicIOStream), [PARTIALLY-TESTED], [STABLE]
* [iterator](lib/iterator/README.md) [DEVELOPMENT-COMPLETE], (PURE VIRTUAL : No Concrete Implementation in this Library)  [TESTING-NOT-NECESSARY] 
* [Properties](lib/Properties/README.md) [PASSIVE-DEVELOPMENT] (Published as : SimpleProperties), [MOSTLY-TESTED], [STABLE]
* [SimpleVector](lib/SimpleVector/README.md) [PASSIVE-DEVELOPMENT] , [TESTED], [STABLE]
* [Timer](lib/Timer/README.md) [PASSIVE_DEVELOPMENT] (Published as : SimpleArduinoTimer), [MOSTLY-TESTED], [SEMI-STABLE]
* [UnorderedMap](lib/UnorderedMap/README.md) [PASSIVE-DEVELOPMENT], [PARTIALLY-TESTED], [STABLE]
* [LinkedList](lib/LinkedList/README.md) [PASSIVE-DEVELOPMENT] (Published as : BasicLinkedList), [PARTIALLY-TESTED], [STABLE]
* [DoubleLinkedList](lib/DoubleLinkedList/README.md) [PASSIVE-DEVELOPMENT], [PARTIALLY-TESTED], [STABLE]
* [MyDictionary](lib/MyDictionary/README.md) [ACTIVE-DEVELOPMENT],(implemented Version of the Dictionary Class), [UNTESTED]
* [SDList](lib/SDList/README.md) [PASSIVE-DEVELOPMENT], [REIMPLEMENTED], [MOSTLY-TESTED], [UNSTABLE]
* [ArrayList](lib/ArrayList/README.md) [PASSIVE-DEVELOPMENT], [TESTED], [STABLE]
* [SimpleMutex](lib/SimpleMutex/) [ACTIVE-DEVELOPMENT]
* [SimpleThreadManager](lib/SimpleThreadManager/) [ACTIVE-DEVELOPMENT]
* [Operators](lib/Operators/README.md) [PASSIVE-DEVELOPMENT], [STABLE]
* [Predicates](lib/Predicates/README.md) [PASSIVE-DEVELOPMENT], [STABLE]
* [TypeTraits](lib/TypeTraits/README.md) [PASSIVE-DEVELOPMENT], [STABLE]
* [Numeric_Limits](lib/Numeric_Limits/README.md) [PASSIVE-DEVELOPMENT], [STABLE]
* [Map](lib/Map/README.md) [ACTIVE-DEVELOPMENT], [STABLE], [PARTIALLY-TESTED]
* [Queue](lib/Queue/README.md) [PASSIVE-DEVELOPMENT], [STABLE]
* [Stack](lib/Stack/README.md) [PASSIVE-DEVELOPMENT], [STABLE]
* [MatrixMath](lib/MatrixMath/README.md) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [MathLib](lib/MathLib/README.md) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [ColorManager](lib/ColorManager/README.md) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [InitializerList](lib/Initializer_List/README.md) [PASSIVE-DEVELOPMENT], [UNTESTED]
* [Optional](lib/Optional/README.md) [ACTIVE-DEVELOPMENT], [MOSTLY-TESTED]
* [Variant](lib/Variant/README.md) [ACTIVE-DEVELOPMENT], [MOSTLY-TESTED]
* [ExtremeVariant](lib/ExtremeVariant/README.md) [ACTIVE-DEVELOPMENT], [MOSTLY-TESTED]


Total Available Libraries: (45)
Total Libraries: (51)

### 🏆 **Stable & Tested Libraries** ✅  
| Library | Status | Notes |
|---------|--------|-------|
| [Dictionary](lib/Dictionary/README.md) | ✅ DEVELOPMENT-COMPLETE | Pure Virtual (No Concrete Implementation) |
| [Hashtable](lib/Hashtable/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [SimpleVector](lib/SimpleVector/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [ArrayList](lib/ArrayList/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, ⚠ SEMI-STABLE |
| [Timer](lib/Timer/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ MOSTLY-TESTED, ⚠ SEMI-STABLE |
| [LinkedList](lib/LinkedList/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ PARTIALLY-TESTED, 🏆 STABLE |
| [UnorderedMap](lib/UnorderedMap/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ PARTIALLY-TESTED, 🏆 STABLE |
| [Stack](lib/Stack/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [Queue](lib/Queue/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [Operators](lib/Operators/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |
| [Predicates](lib/Predicates/README.md) | 🛠 PASSIVE-DEVELOPMENT | ✅ TESTED, 🏆 STABLE |

---

### 🔬 **Active Development Libraries** ⚡  
| Library | Status | Notes |
|---------|--------|-------|
| [MyDictionary](lib/MyDictionary/README.md) | 🚧 DEVELOPMENT-PAUSED | ⚠ UNTESTED |
| [SimpleMutex](lib/SimpleMutex/) | 🚧 DEVELOPMENT-PAUSED | ⚠ UNTESTED |
| [SimpleThreadManager](lib/SimpleThreadManager/) | 🚧 DEVELOPMENT-PAUSED | ⚠ UNTESTED |
| [JSON](lib/Json/) | 🔥 ACTIVE-DEVELOPMENT | 🚧 UNPUBLISHED |
| [MemoryManager](lib/MemoryManager/README.md) | 🔥 ACTIVE-DEVELOPMENT | 🚧 UNPUBLISHED |
| [Optional](lib/Optional/README.md) | 🔥 ACTIVE-DEVELOPMENT | ✅ MOSTLY-TESTED |
| [Variant](lib/Variant/README.md) | 🔥 ACTIVE-DEVELOPMENT | ✅ MOSTLY-TESTED |
| [ExtremeVariant](lib/ExtremeVariant/README.md) | 🔥 ACTIVE-DEVELOPMENT | ✅ MOSTLY-TESTED |
| [DynamicStorage](lib/DynamicStorageLibrary/README.md) | 🔥 ACTIVE-DEVELOPMENT | 🚧 IN PROGRESS |


---
## 🧪 **Currently Testing Libraries** 🔍  
🚧 These libraries are under active testing:  
1. [MatrixMath](lib/MatrixMath/README.md)  🚧 TESTING-PAUSED
2. [MathLib](lib/MathLib/README.md)  🚧 TESTING-PAUSED
3. [JSON](lib/Json/) 🔥 ACTIVELY-TESTING
4. [Optional](lib/Optional/README.md) 🔥 ACTIVELY-TESTING
5. [Variant](lib/Variant/README.md) 🛠 PASSIVE-TESTING
6. [ExtremeVariant](lib/ExtremeVariant/README.md) 🔥 ACTIVELY-TESTING
7. [ArrayList](lib/ArrayList/README.md) 🛠 PASSIVE-TESTING
8. [SimpleVector](lib/SimpleVector/README.md) 🛠 PASSIVE-TESTING
---

## 🚧 **Unpublished Libraries (In Development)**  
📢 These libraries are **not yet available** but are under **active development**:  

| Library | Status |
|---------|--------|
| SimpleMutex | 🚧 DEVELOPMENT-PAUSED, ❌ UNPUBLISHED |
| SimpleThreadManager | 🚧 DEVELOPMENT-PAUSED, ❌ UNPUBLISHED |
| OrderedMap | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| JSON | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| STRING | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| MemoryManager | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| ExtremeVariant | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |
| DynamicStorage | 🔥 ACTIVE-DEVELOPMENT, ❌ UNPUBLISHED |

---

## 📜 **Libraries Published on Arduino Library Manager**  

- **ArrayList** `[ON-PAR]`  
- **SimpleVector** `[ON-PAR]`  
- **Hashtable** `[ON-PAR]`  
- **Properties** (SimpleProperties)  
- **Timer** (ArduinoTimer)  
- **LinkedList** (ArduinoLinkedList)  
- **UnorderedMap**  
- **DoubleLinkedList** (ArduinoDoubleLinkedList)  
- **SDList** (Arduino SDList) `[ON-PAR]`  
- **Optional** (Will be synced between Versions shortly (made a mistake))  
- **Variant** `[ON-PAR]`  

## 📢 **Contributing & Issues**  
🛠 Found a bug? Have a question? **Open an issue** on GitHub!  

💡 Want to contribute? Fork the repository and submit a **pull request** with your changes.  

---

## 🤖 **Automation, Templates, and Cross-Repo Intake**

This repository is the **main tracker** for issues and pull-request intake across related library repositories.

### ✅ What Gets Copied Here From Other Library Repositories

The workflow `.github/workflows/forward-issues-to-main.yml` is designed to run in **separate library repositories** (not this main repo).

- When a new **issue** is opened in a library repo, it is mirrored as an issue in `braydenanderson2014/C-Arduino-Libraries`.
- When a new **pull request** is opened in a library repo, it is also mirrored here as an issue so discussion can stay centralized.
- The mirrored issue is added to project `braydenanderson2014/4` and set to `Status = Triage`.
- The mirrored issue gets `Needs Transfer = Yes`, and after forwarding completes it is set to `Needs Transfer = Completed Transfer`.
- By default, the source library-repo issue/PR is then closed after mirroring (configurable through workflow_dispatch inputs).

Important behavior:
- The forwarding workflow **self-disables in this main repo** (`C-Arduino-Libraries`) to prevent loops.
- This means each library repo should have its own copy of that workflow file, with token/permission setup.

### 🧩 Issue Templates In This Repository

Templates in `.github/ISSUE_TEMPLATE/` standardize intake and labels:

- `bug_report.yml`: applies `bug` + `user-reported`
- `feature_request.yml`: applies `enhancement` + `feature-request`
- `documentation-request.yml`: applies `documentation`
- `general-question.yml`: applies `question`
- `config.yml`: disables blank issues (`blank_issues_enabled: false`)

Each template also includes a **Repository / Library** selector so reports can still be routed correctly in one central tracker.

### 🗂️ Main Issue/PR Automation Workflows

Core workflow behaviors in `.github/workflows/`:

- `welcome-messages.yml`:
	- Sends automated first response
	- Adds issue to project board and sets fields like `Status`, `Type`, and (feature) `Priority`
	- Supports retroactive welcome with `/welcome` comment or `send-welcome` label

- `duplicate-detection.yml`:
	- Runs AI + similarity duplicate detection on issue open
	- Uses `scripts/ai_duplicate_detector.py` for ML-assisted candidate generation
	- Labels potential duplicates (`auto-potential-duplicate`) or auto-closes very high-confidence duplicates
	- Supports admin override labels:
		- `adminduplicate` = confirm duplicate and close
		- `adminduplicatenegative` = mark as not duplicate and remove auto-duplicate labels
	- Maintains parent/child links and duplicate rollups in parent issue body

- `challenge-handler.yml`:
	- Handles duplicate challenge flow via labels or commands
	- Commands/labels can trigger:
		- challenge started
		- challenge upheld
		- challenge failed (with optional admin reason)
	- Updates project fields like `Status`, `Archive Bucket`, and `Reopened Issue`

- `restricted-label-watchdog.yml`:
	- Protects restricted labels on both issues and pull requests
	- Automatically removes restricted labels if added by unauthorized users
	- Automatically restores restricted labels if unauthorized users remove them
	- Allows trusted system actors (for example `github-actions[bot]`) and maintainers/admins

- `issue-reopen-handler.yml`:
	- On issue reopen, sets `Status = REOPENED!!!` unless challenge-override labels are present

- `working-status.yml`, `question-status.yml`, `documentation-status.yml`:
	- Keep project `Status` synchronized when labels are added/removed
	- Examples: move between `Triage`, `Backlog`, `General Question`, `Documentation Request`, and in-progress states

- `needs-more-info.yml`:
	- Posts an info-request message when `needs-more-info` is added
	- Posts follow-up confirmation when that label is removed after updates

- `pull-request-triage.yml`:
	- Auto-labels PRs with `pull-request`
	- Adds welcome comment
	- Adds PR to project board and sets `Status = Triage`

- `user-closed-handler.yml`:
	- If `user-closed` label exists, updates project fields (`Archive Bucket = User-Closed`, `Status = Done`)

- `extract-program-id.yml`:
	- Extracts Program ID patterns from issue body and writes to project `ID` field

- `Project Health Monitoring`:
	- Contains scheduled Project V2 health-summary logic based on active work-item counts
	- Note: the file is currently named without `.yml`/`.yaml`; GitHub Actions only executes workflow files with a workflow extension

### 🆘 If Automation Does Something You Do Not Understand

When a label/comment action is confusing (for example duplicate detection):

1. Check labels on the issue first (`duplicate`, `parent`, `child`, `auto-potential-duplicate`, `challenge-*`, `adminduplicate*`, `needs-more-info`, `working`).
2. Read bot comments in order; automation writes reason/context comments for most actions.
3. If duplicate handling looks wrong:
	 - Add details showing why the issue is unique.
	 - Add `challenge` label (or comment command for challenge flow).
	 - Maintainer can mark `challenge-upheld` or `challenge-failed`.
4. If AI marked a false duplicate:
	 - Use the `challenge` mechanism (this is the correct path for non-maintainers).
	 - Add specific details showing how your case differs from the parent/linked issue.
5. If a true duplicate was missed:
	 - Maintainer/admin can use `adminduplicate` to force confirmed-duplicate flow.
6. If the issue was auto-closed but should be active:
	 - Reopen and explain what is new.
	 - Apply `challenge` when closure reason was duplicate-related.
7. Maintainer-only: if a library-repo issue/PR did not mirror here:
	 - Verify `GH_PROJECT_TOKEN` exists in that source repo.
	 - Verify the source repo has `forward-issues-to-main.yml`.
	 - Verify project fields/options still match expected names (for example `Needs Transfer`, `Status`, `Triage`).

---

## 📜 **Final Notes**  
- **Libraries marked as `[ON-PAR]`** have the same version in **Arduino Library Manager** and **PlatformIO**.  
- **If a library is not `[ON-PAR]`**, the **Arduino version may be outdated**. Updates on **PlatformIO** happen first due to ease of deployment.  

🚀 **Happy coding & thank you for using my libraries!** 🚀  

---

## 🎯 **Quick Links**  
🔗 **[GoFundMe - Support My Work!](https://gofund.me/923e5f10)**
🔗 **[Patreon - Support My Work!](https://www.patreon.com/posts/support-my-open-122298248?utm_medium=clipboard_copy&utm_source=copyLink&utm_campaign=postshare_creator&utm_content=join_link)**  
🔗 **[Platformio Registry](https://registry.platformio.org/search?q=owner%3Abraydenanderson2014)**  
🔗 **[GitHub Repository](https://github.com/braydenanderson2014/C-Arduino-Libraries)**  
🔗 **[Arduino Library Manager](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#using-the-library-manager)** 
 

## ALL LIBRARIES INCLUDE A README.md ##

## 📊 **PlatformIO Leaderboard for These Libraries**  
📅 **Updated:** *01/29/2025 09:08 PM Mountain Time*  

| Library Name | Position | Downloads MTD | Total Downloads |
|--------------|----------|---------------|-----------------|
| [ArrayList](lib/ArrayList/README.md) | 1,186 | 22 | 178 | 
| [Dictionary](lib/Dictionary/README.md) | 14,128 | 0 | 16 |
| [SimpleArduinoTimer](lib/Timer/README.md) | 3,947 | 2 | 16 |
| [UnorderedMap](lib/UnorderedMap/README.md) | 3,952 | 2 | 46 |
| [SimpleProperties](lib/Properties/README.md) | 3,948 | 2 | 46 |
| [Iterator](lib/iterator/README.md) | 3,949 | 2 | 25 |
| [BasicIO](lib/iostream/README.md) | 1,679 | 12 | 360 |
| [SDList](lib/SDList/README.md) | 3,951 | 2 | 12 |
| [Hashtable](lib/Hashtable/README.md) | 1,375 | 17 | 189 |
| [SimpleVector](lib/SimpleVector/README.md) | 1,259 | 21 | 247 |
| [MyDictionary](lib/MyDictionary/README.md) | 14,136 | 0 | 3 |
| [Operators](lib/Operators/README.md) | 14,221 | 0 | 15 | 
| [Predicates](lib/Predicates/README.md) | 14,222 | 0 | 15 |
| [BasicLinkedList](lib/LinkedList/README.md) | 2,818 | 4 | 20 |
| [DoubleLinkedList](lib/DoubleLinkedList/README.md) | 3,250 | 3 | 21 |
| [TypeTraits](lib/TypeTraits/README.md) | 3,980 | 2 | 28 |
| [AVLTree](lib/AVLTree/README.md) | 14,422 | 0 | 4 |
| [BinarySearchTree](lib/BinarySearchTree/README.md) | 14,423 | 0 | 4 |
| [BinaryTree](lib/BinaryTree/README.md) | 14,424 | 0 | 4 |
| [B+ Tree](lib/bPlusTree/README.md) | 14,425 | 0 | 4 |
| [B Tree](lib/bTree/README.md) | 14,426 | 0 | 4 |
| [ColorManager](lib/ColorManager/README.md) | 14,427 | 0 | 5 |
| [FenwickTree](lib/FenwickTree/README.md) | 14,428 | 0 | 4 |
| [HeapTree](lib/HeapTree/README.md) | 14,429 | 0 | 4 |
| [InitializerList](lib/Initializer_List/README.md) | 1,379 | 18 | 217 |
| [IntervalTree](lib/IntervalTree/README.md) | 14,430 | 0 | 4 |
| [KDimensionalTree](lib/kdimensionalTree/README.md) | 14,431 | 0 | 4 |
| [MathLib](lib/MathLib/README.md) | 3,982 | 2 | 21 |
| [Map](lib/Map/README.md) | 2,726 | 1 | 38 |
| [MatrixMath](lib/MatrixMath/README.md) | 14,444 | 0 | 4 |
| [Numeric_Limits](lib/Numeric_Limits/README.md) | 3,983 | 2 | 25 |
| [OCTree](lib/OCTree/README.md) | 14,432 | 0 | 4 |
| [QuadTree](lib/QuadTree/README.md) | 14,436 | 0 | 4 |
| [Queue](lib/Queue/README.md) | 14,437 | 0 | 5 |
| [RedBlackTree](lib/RedBlackTree/README.md) | 14,438 | 0 | 4 |
| [RTree](lib/RTree/README.md) | 14,439 | 0 | 4 |
| [SegmentTree](lib/SegmentTree/README.md) | 14,440 | 0 | 4 |
| [Stack](lib/Stack/README.md) | 14,441 | 0 | 6 |
| [SuffixTree](lib/SuffixTree/README.md) | 14,442 | 0 | 4 |
| [TrieTree](lib/TrieTree/README.md) | 14,443 | 0 | 4 | 
| [Optional](lib/Optional/README.md) | [NEW] | 0 | 0 |
| [Variant](lib/Variant/README.md) | [NEW] | 0 | 0 | 
| [ExtremeVariant](lib/ExtremeVariant/README.md) | [NEW] | 0 | 0 |


## Information about this document ##
| Status | Meaning |
|--------|---------|
|[TESTED] | Library has been tested and found to operate as intended (newer updates may not actually be tested) |
| [PARTIALLY-TESTED] | Library has had some tests completed, the library could still be unstable |
| ✅ [MOSTLY-TESTED] ✅ | Similar to partially tested, most functions have been tested. (main functions) |
| [UNTESTED] | If I have to explain this to you, you got problems ;) Library has not been tested |
| ❌ [UNPUBLISHED] ❌ | The library is only on github. This is usually a temporary tag. |
| [DEVELOPMENT-COMPLETE] | I have no intention to further develop library. (will fix bugs) |
| 🔥 [ACTIVE-DEVELOPMENT] 🔥 | Actively working on library. Enhancements, bug fixes, ext. | 
| 🛠 [PASSIVE-DEVELOPMENT] 🛠 | Resevered for libraries that I have plans for, but am not working on currently |
| 🚧 [DEVELOPMENT-PAUSED] 🚧 | Development is no longer a priority, and the Development will not be progressed  |
| 🚧 [TESTING-PAUSED] 🚧 | Testing is no longer a priority, and at this time will not be continued.
| 🏆 [STABLE] 🏆 | Library should work as intended and shouldnt give any trouble. |
| [CONSIDERED-STABLE] | Library hasnt been fully tested, but the vast majority of the functions seem to work fine with no issues |
| ⚠ [SEMI-STABLE] ⚠ | Library has some aspects that are stable, but others that may not be stable |
| [UNSTABLE] | Testing is showing that a library is not working properly. Even though a library is not labled with this, please consider all libraries on the Development-Branch to be Unstable. Some libraries on main may also be unstable if it had to be merged early. |
| [REIMPLEMENTED] | Library was rebuilt from the ground up. Functionality may have changed |
| [TESTING-NOT-NECESSARY] | Library is Virtual and has no concrete implementation |
| [ON-PAR] | Means the Arduino Current Version and the Platformio Current Version are the same. This is not always true, so this label is used to show that they are currently in sync. Absense of this label should be taken as the library versions are not identical. |
| 🚧 [IN-PROGRESS] 🚧 | Essentially this and Active development are the same. However this means this library is priority |
