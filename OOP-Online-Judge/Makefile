CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

# Default target builds the integrated console application.
all: main

CORE_SRC = src/CustomString.cpp src/User.cpp src/Session.cpp src/VerificationService.cpp src/UserManager.cpp src/TestCase.cpp src/Problem.cpp src/ProblemBank.cpp src/Submission.cpp src/Verdict.cpp src/OutputComparator.cpp src/EvaluationEngine.cpp src/ScoreCalculator.cpp src/ContestResult.cpp
UI_SRC = src/UIManager.cpp src/LoginScreen.cpp src/RegisterScreen.cpp src/DashboardScreen.cpp src/ProblemBrowserScreen.cpp src/ContestScreen.cpp src/ResultScreen.cpp

main: phase7
	copy /Y phase7.exe main.exe >NUL

phase0:
	$(CXX) $(CXXFLAGS) -o phase0.exe main_phase0.cpp

phase1:
	$(CXX) $(CXXFLAGS) -o phase1.exe main_phase1.cpp src/CustomString.cpp

phase2:
	$(CXX) $(CXXFLAGS) -o phase2.exe main_phase2.cpp src/CustomString.cpp src/User.cpp src/Session.cpp src/VerificationService.cpp src/UserManager.cpp

phase3:
	$(CXX) $(CXXFLAGS) -o phase3.exe main_phase3.cpp src/CustomString.cpp src/TestCase.cpp src/Problem.cpp src/ProblemBank.cpp

phase4:
	$(CXX) $(CXXFLAGS) -o phase4.exe main_phase4.cpp src/CustomString.cpp src/User.cpp src/Session.cpp src/VerificationService.cpp src/UserManager.cpp src/TestCase.cpp src/Problem.cpp src/ProblemBank.cpp

phase5:
	$(CXX) $(CXXFLAGS) -o phase5.exe main_phase5.cpp src/CustomString.cpp src/TestCase.cpp src/Problem.cpp src/ProblemBank.cpp src/Submission.cpp src/Verdict.cpp src/OutputComparator.cpp src/EvaluationEngine.cpp

phase6:
	$(CXX) $(CXXFLAGS) -o phase6.exe main_phase6.cpp src/CustomString.cpp src/User.cpp src/Session.cpp src/VerificationService.cpp src/UserManager.cpp src/TestCase.cpp src/Problem.cpp src/ProblemBank.cpp src/ScoreCalculator.cpp src/ContestResult.cpp

phase7:
	$(CXX) $(CXXFLAGS) -o phase7.exe main_phase7.cpp $(CORE_SRC) $(UI_SRC)

phase8:
	$(CXX) $(CXXFLAGS) -o phase8.exe main_phase8.cpp $(CORE_SRC)

clean:
	del /Q *.exe 2>NUL || exit 0
