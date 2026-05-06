#include "../include/ContestScreen.h"

#include <iostream>
#include <cstdio>

#include "../include/Submission.h"
#include "../include/Verdict.h"

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

static bool loadFileLimited(const char* path, int maxChars, CustomString& out) {
    out = "";
    if (!path || path[0] == '\0') return false;
    FILE* f = std::fopen(path, "rb");
    if (!f) return false;

    int total = 0;
    char buf[513];
    while (total < maxChars) {
        int want = 512;
        if (maxChars - total < want) want = maxChars - total;
        size_t n = std::fread(buf, 1, (size_t)want, f);
        if (n == 0) break;
        buf[n] = '\0';
        out.append(buf);
        total += (int)n;
        if (n < (size_t)want) break;
    }

    std::fclose(f);
    return true;
}

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

static Directory* findChildDir(Directory* parent, const char* name) {
    if (!parent) return NULL;
    for (int i = 0; i < parent->childCount(); i++) {
        Directory* d = dynamic_cast<Directory*>(parent->childAt(i));
        if (d && sameName(d->getName(), name)) return d;
    }
    return NULL;
}

static Directory* getUserDir(VFSManager& vfs, const Session& session) {
    if (!session.isActive() || !session.getUser()) return NULL;
    Directory* root = vfs.getRoot();
    if (!root) return NULL;
    Directory* users = findChildDir(root, "users");
    if (!users) return NULL;
    return findChildDir(users, session.getUser()->getUsername().c_str());
}

void ContestScreen::startNew(ContestManager& cm, ProblemBank& bank, VFSManager& vfs, const Session& session, EvaluationEngine& engine) {
    std::cout << "\n--- Start Contest ---\n";
    char buf[32];
    if (!readLine("Variant (1-5): ", buf, (int)sizeof(buf))) return;
    int variant = toInt(buf, 4);
    if (!readLine("Duration seconds (e.g., 120): ", buf, (int)sizeof(buf))) return;
    int dur = toInt(buf, 120);

    Contest* contest = cm.startContest(session, variant, dur);
    if (!contest) {
        std::cout << "Failed to start contest\n";
        return;
    }

    runLoop(cm, bank, vfs, session, engine, contest);
}

void ContestScreen::resumeExisting(ContestManager& cm, ProblemBank& bank, VFSManager& vfs, const Session& session, EvaluationEngine& engine) {
    std::cout << "\n--- Resume Contest ---\n";
    Contest* contest = cm.resumeContest(session);
    if (!contest) {
        std::cout << "No active contest found\n";
        return;
    }

    runLoop(cm, bank, vfs, session, engine, contest);
}

void ContestScreen::runLoop(ContestManager& cm, ProblemBank& bank, VFSManager& vfs, const Session& session, EvaluationEngine& engine, Contest* contest) {
    if (!contest) return;

    while (true) {
        std::cout << "\n=== Contest ===\n";
        std::cout << "Type: " << contest->getType() << "\n";
        std::cout << "Remaining seconds: " << contest->remainingSeconds() << "\n";

        for (int i = 0; i < contest->problemCount(); i++) {
            std::cout << "  [" << i << "] id=" << contest->problemIdAt(i)
                      << " attempts=" << contest->getAttemptsAt(i)
                      << " solved=" << (contest->isSolvedAt(i) ? "yes" : "no")
                      << "\n";
        }

        if (contest->hasEnded()) {
            std::cout << "Contest time ended. Ending (keep=true).\n";
            delete contest;
            cm.endContest(session, true);
            return;
        }

        std::cout << "\n1) Submit Solution\n";
        std::cout << "2) Save and Back\n";
        std::cout << "3) End Contest (Keep + Score)\n";
        std::cout << "4) End Contest (Discard)\n";

        char buf[64];
        if (!readLine("Choose: ", buf, (int)sizeof(buf))) {
            Directory* userDir = getUserDir(vfs, session);
            if (userDir) contest->saveProgress(vfs, userDir);
            delete contest;
            return;
        }
        int ch = toInt(buf, 0);

        if (ch == 2) {
            Directory* userDir = getUserDir(vfs, session);
            if (userDir) contest->saveProgress(vfs, userDir);
            delete contest;
            return;
        }

        if (ch == 3) {
            Directory* userDir = getUserDir(vfs, session);
            if (userDir) contest->saveProgress(vfs, userDir);
            delete contest;
            cm.endContest(session, true);
            return;
        }

        if (ch == 4) {
            delete contest;
            cm.endContest(session, false);
            return;
        }

        if (ch != 1) continue;

        if (!readLine("Problem index: ", buf, (int)sizeof(buf))) continue;
        int idx = toInt(buf, -1);
        if (idx < 0 || idx >= contest->problemCount()) {
            std::cout << "Invalid index\n";
            continue;
        }

        int pid = contest->problemIdAt(idx);
        Problem* p = bank.getById(pid);
        if (!p) {
            std::cout << "Problem not found in bank\n";
            continue;
        }

        std::cout << "\n[Problem " << p->getId() << "] " << p->getTitle() << "\n";
        std::cout << "Statement: " << p->getStatement() << "\n\n";

        char path[260];
        if (!readLine("Enter .cpp source path to submit: ", path, (int)sizeof(path))) continue;
        Submission sub(path);

        Verdict v = engine.evaluate(sub, *p);
        std::cout << "Verdict: " << Verdict::codeToString(v.getCode()) << " | " << v.getMessage() << "\n";

        CustomString codeText("");
        loadFileLimited(path, 16000, codeText);

        bool accepted = (v.getCode() == Verdict::Accepted);
        contest->recordAttempt(idx, accepted, codeText.c_str());

        Directory* userDir = getUserDir(vfs, session);
        if (userDir) contest->saveProgress(vfs, userDir);
    }
}

}
