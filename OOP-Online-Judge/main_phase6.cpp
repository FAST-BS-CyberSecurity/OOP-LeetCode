#include <iostream>
#include <ctime>

#include "include/VFSManager.h"
#include "include/ProblemBank.h"
#include "include/UserManager.h"
#include "include/ContestManager.h"
#include "include/ContestResult.h"

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

static File* findChildFile(Directory* parent, const char* name) {
    if (!parent) return NULL;
    for (int i = 0; i < parent->childCount(); i++) {
        File* f = dynamic_cast<File*>(parent->childAt(i));
        if (f && sameName(f->getName(), name)) return f;
    }
    return NULL;
}

static Directory* getUserDir(VFSManager& vfs, const char* username) {
    Directory* root = vfs.getRoot();
    if (!root) return NULL;

    Directory* users = findChildDir(root, "users");
    if (!users) return NULL;

    return findChildDir(users, username);
}

static Directory* findLatestRunDir(Directory* history) {
    if (!history) return NULL;

    Directory* best = NULL;
    int bestTs = -1;

    for (int i = 0; i < history->childCount(); i++) {
        Directory* d = dynamic_cast<Directory*>(history->childAt(i));
        if (!d || !d->getName()) continue;

        const char* n = d->getName();
        // match run_<digits>
        if (!(n[0]=='r' && n[1]=='u' && n[2]=='n' && n[3]=='_')) continue;

        int ts = 0;
        int j = 4;
        bool ok = false;
        while (n[j] >= '0' && n[j] <= '9') {
            ok = true;
            ts = ts * 10 + (n[j] - '0');
            j++;
        }
        if (!ok) continue;

        if (ts > bestTs) {
            bestTs = ts;
            best = d;
        }
    }

    return best;
}

int main() {
    std::cout << "Phase 6: Scoring System demo\n\n";

    VFSManager vfs;
    vfs.init("OJ_VOL");

    // Problems
    OJ::ProblemBank bank;
    bank.seedIfMissing(vfs);
    bank.loadFromVFS(vfs);

    // User
    OJ::UserManager um(vfs);
    OJ::Session session;

    CustomString token;
    um.registerUser("musa", "musa@nu.edu.pk", "pw", token);
    um.verifyUser("musa", token.c_str());

    if (!um.login("musa", "pw", session)) {
        std::cout << "login failed\n";
        return 1;
    }

    // Contest
    OJ::ContestManager cm(vfs, bank);
    OJ::Contest* contest = cm.startContest(session, 4, 120);
    if (!contest) {
        std::cout << "startContest failed\n";
        return 1;
    }

    // Simulate: problem 0 has 2 attempts (1 wrong, 1 accepted)
    contest->recordAttempt(0, false, "// attempt 1");
    contest->recordAttempt(0, true, "// accepted");

    // Save updated progress so endContest can compute score from VFS
    Directory* userDir = getUserDir(vfs, "musa");
    if (userDir) {
        contest->saveProgress(vfs, userDir);
    }

    delete contest;
    contest = NULL;

    std::cout << "Ending contest (keep=true) and computing score...\n\n";
    cm.endContest(session, true);

    // Read the result.txt from the latest history run
    userDir = getUserDir(vfs, "musa");
    Directory* contests = findChildDir(userDir, "contests");
    Directory* history = contests ? findChildDir(contests, "history") : NULL;

    Directory* runDir = findLatestRunDir(history);
    if (!runDir) {
        std::cout << "No history run directory found\n";
        return 0;
    }

    File* rf = findChildFile(runDir, "result.txt");
    if (!rf || !rf->getData()) {
        std::cout << "result.txt not found in history run\n";
        return 0;
    }

    OJ::ContestResult r;
    OJ::ContestResult::deserialize(rf->getData(), r);

    std::cout << "Result loaded from VFS:\n";
    std::cout << "  type=" << r.getContestType() << "\n";
    std::cout << "  score=" << r.getTotalScore() << "\n";
    std::cout << "  solved=" << r.getSolvedCount() << "\n";
    std::cout << "  wrongAttempts=" << r.getWrongAttempts() << "\n";
    std::cout << "  minutesUsed=" << r.getMinutesUsed() << "\n";

    return 0;
}
