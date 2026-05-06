#ifndef OJ_CONTESTRESULT_H
#define OJ_CONTESTRESULT_H

#include <cstdio>

#include "CustomString.h"

namespace OJ {

class ContestResult {
private:
    CustomString contestType;
    int totalScore;
    int solvedCount;
    int wrongAttempts;
    int minutesUsed;
    long long endEpoch;

    static int toInt(const char* s) {
        if (!s) return 0;
        int i = 0;
        int sign = 1;
        if (s[i] == '-') { sign = -1; i++; }
        int v = 0;
        while (s[i] >= '0' && s[i] <= '9') {
            v = v * 10 + (s[i] - '0');
            i++;
        }
        return v * sign;
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
        if (!meta || !key) return false;
        const char* p = meta;
        while (*p != '\0') {
            if (startsWith(p, key)) {
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

    static bool parseKeyLongLong(const char* meta, const char* key, long long& outVal) {
        if (!meta || !key) return false;
        const char* p = meta;
        while (*p != '\0') {
            if (startsWith(p, key)) {
                int k = 0;
                while (key[k] != '\0') k++;
                p += k;
                // parse long long
                int sign = 1;
                if (*p == '-') { sign = -1; p++; }
                long long v = 0;
                while (*p >= '0' && *p <= '9') {
                    v = v * 10 + (*p - '0');
                    p++;
                }
                outVal = v * sign;
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
    ContestResult()
        : contestType(""), totalScore(0), solvedCount(0), wrongAttempts(0), minutesUsed(0), endEpoch(0) {}

    ContestResult(const char* type, int score, int solved, int wrong, int minutes, long long endTs)
        : contestType(type), totalScore(score), solvedCount(solved), wrongAttempts(wrong), minutesUsed(minutes), endEpoch(endTs) {}

    const CustomString& getContestType() const { return contestType; }
    int getTotalScore() const { return totalScore; }
    int getSolvedCount() const { return solvedCount; }
    int getWrongAttempts() const { return wrongAttempts; }
    int getMinutesUsed() const { return minutesUsed; }
    long long getEndEpoch() const { return endEpoch; }

    void serialize(char* outBuf, int outCap) const {
        if (!outBuf || outCap <= 0) return;
        std::snprintf(outBuf, outCap,
                      "type=%s;score=%d;solved=%d;wrong=%d;minutes=%d;end=%d;",
                      contestType.c_str(), totalScore, solvedCount, wrongAttempts, minutesUsed, (int)endEpoch);
    }

    static bool deserialize(const char* meta, ContestResult& out) {
        if (!meta) return false;
        char typeBuf[24];
        typeBuf[0] = '\0';
        if (parseKeyStr(meta, "type=", typeBuf, (int)sizeof(typeBuf))) {
            out.contestType = typeBuf;
        }
        parseKeyInt(meta, "score=", out.totalScore);
        parseKeyInt(meta, "solved=", out.solvedCount);
        parseKeyInt(meta, "wrong=", out.wrongAttempts);
        parseKeyInt(meta, "minutes=", out.minutesUsed);
        parseKeyLongLong(meta, "end=", out.endEpoch);
        return true;
    }
};

}

#endif
