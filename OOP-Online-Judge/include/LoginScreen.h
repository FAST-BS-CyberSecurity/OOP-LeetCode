#ifndef OJ_LOGINSCREEN_H
#define OJ_LOGINSCREEN_H

#include "UserManager.h"
#include "Session.h"

namespace OJ {

class LoginScreen {
public:
    bool run(UserManager& um, Session& session);
};

}

#endif
