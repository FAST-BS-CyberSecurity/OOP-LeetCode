#include "../include/ProblemBrowserScreen.h"

#include <iostream>

namespace OJ {

static bool readLine(const char* prompt, char* out, int cap) {
    if (!out || cap <= 0) return false;
    out[0] = '\0';
    if (prompt) std::cout << prompt;
    std::cin.getline(out, cap);
    if (!std::cin) {
        if (std::cin.eof()) return false;
        std::cin.clear();
        out[0] = '\0';
        return true;
    }
    return true;
}

static int toInt(const char* s, int defVal = 0) {
    if (!s) return defVal;
    int i = 0;
    while (s[i] == ' ' || s[i] == '\t') i++;
    int sign = 1;
    if (s[i] == '-') { sign = -1; i++; }
    int v = 0;
    bool any = false;
    while (s[i] >= '0' && s[i] <= '9') {
        any = true;
        v = v * 10 + (s[i] - '0');
        i++;
    }
    return any ? v * sign : defVal;
}

void ProblemBrowserScreen::run(const ProblemBank& bank) {
    std::cout << "\n--- Problem Browser ---\n";
    std::cout << "Total problems: " << bank.count() << "\n";

    char buf[32];
    if (!readLine("Filter difficulty (1-5) or 0 for all: ", buf, (int)sizeof(buf))) return;
    int diff = toInt(buf, 0);

    DynamicArray<Problem*> list;
    if (diff >= 1 && diff <= 5) {
        bank.filterByDifficulty(diff, list);
    } else {
        for (int i = 0; i < bank.count(); i++) {
            // No direct index API; list all by scanning ids 1..500
            // Simpler: use getById on known seed range
        }
        // fallback: show a few known ids
        for (int id = 1; id <= 200; id++) {
            Problem* p = bank.getById(id);
            if (p) list.pushBack(p);
        }
    }

    std::cout << "Showing " << list.size() << " problems\n";
    for (int i = 0; i < list.size(); i++) {
        std::cout << "  id=" << list[i]->getId() << " diff=" << list[i]->getDifficulty() << " title=" << list[i]->getTitle() << "\n";
    }

    if (!readLine("Enter a problem id to view statement (or 0 to back): ", buf, (int)sizeof(buf))) return;
    int pid = toInt(buf, 0);
    if (pid == 0) return;

    Problem* p = bank.getById(pid);
    if (!p) {
        std::cout << "Problem not found\n";
        return;
    }

    std::cout << "\n[Problem " << p->getId() << "] " << p->getTitle() << "\n";
    std::cout << "Difficulty: " << p->getDifficulty() << "\n";
    std::cout << "Statement: " << p->getStatement() << "\n";

    readLine("Press Enter to continue...", buf, (int)sizeof(buf));
}

}
