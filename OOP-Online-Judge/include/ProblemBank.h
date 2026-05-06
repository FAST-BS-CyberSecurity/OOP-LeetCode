#ifndef OJ_PROBLEMBANK_H
#define OJ_PROBLEMBANK_H

#include <cstdio>

#include "DynamicArray.h"
#include "Problem.h"

namespace OJ {

class ProblemBank {
private:
    DynamicArray<Problem*> problems;

    static bool sameName(const char* a, const char* b) {
        if (a == b) return true;
        if (!a || !b) return false;
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (a[i] != b[i]) return false;
            i++;
        }
        return a[i] == '\0' && b[i] == '\0';
    }

    static Directory* findChildDirByName(Directory* parent, const char* name) {
        if (!parent) return NULL;
        for (int i = 0; i < parent->childCount(); i++) {
            Directory* d = dynamic_cast<Directory*>(parent->childAt(i));
            if (d && sameName(d->getName(), name)) return d;
        }
        return NULL;
    }

public:
    ProblemBank() : problems() {}

    ~ProblemBank() {
        clear();
    }

    void clear() {
        for (int i = 0; i < problems.size(); i++) {
            delete problems[i];
        }
        problems.clear();
    }

    int count() const { return problems.size(); }

    void addProblem(Problem* p) {
        if (!p) return;
        problems.pushBack(p);
    }

    Problem* getById(int id) const {
        for (int i = 0; i < problems.size(); i++) {
            if (problems[i]->getId() == id) return problems[i];
        }
        return NULL;
    }

    void filterByDifficulty(int diff, DynamicArray<Problem*>& out) const {
        out.clear();
        for (int i = 0; i < problems.size(); i++) {
            if (problems[i]->getDifficulty() == diff) out.pushBack(problems[i]);
        }
    }

    // Ensure /problems exists and seed 10 minimal problems per difficulty.
    Directory* seedIfMissing(VFSManager& vfs) {
        Directory* root = vfs.getRoot();
        if (!root) return NULL;

        Directory* problemsRoot = findChildDirByName(root, "problems");
        if (problemsRoot != NULL) return problemsRoot;

        problemsRoot = vfs.createDirectory(root, "problems");
        if (!problemsRoot) return NULL;

        int idCounter = 1;
        for (int diff = 1; diff <= 5; diff++) {
            for (int k = 1; k <= 10; k++) {
                char title[40];
                char stmt[96];
                char cons[96];
                char ex[96];

                std::snprintf(title, sizeof(title), "Dummy_%d_%d", diff, k);
                std::snprintf(stmt, sizeof(stmt), "Solve dummy problem d%d-%d (Phase3 seed).", diff, k);
                std::snprintf(cons, sizeof(cons), "Constraints: small IO for now.");
                std::snprintf(ex, sizeof(ex), "Example: input 1 -> output 1");

                Problem p(idCounter, diff, title, stmt, cons, ex);
                p.addTestCase(TestCase("1", "1"));
                p.addTestCase(TestCase("2", "2"));
                p.saveToVFS(vfs, problemsRoot);

                idCounter++;
            }
        }

        return problemsRoot;
    }

    bool loadFromVFS(VFSManager& vfs) {
        clear();
        Directory* root = vfs.getRoot();
        if (!root) return false;

        Directory* problemsRoot = findChildDirByName(root, "problems");
        if (!problemsRoot) return false;

        for (int i = 0; i < problemsRoot->childCount(); i++) {
            Directory* dDir = dynamic_cast<Directory*>(problemsRoot->childAt(i));
            if (!dDir) continue;

            for (int j = 0; j < dDir->childCount(); j++) {
                Directory* pDir = dynamic_cast<Directory*>(dDir->childAt(j));
                if (!pDir) continue;

                Problem temp;
                if (Problem::loadFromVFS(pDir, temp)) {
                    Problem* heap = new Problem(temp);
                    addProblem(heap);
                }
            }
        }

        return true;
    }
};

}

#endif
