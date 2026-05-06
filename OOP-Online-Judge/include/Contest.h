#ifndef OJ_CONTEST_H
#define OJ_CONTEST_H

#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "CustomString.h"
#include "DynamicArray.h"
#include "ProblemBank.h"
#include "VFSManager.h"

namespace OJ {

class Contest {
protected:
    CustomString contestType;  // "easy" | "balanced" | "hard"
    CustomString contestName;

    long long startEpoch;
    int durationSeconds;

    DynamicArray<int> problemIds;
    DynamicArray<int> attempts;
    DynamicArray<int> solved;
    DynamicArray<CustomString> drafts; // small in-memory draft; persisted via chunks

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

    static long long nowEpoch() {
        return (long long)std::time(NULL);
    }

    static bool parseIntAfterKey(const char* s, const char* key, int& out) {
        if (!s || !key) return false;
        for (int i = 0; s[i] != '\0'; i++) {
            int k = 0;
            while (key[k] != '\0' && s[i + k] != '\0' && s[i + k] == key[k]) k++;
            if (key[k] == '\0') {
                // parse integer at s[i+k]
                const char* p = s + i + k;
                int sign = 1;
                if (*p == '-') { sign = -1; p++; }
                int val = 0;
                while (*p >= '0' && *p <= '9') {
                    val = val * 10 + (*p - '0');
                    p++;
                }
                out = val * sign;
                return true;
            }
        }
        return false;
    }

    static bool parseStrAfterKey(const char* s, const char* key, char* outBuf, int outCap) {
        if (!s || !key || !outBuf || outCap <= 0) return false;
        for (int i = 0; s[i] != '\0'; i++) {
            int k = 0;
            while (key[k] != '\0' && s[i + k] != '\0' && s[i + k] == key[k]) k++;
            if (key[k] == '\0') {
                const char* p = s + i + k;
                int j = 0;
                while (*p != '\0' && *p != ';' && j < outCap - 1) {
                    outBuf[j++] = *p++;
                }
                outBuf[j] = '\0';
                return true;
            }
        }
        return false;
    }

    static void ensureSeededRand() {
        static bool seeded = false;
        if (!seeded) {
            std::srand((unsigned int)std::time(NULL));
            seeded = true;
        }
    }

    static int randIndex(int n) {
        if (n <= 0) return 0;
        return std::rand() % n;
    }

    static void writeIntFile(VFSManager& vfs, Directory* dir, const char* fileName, int value) {
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%d", value);
        // delete if exists then recreate (A3 File has no setter)
        dir->remove(fileName);
        vfs.createFile(dir, fileName, buf);
    }

    static int readIntFile(Directory* dir, const char* fileName, int def = 0) {
        File* f = findChildFile(dir, fileName);
        if (!f) return def;
        // parse int
        const char* s = f->getData();
        if (!s) return def;
        int sign = 1;
        int i = 0;
        if (s[i] == '-') { sign = -1; i++; }
        int val = 0;
        while (s[i] >= '0' && s[i] <= '9') {
            val = val * 10 + (s[i] - '0');
            i++;
        }
        return val * sign;
    }

    static void writeTextChunked(VFSManager& vfs, Directory* dir, const char* baseName, const char* text) {
        // Store in 90-char chunks due to File::data[100]
        if (!dir || !baseName) return;
        if (!text) text = "";

        // remove old chunks
        for (int i = 0; i < dir->childCount(); ) {
            File* f = dynamic_cast<File*>(dir->childAt(i));
            if (!f) { i++; continue; }
            const char* n = f->getName();
            if (!n) { i++; continue; }

            // match prefix baseName
            int k = 0;
            while (baseName[k] != '\0' && n[k] != '\0' && baseName[k] == n[k]) k++;
            if (baseName[k] == '\0') {
                // remove will shift array
                dir->remove(n);
                continue;
            }
            i++;
        }

        int len = 0;
        while (text[len] != '\0') len++;

        int part = 0;
        int pos = 0;
        while (pos < len) {
            char chunk[96];
            int j = 0;
            while (pos < len && j < 90) {
                chunk[j++] = text[pos++];
            }
            chunk[j] = '\0';

            char fname[32];
            std::snprintf(fname, sizeof(fname), "%s_%d.txt", baseName, part);
            vfs.createFile(dir, fname, chunk);
            part++;
        }

        if (len == 0) {
            char fname[32];
            std::snprintf(fname, sizeof(fname), "%s_%d.txt", baseName, 0);
            vfs.createFile(dir, fname, "");
        }
    }

