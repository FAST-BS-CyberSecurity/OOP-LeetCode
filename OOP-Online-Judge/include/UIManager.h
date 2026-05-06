#ifndef OJ_UIMANAGER_H
#define OJ_UIMANAGER_H

#include "VFSManager.h"
#include "UserManager.h"
#include "Session.h"
#include "ProblemBank.h"
#include "ContestManager.h"
#include "EvaluationEngine.h"

namespace OJ {

class UIManager {
private:
    VFSManager* vfs;
    UserManager* userManager;
    Session session;
    ProblemBank bank;
    ContestManager* contestManager;
    EvaluationEngine engine;

public:
    UIManager(VFSManager& vfsMgr, UserManager& um);
    ~UIManager();

    void init();
    void run();
};

}

#endif
