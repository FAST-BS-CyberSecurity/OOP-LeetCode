#ifndef OJ_OUTPUTCOMPARATOR_H
#define OJ_OUTPUTCOMPARATOR_H

#include <cstdio>

namespace OJ {

class OutputComparator {
private:
    static int readLine(FILE* f, char* buf, int cap) {
        if (!f || !buf || cap <= 0) return 0;
        int i = 0;
        int c;
        while ((c = std::fgetc(f)) != EOF) {
            if (c == '\r') continue; // normalize Windows newlines
            if (c == '\n') break;
            if (i < cap - 1) buf[i++] = (char)c;
        }
        buf[i] = '\0';
        if (c == EOF && i == 0) return 0;
        return 1;
    }

    static void rtrim(char* s) {
        if (!s) return;
        int n = 0;
        while (s[n] != '\0') n++;
        int i = n - 1;
        while (i >= 0) {
            char ch = s[i];
            if (ch == ' ' || ch == '\t') {
                s[i] = '\0';
                i--;
                continue;
            }
            break;
        }
    }

    static bool sameStr(const char* a, const char* b) {
        if (a == b) return true;
        if (!a || !b) return false;
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (a[i] != b[i]) return false;
            i++;
        }
        return a[i] == '\0' && b[i] == '\0';
    }

public:
    static bool compareFiles(const char* expectedPath, const char* actualPath) {
        FILE* fe = std::fopen(expectedPath, "rb");
        FILE* fa = std::fopen(actualPath, "rb");
        if (!fe || !fa) {
            if (fe) std::fclose(fe);
            if (fa) std::fclose(fa);
            return false;
        }

        char le[2048];
        char la[2048];

        while (true) {
            int he = readLine(fe, le, (int)sizeof(le));
            int ha = readLine(fa, la, (int)sizeof(la));

            if (!he && !ha) {
                std::fclose(fe);
                std::fclose(fa);
                return true;
            }

            if (he != ha) {
                std::fclose(fe);
                std::fclose(fa);
                return false;
            }

            rtrim(le);
            rtrim(la);

            if (!sameStr(le, la)) {
                std::fclose(fe);
                std::fclose(fa);
                return false;
            }
        }
    }
};

}

#endif
