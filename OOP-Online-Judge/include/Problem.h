#ifndef OJ_PROBLEM_H
#define OJ_PROBLEM_H

#include <cstdio>

#include "CustomString.h"
#include "DynamicArray.h"
#include "TestCase.h"
#include "VFSManager.h"

namespace OJ {

class Problem {
private:
    int id;
    int difficulty; // 1-5
    CustomString title;
    CustomString statement;
    CustomString constraints;
    CustomString examples;
    DynamicArray<TestCase> testCases;

    static int toInt(const char* s) {
        if (!s) return 0;
        int i = 0;
        int sign = 1;
        if (s[i] == '-') { sign = -1; i++; }
        int val = 0;
        while (s[i] >= '0' && s[i] <= '9') {
            val = val * 10 + (s[i] - '0');
            i++;
        }
        return val * sign;
    }

    static bool startsWith(const char* s, const char* prefix) {
        if (!s || !prefix) return false;
        int i = 0;
        while (prefix[i] != '\0') {
            if (s[i] != prefix[i]) return false;
            i++;
        }
        return true;
    }

    static bool parseKeyInt(const char* meta, const char* key, int& outVal) {
        // key like "id=" or "diff="
        if (!meta || !key) return false;
        const char* p = meta;
        while (*p != '\0') {
            if (startsWith(p, key)) {
                p += 0;
                // advance to value start
                int k = 0;
                while (key[k] != '\0') k++;
                p += k;
                outVal = toInt(p);
                return true;
            }
            p++;
        }
        return false;
    }

    static bool parseKeyStr(const char* meta, const char* key, char* outBuf, int outCap) {
        if (!meta || !key || !outBuf || outCap <= 0) return false;
        const char* p = meta;
        while (*p != '\0') {
            if (startsWith(p, key)) {
                int k = 0;
                while (key[k] != '\0') k++;
                p += k;
                int i = 0;
                while (*p != '\0' && *p != ';' && i < outCap - 1) {
                    outBuf[i++] = *p++;
                }
                outBuf[i] = '\0';
                return true;
            }
            p++;
        }
        return false;
    }

public:
    Problem()
        : id(0), difficulty(1), title(""), statement(""), constraints(""), examples(""), testCases() {}

    Problem(int pid, int diff, const char* t, const char* st, const char* con, const char* ex)
        : id(pid), difficulty(diff), title(t), statement(st), constraints(con), examples(ex), testCases() {}

    int getId() const { return id; }
    int getDifficulty() const { return difficulty; }
    const CustomString& getTitle() const { return title; }
    const CustomString& getStatement() const { return statement; }

    int testCaseCount() const { return testCases.size(); }
    const TestCase& testCaseAt(int i) const { return testCases[i]; }

    void addTestCase(const TestCase& tc) { testCases.pushBack(tc); }

    // Save under: /problems/d<diff>/p<id>/
    bool saveToVFS(VFSManager& vfs, Directory* problemsRoot) const {
        if (problemsRoot == NULL) return false;

        char dName[8];
        std::snprintf(dName, sizeof(dName), "d%d", difficulty);

        // find or create difficulty dir
        Directory* dDir = NULL;
        for (int i = 0; i < problemsRoot->childCount(); i++) {
            Directory* cand = dynamic_cast<Directory*>(problemsRoot->childAt(i));
            if (cand != NULL && cand->getName() != NULL) {
                // compare names manually
                const char* cn = cand->getName();
                int j = 0;
                while (cn[j] != '\0' && dName[j] != '\0' && cn[j] == dName[j]) j++;
                if (cn[j] == '\0' && dName[j] == '\0') { dDir = cand; break; }
            }
        }
        if (dDir == NULL) dDir = vfs.createDirectory(problemsRoot, dName);
        if (dDir == NULL) return false;

        char pName[16];
        std::snprintf(pName, sizeof(pName), "p%d", id);
        Directory* pDir = vfs.createDirectory(dDir, pName);
        if (pDir == NULL) return false;

        // meta fits in 100 chars
        char meta[96];
        std::snprintf(meta, sizeof(meta), "id=%d;diff=%d;title=%s;", id, difficulty, title.c_str());
        vfs.createFile(pDir, "meta.txt", meta);
        vfs.createFile(pDir, "statement.txt", statement.c_str());
        vfs.createFile(pDir, "constraints.txt", constraints.c_str());
        vfs.createFile(pDir, "examples.txt", examples.c_str());

        Directory* tcs = vfs.createDirectory(pDir, "tcs");
        if (tcs == NULL) return true;

        for (int i = 0; i < testCases.size(); i++) {
            char tcDirName[16];
            std::snprintf(tcDirName, sizeof(tcDirName), "tc%d", i + 1);
            Directory* tcDir = vfs.createDirectory(tcs, tcDirName);
            if (tcDir == NULL) continue;
            vfs.createFile(tcDir, "in.txt", testCases[i].getInput().c_str());
            vfs.createFile(tcDir, "out.txt", testCases[i].getExpectedOutput().c_str());
        }

        return true;
    }

    static bool loadFromVFS(Directory* pDir, Problem& outProblem) {
        if (pDir == NULL) return false;

        // find files by name
        File* meta = NULL;
        File* st = NULL;
        File* con = NULL;
        File* ex = NULL;
        Directory* tcs = NULL;

        for (int i = 0; i < pDir->childCount(); i++) {
            FSE* child = pDir->childAt(i);
            if (!child) continue;
            const char* n = child->getName();
            if (!n) continue;

            File* asFile = dynamic_cast<File*>(child);
            Directory* asDir = dynamic_cast<Directory*>(child);

            // quick name compare
            if (asFile != NULL) {
                if (startsWith(n, "meta.txt") && n[8] == '\0') meta = asFile;
                else if (startsWith(n, "statement.txt") && n[13] == '\0') st = asFile;
                else if (startsWith(n, "constraints.txt") && n[15] == '\0') con = asFile;
                else if (startsWith(n, "examples.txt") && n[12] == '\0') ex = asFile;
            } else if (asDir != NULL) {
                if (startsWith(n, "tcs") && n[3] == '\0') tcs = asDir;
            }
        }

        if (meta == NULL) return false;

        int pid = 0;
        int diff = 1;
        parseKeyInt(meta->getData(), "id=", pid);
        parseKeyInt(meta->getData(), "diff=", diff);

        char titleBuf[64];
        titleBuf[0] = '\0';
        parseKeyStr(meta->getData(), "title=", titleBuf, (int)sizeof(titleBuf));

        outProblem = Problem(pid, diff,
            titleBuf,
            st ? st->getData() : "",
            con ? con->getData() : "",
            ex ? ex->getData() : "");

        if (tcs != NULL) {
            for (int i = 0; i < tcs->childCount(); i++) {
                Directory* tcDir = dynamic_cast<Directory*>(tcs->childAt(i));
                if (tcDir == NULL) continue;

                File* inF = NULL;
                File* outF = NULL;
                for (int j = 0; j < tcDir->childCount(); j++) {
                    File* f = dynamic_cast<File*>(tcDir->childAt(j));
                    if (!f) continue;
                    const char* fn = f->getName();
                    if (!fn) continue;
                    if (startsWith(fn, "in.txt") && fn[6] == '\0') inF = f;
                    else if (startsWith(fn, "out.txt") && fn[7] == '\0') outF = f;
                }

                outProblem.addTestCase(TestCase(inF ? inF->getData() : "", outF ? outF->getData() : ""));
            }
        }

        return true;
    }
};

}

#endif
