#ifndef OJ_RESULTSCREEN_H
#define OJ_RESULTSCREEN_H

#include "VFSManager.h"
#include "Session.h"

namespace OJ {

class ResultScreen {
public:
    void showLast(VFSManager& vfs, const Session& session);
};

}

#endif
