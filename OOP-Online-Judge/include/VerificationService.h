#ifndef OJ_VERIFICATIONSERVICE_H
#define OJ_VERIFICATIONSERVICE_H

#include <cstdio>
#include "User.h"

namespace OJ {

class VerificationService {
private:
    static unsigned int checksum(const char* s) {
        if (!s) return 0;
        unsigned int sum = 0;
        for (int i = 0; s[i] != '\0'; i++) {
            sum = (sum * 131u) + (unsigned int)(unsigned char)s[i];
        }
        return sum;
    }

public:
    static CustomString generateToken(const User& u) {
        // Simple deterministic token (good enough for Phase 2 demo).
        // Format: TOK_<id>_<ck>
        unsigned int ck = checksum(u.getUsername().c_str()) ^ checksum(u.getEmail().c_str());

        char buf[64];
        std::snprintf(buf, sizeof(buf), "TOK_%d_%u", u.getId(), ck);
        return CustomString(buf);
    }
};

}

#endif
