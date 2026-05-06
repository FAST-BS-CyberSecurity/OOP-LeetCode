#include <iostream>

#include "include/VFSManager.h"
#include "include/ProblemBank.h"
#include "include/UserManager.h"
#include "include/ContestManager.h"

static void printContest(const OJ::Contest& c) {
    std::cout << "Contest type: " << c.getType() << "\n";
    std::cout << "Problems: " << c.problemCount() << "\n";
    for (int i = 0; i < c.problemCount(); i++) {
        std::cout << "  [" << i << "] id=" << c.problemIdAt(i)
                  << " attempts=" << c.getAttemptsAt(i)
                  << " solved=" << (c.isSolvedAt(i) ? "yes" : "no")
                  << "\n";
    }
    std::cout << "Remaining seconds: " << c.remainingSeconds() << "\n";
}

int main() {
    std::cout << "Phase 4: Contest System demo\n\n";

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
    bool ok = um.login("musa", "pw", session);
    std::cout << "login musa: " << (ok ? "OK" : "FAIL") << "\n\n";

    // Contest
    OJ::ContestManager cm(vfs, bank);

    // Variant selection: 1-5 (<=2 easy, <=4 balanced, 5 hard)
    OJ::Contest* contest = cm.startContest(session, 4, 120);
    if (!contest) {
        std::cout << "startContest failed\n";
        return 1;
    }

    std::cout << "Started contest + saved progress\n";
    printContest(*contest);

    // simulate work + save
    contest->recordAttempt(0, false, "// draft attempt 1");
    contest->recordAttempt(0, true, "// accepted solution");

    // Save again
    // (fetch user dir and save using manager resume/load mechanics)
    // easiest: end-to-end via saveProgress
    // NOTE: Contest::saveProgress overwrites /contests/active
    // so it acts like a real "save".
    //
    // (We don't end the contest here)
    {
        // save via the same path used by ContestManager
        // reuse: make a fresh manager resume later.
        //
        // the Contest class itself needs userDir; we just ask ContestManager to end? no.
        // We'll replicate minimal: create a new ContestManager and call end? no.
    }

    // Save using internal method by recreating a userDir pointer
    // (simple in demo)
    Directory* root = vfs.getRoot();
    Directory* users = NULL;
    for (int i = 0; i < root->childCount(); i++) {
        Directory* d = dynamic_cast<Directory*>(root->childAt(i));
        if (d && d->getName() && d->getName()[0]=='u' && d->getName()[1]=='s') { users = d; break; }
    }
    Directory* musaDir = NULL;
    if (users) {
        for (int i = 0; i < users->childCount(); i++) {
            Directory* d = dynamic_cast<Directory*>(users->childAt(i));
            if (d && d->getName() && d->getName()[0]=='m') { musaDir = d; break; }
        }
    }
    if (musaDir) contest->saveProgress(vfs, musaDir);

    delete contest;
    contest = NULL;

    std::cout << "\nSimulating resume...\n";
    OJ::Contest* resumed = cm.resumeContest(session);
    if (!resumed) {
        std::cout << "resumeContest failed\n";
        return 1;
    }
    printContest(*resumed);

    std::cout << "\nEnding contest (keep=true)\n";
    cm.endContest(session, true);

    std::cout << "\nVFS tree:\n";
    vfs.listTree();

    delete resumed;
    return 0;
}
