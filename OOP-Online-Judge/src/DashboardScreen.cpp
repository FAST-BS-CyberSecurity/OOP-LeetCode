#include "../include/DashboardScreen.h"

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

static int toInt(const char* s, int defVal = 0) {
    if (!s) return defVal;
    int i = 0;
    while (s[i] == ' ' || s[i] == '\t') i++;
    int sign = 1;
    if (s[i] == '-') { sign = -1; i++; }
    int v = 0;
    bool any = false;
    while (s[i] >= '0' && s[i] <= '9') {
        any = true;
        v = v * 10 + (s[i] - '0');
        i++;
    }
    return any ? v * sign : defVal;
}

int DashboardScreen::show(const Session& session) {
    std::cout << "\n=== Dashboard ===\n";
    if (session.getUser()) {
        std::cout << "User: " << session.getUser()->getUsername() << "\n";
    }
    std::cout << "1) Browse Problems\n";
    std::cout << "2) Start Contest\n";
    std::cout << "3) Resume Contest\n";
    std::cout << "4) View Last Result\n";
    std::cout << "5) Logout\n";
    std::cout << "6) Delete Account\n";
    std::cout << "0) Exit\n";

    char buf[32];
    if (!readLine("Choose: ", buf, (int)sizeof(buf))) return ActionExit;
    int ch = toInt(buf, -1);

    if (ch == 1) return ActionBrowseProblems;
    if (ch == 2) return ActionStartContest;
    if (ch == 3) return ActionResumeContest;
    if (ch == 4) return ActionViewLastResult;
    if (ch == 5) return ActionLogout;
    if (ch == 6) return ActionDeleteAccount;
    if (ch == 0) return ActionExit;
    return ActionExit;
}

}
