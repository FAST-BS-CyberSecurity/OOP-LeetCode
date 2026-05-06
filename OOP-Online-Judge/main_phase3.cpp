#include <iostream>

#include "include/VFSManager.h"
#include "include/ProblemBank.h"

static void printProblemSummary(const OJ::Problem& p) {
    std::cout << "[id=" << p.getId() << "] d" << p.getDifficulty() << " - " << p.getTitle() << "\n";
}

int main() {
    std::cout << "Phase 3: Problem Bank demo\n\n";

    VFSManager vfs;
    vfs.init("OJ_VOL");

    OJ::ProblemBank bank;
    bank.seedIfMissing(vfs);

    bool ok = bank.loadFromVFS(vfs);
    std::cout << "loadFromVFS: " << (ok ? "OK" : "FAIL") << "\n";
    std::cout << "total problems loaded: " << bank.count() << "\n\n";

    OJ::Problem* p = bank.getById(7);
    if (p) {
        std::cout << "Problem id 7 details:\n";
        printProblemSummary(*p);
        std::cout << "statement: " << p->getStatement() << "\n";
        std::cout << "testcases: " << p->testCaseCount() << "\n\n";
    }

    DynamicArray<OJ::Problem*> d3;
    bank.filterByDifficulty(3, d3);
    std::cout << "difficulty 3 count: " << d3.size() << "\n";
    for (int i = 0; i < d3.size() && i < 5; i++) {
        printProblemSummary(*d3[i]);
    }

    std::cout << "\nVFS tree (top):\n";
    vfs.listTree();

    return 0;
}
