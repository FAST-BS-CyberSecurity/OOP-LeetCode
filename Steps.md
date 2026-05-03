# OOPs Project Team Workflow

## Main Idea

We will build the project phase by phase. In each phase, all three members will work on different files/classes. After one member completes their assigned file, they upload it to the common GitHub repository. Then the other members pull the latest code and continue their own work.

Because the team does not know Git branches yet, everyone can work directly on `main`, but with strict rules.

The most important rule:

```text
One person should work on one file at a time.
Do not let two people edit the same file together.
```

## Tools We Will Use

- GitHub for common source code storage.
- VS Code for editing code.
- Git commands for uploading and downloading code.
- One shared repository for the whole team.

Repository example name:

```text
OOP-Online-Judge
```

## Basic Project Folder Structure

Use this structure:

```text
OOP-Online-Judge/
  include/
    CustomString.h
    DynamicArray.h
    LinkedList.h
    KeyValueStore.h
    VFSManager.h
    User.h
    UserManager.h
    Session.h
    Problem.h
    TestCase.h
    ProblemBank.h
    Contest.h
    EasyContest.h
    BalancedContest.h
    HardContest.h
    ContestManager.h
    Submission.h
    EvaluationEngine.h
    OutputComparator.h
    ScoreCalculator.h
    UIManager.h

  src/
    CustomString.cpp
    DynamicArray.cpp
    LinkedList.cpp
    KeyValueStore.cpp
    VFSManager.cpp
    User.cpp
    UserManager.cpp
    Session.cpp
    Problem.cpp
    TestCase.cpp
    ProblemBank.cpp
    Contest.cpp
    EasyContest.cpp
    BalancedContest.cpp
    HardContest.cpp
    ContestManager.cpp
    Submission.cpp
    EvaluationEngine.cpp
    OutputComparator.cpp
    ScoreCalculator.cpp
    UIManager.cpp

  docs/
    UML.pdf
    Steps.md

  main.cpp
  README.md
  Makefile
```

## Simple GitHub Workflow Without Branches

Everyone works directly on `main`.

### Step 1: Clone The Repository

Each person opens VS Code terminal and runs:

```bash
git clone https://github.com/your-username/OOP-Online-Judge.git
cd OOP-Online-Judge
code .
```

### Step 2: Before Starting Work

Every person must run:

```bash
git pull
```

This downloads the latest code from GitHub.

Do not start coding before running `git pull`.

### Step 3: Work Only On Assigned Files

Example:

```text
Person 1 works only on:
  include/CustomString.h
  src/CustomString.cpp

Person 2 works only on:
  include/DynamicArray.h
  src/DynamicArray.cpp

Person 3 works only on:
  include/LinkedList.h
  src/LinkedList.cpp
```

Do not edit another person's file unless the group agrees.

### Step 4: After Completing Work

Run:

```bash
git status
git add .
git commit -m "Add CustomString class"
git push
```

Change the commit message according to your work.

Examples:

```bash
git commit -m "Add DynamicArray class"
git commit -m "Add User login system"
git commit -m "Add contest base class"
```

### Step 5: After Someone Pushes

All other members should run:

```bash
git pull
```

This updates their local project with the latest code.

## Team Communication Rule

Before starting work, send a group message:

```text
I am working on CustomString.h and CustomString.cpp.
Please do not edit these files.
```

After pushing:

```text
I pushed CustomString. Everyone pull latest main.
```

This prevents overwriting and confusion.

## Important Rules For Direct Main

- Always run `git pull` before coding.
- Always tell the group which files you are editing.
- Never let two people edit the same file at the same time.
- Push one by one, not all at the exact same time.
- Push only code that compiles.
- Use clear commit messages.
- After one person pushes, everyone else pulls.
- Keep one class per `.cpp` file.
- File name should match class name.
- Do not use `std::string`.
- Do not use STL containers like `vector`, `list`, `map`, `queue`, or `stack`.

## Phase-Wise Development Plan

Each phase has files. Divide those files among members. Complete the phase, push all work, test together, then move to the next phase.

## Phase 0: Setup

Goal:

Create the common GitHub repository and initial folder structure.

Files to create:

```text
include/
src/
docs/
main.cpp
README.md
Makefile
```

Team split:

```text
Person 1:
  Create GitHub repository.
  Add folder structure.

Person 2:
  Create README.md.
  Write project overview and restrictions.

Person 3:
  Create Makefile or basic build setup.
  Create empty main.cpp.
```

