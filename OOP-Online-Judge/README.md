# OOP Online Judge (Spring 2026)

This project implements a small **Online Judge** platform in **C++** on top of our custom **Virtual File System (VFS)** from Assignment 3.

## Constraints (must follow)

- **No STL containers** (no `vector`, `list`, `map`, etc.).
- **No `std::string`** (all text handled with character arrays / custom wrapper).
- **No external databases** (all persistence must go through the VFS).
- Careful **memory management** (constructors/destructors, no leaks).

## Plan

We build phase-by-phase (see `../Steps.md`). Phase 0 scaffolds the folder structure and build. Phase 1 adds foundation classes (CustomString, DynamicArray, LinkedList, KeyValueStore) and a thin VFS wrapper (VFSManager).

## Build and Run

From this folder:

```powershell
cd D:\Semester2\OOP\Project\OOP-LeetCode\OOP-Online-Judge
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude -o main.exe main.cpp src/CustomString.cpp src/User.cpp src/Session.cpp src/VerificationService.cpp src/UserManager.cpp src/TestCase.cpp src/Problem.cpp src/ProblemBank.cpp src/Submission.cpp src/Verdict.cpp src/OutputComparator.cpp src/EvaluationEngine.cpp src/ScoreCalculator.cpp src/ContestResult.cpp src/UIManager.cpp src/LoginScreen.cpp src/RegisterScreen.cpp src/DashboardScreen.cpp src/ProblemBrowserScreen.cpp src/ContestScreen.cpp src/ResultScreen.cpp
.\main.exe
```

If `make` is available, use:

```powershell
make
.\main.exe
```

The older phase scripts still work, but Windows may block direct `.ps1` execution. Use this form when needed:

```powershell
powershell -ExecutionPolicy Bypass -File .\build_phase7.ps1
powershell -ExecutionPolicy Bypass -File .\build_phase8.ps1
```

Phase 7 is the interactive console app. Phase 8 is an automated full-integration demo that registers a sample user, starts/resumes a contest, evaluates one generated submission, scores it, and reads the saved result from the VFS.
