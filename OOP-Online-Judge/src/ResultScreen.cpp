#include "../include/ResultScreen.h"

#include <iostream>

#include "../include/ContestResult.h"

namespace OJ {

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

static Directory* getUserDir(VFSManager& vfs, const Session& session) {
    if (!session.isActive() || !session.getUser()) return NULL;
    Directory* root = vfs.getRoot();
    if (!root) return NULL;
    Directory* users = findChildDir(root, "users");
    if (!users) return NULL;
    return findChildDir(users, session.getUser()->getUsername().c_str());
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

void ResultScreen::showLast(VFSManager& vfs, const Session& session) {
    std::cout << "\n--- Last Result ---\n";

    Directory* userDir = getUserDir(vfs, session);
    Directory* contests = userDir ? findChildDir(userDir, "contests") : NULL;
    Directory* history = contests ? findChildDir(contests, "history") : NULL;

    if (!history) {
        std::cout << "No contest history found\n";
        return;
    }

    Directory* runDir = findLatestRunDir(history);
    if (!runDir) {
        std::cout << "No run directory found\n";
        return;
    }

    File* rf = findChildFile(runDir, "result.txt");
    if (!rf || !rf->getData()) {
        std::cout << "result.txt not found\n";
        return;
    }

    ContestResult r;
    ContestResult::deserialize(rf->getData(), r);

    std::cout << "type=" << r.getContestType() << "\n";
    std::cout << "score=" << r.getTotalScore() << "\n";
    std::cout << "solved=" << r.getSolvedCount() << "\n";
    std::cout << "wrongAttempts=" << r.getWrongAttempts() << "\n";
    std::cout << "minutesUsed=" << r.getMinutesUsed() << "\n";
}

}