Completion rule:

```text
Repository exists.
Everyone can clone it.
Everyone can run git pull.
Basic project compiles.
```

## Phase 1: Foundation Classes

Goal:

Build the basic classes that replace STL and support the whole project.

Files to create:

```text
include/CustomString.h
src/CustomString.cpp

include/DynamicArray.h
src/DynamicArray.cpp

include/LinkedList.h
src/LinkedList.cpp

include/KeyValueStore.h
src/KeyValueStore.cpp

include/VFSManager.h
src/VFSManager.cpp
```

Team split:

```text
Person 1:
  CustomString.h
  CustomString.cpp

Person 2:
  DynamicArray.h
  DynamicArray.cpp

Person 3:
  LinkedList.h
  LinkedList.cpp
```

After these are complete:

```text
Person 1:
  KeyValueStore.h
  KeyValueStore.cpp

Person 2:
  VFSManager.h
  VFSManager.cpp

Person 3:
  Test foundation classes in main.cpp or separate test file.
```

Completion rule:

```text
CustomString works.
DynamicArray works.
LinkedList works.
KeyValueStore works.
VFSManager basic functions work.
No std::string used.
No STL containers used.
Code compiles.
Everyone has pulled latest main.
```

## Phase 2: User System

Goal:

Build registration, verification, login, session, and account deletion.

Files to create:

```text
include/User.h
src/User.cpp

include/UserManager.h
src/UserManager.cpp

include/Session.h
src/Session.cpp

include/VerificationService.h
src/VerificationService.cpp
```

Team split:

```text
Person 1:
  User.h
  User.cpp

Person 2:
  UserManager.h
  UserManager.cpp

Person 3:
  Session.h
  Session.cpp
  VerificationService.h
  VerificationService.cpp
```

Completion rule:

```text
User can register.
Verification token works.
Verified user can log in.
Unverified user cannot log in.
Session starts and ends.
User folder is created in VFS.
Account deletion removes user data.
Code compiles.
Everyone pulls latest main.
```

## Phase 3: Problem Bank

Goal:

Store and load coding problems from VFS.

Files to create:

```text
include/Problem.h
src/Problem.cpp

include/TestCase.h
src/TestCase.cpp

include/ProblemBank.h
src/ProblemBank.cpp
```

Team split:

```text
Person 1:
  Problem.h
  Problem.cpp

Person 2:
  TestCase.h
  TestCase.cpp

Person 3:
  ProblemBank.h
  ProblemBank.cpp
```

Completion rule:

```text
Problem stores title, difficulty, statement, constraints, and examples.
TestCase stores input and expected output.
ProblemBank can load problems.
ProblemBank can filter by difficulty.
ProblemBank can return a problem by ID.
Minimum 10 problems per difficulty are planned.
Code compiles.
Everyone pulls latest main.
```

## Phase 4: Contest System

Goal:

Build contest variants, random problem selection, timer data, save progress, and resume progress.

Files to create:

```text
include/Contest.h
src/Contest.cpp

include/EasyContest.h
src/EasyContest.cpp

include/BalancedContest.h
src/BalancedContest.cpp

include/HardContest.h
src/HardContest.cpp

include/ContestManager.h
src/ContestManager.cpp
```

Team split:

```text
Person 1:
  Contest.h
  Contest.cpp

Person 2:
  EasyContest.h
  EasyContest.cpp
  BalancedContest.h
  BalancedContest.cpp
  HardContest.h
  HardContest.cpp

Person 3:
  ContestManager.h
  ContestManager.cpp
```

Completion rule:

```text
Base Contest class exists.
Derived contest classes exist.
Polymorphism is used.
Contest can select problems.
Contest can save progress.
Contest can resume progress.
Timer uses start time and duration.
Code compiles.
Everyone pulls latest main.
```

## Phase 5: Evaluation Engine

Goal:

Compile submitted code, run it against test cases, compare output, and return verdict.

Files to create:

```text
include/Submission.h
src/Submission.cpp

include/EvaluationEngine.h
src/EvaluationEngine.cpp

include/OutputComparator.h
src/OutputComparator.cpp

include/Verdict.h
src/Verdict.cpp
```

Team split:

```text
Person 1:
  Submission.h
  Submission.cpp

Person 2:
  OutputComparator.h
  OutputComparator.cpp

Person 3:
  EvaluationEngine.h
  EvaluationEngine.cpp
  Verdict.h
  Verdict.cpp
```

