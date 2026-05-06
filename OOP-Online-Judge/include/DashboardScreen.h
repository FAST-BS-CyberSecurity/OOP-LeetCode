#ifndef OJ_DASHBOARDSCREEN_H
#define OJ_DASHBOARDSCREEN_H

#include "Session.h"

namespace OJ {

class DashboardScreen {
public:
    enum Action {
        ActionBrowseProblems = 1,
        ActionStartContest = 2,
        ActionResumeContest = 3,
        ActionViewLastResult = 4,
        ActionLogout = 5,
        ActionDeleteAccount = 6,
        ActionExit = 0
    };

    int show(const Session& session);
};

}

#endif
