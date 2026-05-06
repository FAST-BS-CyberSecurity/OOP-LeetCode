#include "../include/LoginScreen.h"

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

bool LoginScreen::run(UserManager& um, Session& session) {
    std::cout << "\n--- Login ---\n";

    char uname[64];
    char pw[64];
    if (!readLine("Username: ", uname, (int)sizeof(uname))) return false;
    if (!readLine("Password: ", pw, (int)sizeof(pw))) return false;

    bool ok = um.login(uname, pw, session);
    std::cout << (ok ? "Login successful\n" : "Login failed (check verification/password)\n");
    return ok;
}

}
