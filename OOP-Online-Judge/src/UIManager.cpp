#include "../include/UIManager.h"

#include <iostream>

#include "../include/LoginScreen.h"
#include "../include/RegisterScreen.h"
#include "../include/DashboardScreen.h"
#include "../include/ProblemBrowserScreen.h"
#include "../include/ContestScreen.h"
#include "../include/ResultScreen.h"

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

UIManager::UIManager(VFSManager& vfsMgr, UserManager& um)
    : vfs(&vfsMgr), userManager(&um), session(), bank(), contestManager(NULL), engine(".tmp_eval") {}

UIManager::~UIManager() {
    delete contestManager;
    contestManager = NULL;
}

void UIManager::init() {
    if (!vfs) return;

    bank.seedIfMissing(*vfs);
    bank.loadFromVFS(*vfs);

    delete contestManager;
    contestManager = new ContestManager(*vfs, bank);
}

void UIManager::run() {
    init();

    while (true) {
        if (!session.isActive()) {
            std::cout << "\n=== Online Judge ===\n";
            std::cout << "1) Login\n";
            std::cout << "2) Register\n";
            std::cout << "0) Exit\n";

            char buf[32];
            if (!readLine("Choose: ", buf, (int)sizeof(buf))) break;
            int ch = toInt(buf, -1);

            if (ch == 0) break;
            if (ch == 1) {
                LoginScreen ls;
                ls.run(*userManager, session);
            } else if (ch == 2) {
                RegisterScreen rs;
                rs.run(*userManager);
            }
            continue;
        }

        DashboardScreen ds;
        int action = ds.show(session);

        if (action == DashboardScreen::ActionLogout) {
            userManager->logout(session);
            continue;
        }

        if (action == DashboardScreen::ActionDeleteAccount) {
            const char* uname = session.getUser() ? session.getUser()->getUsername().c_str() : "";
            userManager->logout(session);
            userManager->deleteUser(uname);
            std::cout << "Account deleted (if existed).\n";
            continue;
        }

        if (action == DashboardScreen::ActionBrowseProblems) {
            ProblemBrowserScreen ps;
            ps.run(bank);
            continue;
        }

        if (!contestManager) {
            std::cout << "InternalError: ContestManager not ready\n";
            continue;
        }

        if (action == DashboardScreen::ActionStartContest) {
            ContestScreen cs;
            cs.startNew(*contestManager, bank, *vfs, session, engine);
            continue;
        }

        if (action == DashboardScreen::ActionResumeContest) {
            ContestScreen cs;
            cs.resumeExisting(*contestManager, bank, *vfs, session, engine);
            continue;
        }

        if (action == DashboardScreen::ActionViewLastResult) {
            ResultScreen rs;
            rs.showLast(*vfs, session);
            continue;
        }

        if (action == DashboardScreen::ActionExit) {
            break;
        }
    }

    std::cout << "Goodbye.\n";
}

}