    static CustomString readTextChunked(Directory* dir, const char* baseName) {
        CustomString out("");
        if (!dir || !baseName) return out;

        // read parts in order baseName_0.txt, baseName_1.txt, ... until missing
        for (int part = 0; part < 200; part++) {
            char fname[32];
            std::snprintf(fname, sizeof(fname), "%s_%d.txt", baseName, part);
            File* f = findChildFile(dir, fname);
            if (!f) break;
            out.append(f->getData());
        }
        return out;
    }

    virtual void buildDifficultyProfile(DynamicArray<int>& outDiffs) const = 0;

    void selectProblems(ProblemBank& bank, const DynamicArray<int>& diffs) {
        problemIds.clear();
        attempts.clear();
        solved.clear();
        drafts.clear();

        ensureSeededRand();

        for (int i = 0; i < diffs.size(); i++) {
            int diff = diffs[i];
            DynamicArray<Problem*> pool;
            bank.filterByDifficulty(diff, pool);

            if (pool.size() == 0) continue;

            // pick unique by id
            int chosenId = -1;
            for (int tries = 0; tries < 50; tries++) {
                int idx = randIndex(pool.size());
                int candidate = pool[idx]->getId();

                bool already = false;
                for (int k = 0; k < problemIds.size(); k++) {
                    if (problemIds[k] == candidate) { already = true; break; }
                }

                if (!already) { chosenId = candidate; break; }
            }

            if (chosenId == -1) {
                chosenId = pool[0]->getId();
            }

            problemIds.pushBack(chosenId);
            attempts.pushBack(0);
            solved.pushBack(0);
            drafts.pushBack(CustomString(""));
        }
    }

public:
    Contest(const char* typeName, const char* name, int durationSec)
        : contestType(typeName), contestName(name), startEpoch(0), durationSeconds(durationSec),
          problemIds(), attempts(), solved(), drafts() {}

    virtual ~Contest() {}

    const CustomString& getType() const { return contestType; }
    const CustomString& getName() const { return contestName; }

    void start(ProblemBank& bank) {
        DynamicArray<int> diffs;
        buildDifficultyProfile(diffs);
        selectProblems(bank, diffs);
        startEpoch = nowEpoch();
    }

    int problemCount() const { return problemIds.size(); }
    int problemIdAt(int index) const { return problemIds[index]; }

    int getAttemptsAt(int index) const { return attempts[index]; }
    bool isSolvedAt(int index) const { return solved[index] != 0; }

    int remainingSeconds() const {
        if (startEpoch == 0) return durationSeconds;
        long long elapsed = nowEpoch() - startEpoch;
        int rem = durationSeconds - (int)elapsed;
        return (rem < 0) ? 0 : rem;
    }

    bool hasEnded() const {
        return remainingSeconds() <= 0;
    }

    void recordAttempt(int index, bool accepted, const char* draftCode) {
        if (index < 0 || index >= attempts.size()) return;
        if (hasEnded()) return;

        attempts[index] = attempts[index] + 1;
        solved[index] = accepted ? 1 : solved[index];
        drafts[index] = draftCode;
    }

