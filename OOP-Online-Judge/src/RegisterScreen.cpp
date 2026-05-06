#include "../include/RegisterScreen.h"

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

bool RegisterScreen::run(UserManager& um) {
    std::cout << "\n--- Register ---\n";

    char uname[64];
    char email[64];
    char pw[64];

    if (!readLine("Username: ", uname, (int)sizeof(uname))) return false;
    if (!readLine("Email: ", email, (int)sizeof(email))) return false;
    if (!readLine("Password: ", pw, (int)sizeof(pw))) return false;

    CustomString token;
    bool ok = um.registerUser(uname, email, pw, token);
    if (!ok) {
        std::cout << "Register failed (username may exist)\n";
        return false;
    }

    std::cout << "Registered. Verification token: " << token << "\n";

    char inTok[96];
    if (!readLine("Enter token to verify now (or press Enter to skip): ", inTok, (int)sizeof(inTok))) return true;
    if (inTok[0] != '\0') {
        bool v = um.verifyUser(uname, inTok);
        std::cout << (v ? "Verified successfully\n" : "Verification failed\n");
    }

    return true;
}

}