Completion rule:

```text
Code can be submitted.
Compilation error is detected.
Accepted verdict works.
Wrong answer verdict works.
Runtime error is handled.
Temporary files are cleaned.
Code compiles.
Everyone pulls latest main.
```

## Phase 6: Scoring System

Goal:

Calculate score using correctness, attempts, and time.

Files to create:

```text
include/ScoreCalculator.h
src/ScoreCalculator.cpp

include/ContestResult.h
src/ContestResult.cpp
```

Team split:

```text
Person 1:
  ScoreCalculator.h
  ScoreCalculator.cpp

Person 2:
  ContestResult.h
  ContestResult.cpp

Person 3:
  Connect score with contest result flow.
```

Example formula:

```text
score = difficulty * 100 - wrongAttempts * 10 - minutesUsed
```

Completion rule:

```text
Correct solution gives score.
Wrong attempts reduce score.
Late solving reduces score.
Score never becomes negative.
Contest result stores total score.
Code compiles.
Everyone pulls latest main.
```

## Phase 7: UI System

Goal:

Build screens and connect user interaction with backend classes.

Files to create:

```text
include/UIManager.h
src/UIManager.cpp

include/LoginScreen.h
src/LoginScreen.cpp

include/RegisterScreen.h
src/RegisterScreen.cpp

include/DashboardScreen.h
src/DashboardScreen.cpp

include/ProblemBrowserScreen.h
src/ProblemBrowserScreen.cpp

include/ContestScreen.h
src/ContestScreen.cpp

include/ResultScreen.h
src/ResultScreen.cpp
```

Team split:

```text
Person 1:
  LoginScreen.h
  LoginScreen.cpp
  RegisterScreen.h
  RegisterScreen.cpp

Person 2:
  DashboardScreen.h
  DashboardScreen.cpp
  ProblemBrowserScreen.h
  ProblemBrowserScreen.cpp

Person 3:
  UIManager.h
  UIManager.cpp
  ContestScreen.h
  ContestScreen.cpp
  ResultScreen.h
  ResultScreen.cpp
```

Completion rule:

```text
Login screen works.
Register screen works.
Dashboard works.
Problem browser works.
Contest screen shows timer.
Result screen shows score.
UI connects to backend classes.
Code compiles.
Everyone pulls latest main.
```

## Phase 8: Full Integration

Goal:

Connect all modules into one working application.

Test full flow:

```text
Register user.
Verify user.
Log in.
Browse problems.
Start contest.
Write code.
Submit code.
Get verdict.
Update score.
Save progress.
Close program.
Resume contest.
End contest.
Keep or delete code.
Delete account.
```

Completion rule:

```text
Full user journey works.
No STL containers.
No std::string.
All persistent data goes through VFS.
Temporary evaluation files are cleaned.
No memory leaks.
Code compiles without warnings.
UML matches code.
```

## Direct Main Push Order For Each Phase

Use this order inside every phase:

```text
1. Team assigns files.
2. Everyone runs git pull.
3. Person 1 completes assigned files.
4. Person 1 runs git add, commit, push.
5. Everyone runs git pull.
6. Person 2 completes assigned files.
7. Person 2 runs git add, commit, push.
8. Everyone runs git pull.
9. Person 3 completes assigned files.
10. Person 3 runs git add, commit, push.
11. Everyone runs git pull.
12. Team compiles and tests the phase.
13. Move to next phase.
```

## Basic Git Commands To Remember

Before work:

```bash
git pull
```

Check changed files:

```bash
git status
```

Save changes locally:

```bash
git add .
git commit -m "Write your message here"
```

Upload to GitHub:

```bash
git push
```

Download latest code after someone else pushes:

```bash
git pull
```

## VS Code Source Control Method

If terminal commands feel hard:

```text
1. Open VS Code.
2. Click Source Control icon.
3. Before work, click Pull.
4. Edit your assigned files.
5. Write a commit message.
6. Click Commit.
7. Click Sync Changes or Push.
8. Tell group members to pull.
```

## Final Advice

This direct `main` method is simple and suitable for beginners, but it only works if the team communicates clearly.

The best rule is:

```text
One file, one owner, one push at a time.
```

Do not combine everything at the end. Combine after every phase. This keeps problems small and easy to fix.
