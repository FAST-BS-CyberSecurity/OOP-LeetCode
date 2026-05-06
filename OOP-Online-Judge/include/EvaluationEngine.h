#ifndef OJ_EVALUATIONENGINE_H
#define OJ_EVALUATIONENGINE_H

#include <cstdio>
#include <cstdlib>

#include "CustomString.h"
#include "Submission.h"
#include "Verdict.h"
#include "OutputComparator.h"
#include "Problem.h"

namespace OJ {

class EvaluationEngine {
private:
    CustomString workDir;

    static bool ensureDir(const char* path) {
        if (!path) return false;
#ifdef _WIN32
        char cmd[512];
        std::snprintf(cmd, sizeof(cmd), "if not exist \"%s\" mkdir \"%s\"", path, path);
        return std::system(cmd) == 0;
#else
        char cmd[512];
        std::snprintf(cmd, sizeof(cmd), "mkdir -p '%s'", path);
        return std::system(cmd) == 0;
#endif
    }

    static bool writeTextFile(const char* path, const char* text) {
        FILE* f = std::fopen(path, "wb");
        if (!f) return false;
        if (!text) text = "";
        int n = 0;
        while (text[n] != '\0') n++;
        std::fwrite(text, 1, (size_t)n, f);
        std::fclose(f);
        return true;
    }

    static void safeRemove(const char* path) {
        if (!path) return;
        std::remove(path);
    }

    static void joinPath(char* out, int cap, const char* a, const char* b) {
        if (!out || cap <= 0) return;
        out[0] = '\0';
        if (!a) a = "";
        if (!b) b = "";
#ifdef _WIN32
        std::snprintf(out, cap, "%s\\%s", a, b);
#else
        std::snprintf(out, cap, "%s/%s", a, b);
#endif
    }

public:
    EvaluationEngine() : workDir(".tmp_eval") {
        ensureDir(workDir.c_str());
    }

    explicit EvaluationEngine(const char* dir) : workDir(dir) {
        ensureDir(workDir.c_str());
    }

    // Compile submission source into an exe inside workDir.
    Verdict compile(const Submission& sub, const char* exeName, const char* compileLogName) const {
        if (sub.getSourcePath().empty()) {
            return Verdict(Verdict::InternalError, "Empty source path");
        }

        char exePath[256];
        char logPath[256];
        joinPath(exePath, (int)sizeof(exePath), workDir.c_str(), exeName);
        joinPath(logPath, (int)sizeof(logPath), workDir.c_str(), compileLogName);

#ifdef _WIN32
        char cmd[1024];
        std::snprintf(cmd, sizeof(cmd), "g++ -std=c++17 -O2 -Wall -Wextra -pedantic -o \"%s\" \"%s\" > \"%s\" 2>&1",
            exePath, sub.getSourcePath().c_str(), logPath);
#else
        char cmd[1024];
        std::snprintf(cmd, sizeof(cmd), "g++ -std=c++17 -O2 -Wall -Wextra -pedantic -o '%s' '%s' > '%s' 2>&1",
            exePath, sub.getSourcePath().c_str(), logPath);
#endif

        int rc = std::system(cmd);
        if (rc != 0) {
            return Verdict(Verdict::CompileError, "Compilation failed (see compile log)");
        }

        return Verdict(Verdict::Accepted, "Compiled");
    }

    Verdict runAndCompare(const char* exeName, const char* inputText, const char* expectedText, int testIndex) const {
        char inPath[256];
        char expPath[256];
        char outPath[256];

        char inName[32];
        char expName[32];
        char outName[32];
        std::snprintf(inName, sizeof(inName), "in_%d.txt", testIndex);
        std::snprintf(expName, sizeof(expName), "exp_%d.txt", testIndex);
        std::snprintf(outName, sizeof(outName), "out_%d.txt", testIndex);

        joinPath(inPath, (int)sizeof(inPath), workDir.c_str(), inName);
        joinPath(expPath, (int)sizeof(expPath), workDir.c_str(), expName);
        joinPath(outPath, (int)sizeof(outPath), workDir.c_str(), outName);

        if (!writeTextFile(inPath, inputText)) {
            return Verdict(Verdict::InternalError, "Failed to write input file");
        }
        if (!writeTextFile(expPath, expectedText)) {
            return Verdict(Verdict::InternalError, "Failed to write expected file");
        }

        char exePath[256];
        joinPath(exePath, (int)sizeof(exePath), workDir.c_str(), exeName);

#ifdef _WIN32
        char cmd[1024];
    // system() already runs via cmd.exe on Windows; prepend a cd to set working dir.
    std::snprintf(cmd, sizeof(cmd), "cd /d \"%s\" & \"%s\" < \"%s\" > \"%s\"",
              workDir.c_str(), exeName, inName, outName);
#else
        char cmd[1024];
        std::snprintf(cmd, sizeof(cmd), "'%s' < '%s' > '%s'", exePath, inPath, outPath);
#endif

        int rc = std::system(cmd);
        if (rc != 0) {
            safeRemove(inPath);
            safeRemove(expPath);
            safeRemove(outPath);
            return Verdict(Verdict::RuntimeError, "Program returned non-zero exit code");
        }

        bool ok = OutputComparator::compareFiles(expPath, outPath);

        safeRemove(inPath);
        safeRemove(expPath);
        safeRemove(outPath);

        if (!ok) {
            return Verdict(Verdict::WrongAnswer, "Output mismatch");
        }

        return Verdict(Verdict::Accepted, "OK");
    }

    Verdict evaluate(const Submission& sub, const Problem& problem) const {
        const char* exeName = "run.exe";
        const char* logName = "compile.log";

        Verdict cv = compile(sub, exeName, logName);
        if (cv.getCode() != Verdict::Accepted) {
            return cv;
        }

        for (int i = 0; i < problem.testCaseCount(); i++) {
            const TestCase& tc = problem.testCaseAt(i);
            Verdict tv = runAndCompare(exeName, tc.getInput().c_str(), tc.getExpectedOutput().c_str(), i);
            if (tv.getCode() != Verdict::Accepted) {
                // Cleanup compiled artifacts on runtime/WA as well.
                char exePath[256];
                char logPath[256];
                joinPath(exePath, (int)sizeof(exePath), workDir.c_str(), exeName);
                joinPath(logPath, (int)sizeof(logPath), workDir.c_str(), logName);
                safeRemove(exePath);
                safeRemove(logPath);
                return tv;
            }
        }

        // Cleanup compiled artifacts after success.
        char exePath[256];
        char logPath[256];
        joinPath(exePath, (int)sizeof(exePath), workDir.c_str(), exeName);
        joinPath(logPath, (int)sizeof(logPath), workDir.c_str(), logName);
        safeRemove(exePath);
        safeRemove(logPath);

        return Verdict(Verdict::Accepted, "All testcases passed");
    }
};

}

#endif
