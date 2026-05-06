#include <iostream>
#include <cstdio>
#include <ctime>

#include "include/VFSManager.h"
#include "include/UserManager.h"
#include "include/ProblemBank.h"
#include "include/ContestManager.h"
#include "include/EvaluationEngine.h"
#include "include/Submission.h"
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

static bool writeTempSource(const char* path, const char* code) {
    FILE* f = std::fopen(path, "wb");
    if (!f) return false;
    if (!code) code = "";
    int n = 0;
    while (code[n] != '\0') n++;
    std::fwrite(code, 1, (size_t)n, f);
    std::fclose(f);
    return true;
}

int main() {
    std::cout << "Phase 8: Full Integration demo\n\n";

    // 1) Initialize VFS
    VFSManager vfs;
    vfs.init("OJ_VOL");

    // 2) Problems
    OJ::ProblemBank bank;
    bank.seedIfMissing(vfs);
    bank.loadFromVFS(vfs);
    std::cout << "Problems loaded: " << bank.count() << "\n";

    // 3) Register/verify/login
    OJ::UserManager um(vfs);
    OJ::Session session;

    CustomString token;
    bool reg = um.registerUser("musa", "musa@nu.edu.pk", "pw", token);
    if (reg) {
        std::cout << "Registered user musa (token=" << token << ")\n";
        um.verifyUser("musa", token.c_str());
        std::cout << "Verified user musa\n";
    }

    bool loginOk = um.login("musa", "pw", session);
    std::cout << "Login: " << (loginOk ? "OK" : "FAIL") << "\n\n";
    if (!loginOk) return 1;

    // 4) Start contest
    OJ::ContestManager cm(vfs, bank);
    OJ::Contest* contest = cm.startContest(session, 4, 120); // balanced
    if (!contest) {
        std::cout << "startContest failed\n";
        return 1;
    }
    std::cout << "Contest started (type=" << contest->getType() << ")\n";

    // 5) Simulate close/resume
    delete contest;
    contest = NULL;

    contest = cm.resumeContest(session);
    if (!contest) {
        std::cout << "resumeContest failed\n";
        return 1;
    }
    std::cout << "Contest resumed. Problems=" << contest->problemCount() << "\n";

    if (contest->problemCount() <= 0) {
        std::cout << "No problems in contest\n";
        delete contest;
        return 1;
    }

    // Pick first problem, create a correct submission (echo input)
    int pid = contest->problemIdAt(0);
    OJ::Problem* p = bank.getById(pid);
    if (!p) {
        std::cout << "Problem not found: id=" << pid << "\n";
        delete contest;
        return 1;
    }

    const char* code =
        "#include <iostream>\n"
        "int main(){ long long x; if(!(std::cin>>x)) return 0; std::cout<<x; return 0; }\n";

    std::system("if not exist \".tmp_eval\" mkdir \".tmp_eval\"");
    if (!writeTempSource(".tmp_eval\\sub.cpp", code)) {
        std::cout << "Failed writing temp source\n";
        delete contest;
        return 1;
    }

    // 6) Evaluate
    OJ::EvaluationEngine engine(".tmp_eval");
    OJ::Submission sub(".tmp_eval\\sub.cpp");
    OJ::Verdict v = engine.evaluate(sub, *p);

    std::cout << "Submission verdict: " << OJ::Verdict::codeToString(v.getCode())
              << " | " << v.getMessage() << "\n";

    bool accepted = (v.getCode() == OJ::Verdict::Accepted);
    contest->recordAttempt(0, accepted, code);

    // Save progress after attempt
    Directory* userDir = getUserDir(vfs, "musa");
    if (userDir) {
        contest->saveProgress(vfs, userDir);
    }

    delete contest;
    contest = NULL;

    // 7) End contest (keep) -> scoring + result.txt persisted and moved to history
    std::cout << "\nEnding contest (keep=true) ...\n";
    cm.endContest(session, true);

    // 8) Load latest history result
    userDir = getUserDir(vfs, "musa");
    Directory* contestsDir = userDir ? findChildDir(userDir, "contests") : NULL;
    Directory* history = contestsDir ? findChildDir(contestsDir, "history") : NULL;

    Directory* runDir = findLatestRunDir(history);
    if (!runDir) {
        std::cout << "No history run directory found\n";
        return 0;
    }

    File* rf = findChildFile(runDir, "result.txt");
    if (!rf || !rf->getData()) {
        std::cout << "result.txt not found\n";
        return 0;
    }

    OJ::ContestResult r;
    OJ::ContestResult::deserialize(rf->getData(), r);

    std::cout << "\nContest Result (from VFS):\n";
    std::cout << "  type=" << r.getContestType() << "\n";
    std::cout << "  score=" << r.getTotalScore() << "\n";
    std::cout << "  solved=" << r.getSolvedCount() << "\n";
    std::cout << "  wrongAttempts=" << r.getWrongAttempts() << "\n";
    std::cout << "  minutesUsed=" << r.getMinutesUsed() << "\n";

    std::cout << "\nPhase 8 demo complete.\n";
    return 0;
}