    // Persistence under: userDir/contests/active/
    bool saveProgress(VFSManager& vfs, Directory* userDir) const {
        if (!userDir) return false;

        Directory* contests = findChildDir(userDir, "contests");
        if (!contests) contests = vfs.createDirectory(userDir, "contests");
        if (!contests) return false;

        // recreate active
        contests->remove("active");
        Directory* active = vfs.createDirectory(contests, "active");
        if (!active) return false;

        // meta
        char meta[96];
        std::snprintf(meta, sizeof(meta), "type=%s;dur=%d;start=%d;", contestType.c_str(), durationSeconds, (int)startEpoch);
        vfs.createFile(active, "meta.txt", meta);

        writeIntFile(vfs, active, "count.txt", problemIds.size());

        // problems / attempts / solved
        for (int i = 0; i < problemIds.size(); i++) {
            char f1[16];
            char f2[16];
            char f3[16];
            std::snprintf(f1, sizeof(f1), "p%d.txt", i);
            std::snprintf(f2, sizeof(f2), "a%d.txt", i);
            std::snprintf(f3, sizeof(f3), "s%d.txt", i);
            vfs.createFile(active, f1, "");
            writeIntFile(vfs, active, f1, problemIds[i]);
            vfs.createFile(active, f2, "");
            writeIntFile(vfs, active, f2, attempts[i]);
            vfs.createFile(active, f3, "");
            writeIntFile(vfs, active, f3, solved[i]);
        }

        Directory* draftsDir = vfs.createDirectory(active, "drafts");
        if (draftsDir) {
            for (int i = 0; i < problemIds.size(); i++) {
                char base[24];
                std::snprintf(base, sizeof(base), "p%d", problemIds[i]);
                writeTextChunked(vfs, draftsDir, base, drafts[i].c_str());
            }
        }

        return true;
    }

    bool loadProgress(VFSManager& vfs, Directory* userDir) {
        (void)vfs;
        if (!userDir) return false;

        Directory* contests = findChildDir(userDir, "contests");
        if (!contests) return false;
        Directory* active = findChildDir(contests, "active");
        if (!active) return false;

        // count
        int count = readIntFile(active, "count.txt", 0);
        if (count <= 0) return false;

        // meta start/dur are optional for demo; keep defaults if missing
        File* meta = findChildFile(active, "meta.txt");
        if (meta) {
            const char* m = meta->getData();
            if (m) {
                int dur = durationSeconds;
                int start = (int)startEpoch;
                parseIntAfterKey(m, "dur=", dur);
                parseIntAfterKey(m, "start=", start);
                durationSeconds = dur;
                startEpoch = (long long)start;

                char typeBuf[24];
                typeBuf[0] = '\0';
                if (parseStrAfterKey(m, "type=", typeBuf, (int)sizeof(typeBuf))) {
                    contestType = typeBuf;
                }
            }
        }

        problemIds.clear();
        attempts.clear();
        solved.clear();
        drafts.clear();

        for (int i = 0; i < count; i++) {
            char f1[16];
            char f2[16];
            char f3[16];
            std::snprintf(f1, sizeof(f1), "p%d.txt", i);
            std::snprintf(f2, sizeof(f2), "a%d.txt", i);
            std::snprintf(f3, sizeof(f3), "s%d.txt", i);

            int pid = readIntFile(active, f1, -1);
            int att = readIntFile(active, f2, 0);
            int sol = readIntFile(active, f3, 0);

            if (pid == -1) continue;
            problemIds.pushBack(pid);
            attempts.pushBack(att);
            solved.pushBack(sol);
            drafts.pushBack(CustomString(""));
        }

        Directory* draftsDir = findChildDir(active, "drafts");
        if (draftsDir) {
            for (int i = 0; i < problemIds.size(); i++) {
                char base[24];
                std::snprintf(base, sizeof(base), "p%d", problemIds[i]);
                drafts[i] = readTextChunked(draftsDir, base);
            }
        }

        return true;
    }

private:
    static int readIntFrom(const char* s) {
        if (!s) return 0;
        int sign = 1;
        int i = 0;
        if (s[i] == '-') { sign = -1; i++; }
        int val = 0;
        while (s[i] >= '0' && s[i] <= '9') {
            val = val * 10 + (s[i] - '0');
            i++;
        }
        return val * sign;
    }
};

}

#endif
