#ifndef OJ_CONTESTSCREEN_H
#define OJ_CONTESTSCREEN_H

#include "ContestManager.h"
#include "ProblemBank.h"
#include "Session.h"
#include "EvaluationEngine.h"
#include "VFSManager.h"

namespace OJ {

class ContestScreen {
public:
    void startNew(ContestManager& cm, ProblemBank& bank, VFSManager& vfs, const Session& session, EvaluationEngine& engine);
    void resumeExisting(ContestManager& cm, ProblemBank& bank, VFSManager& vfs, const Session& session, EvaluationEngine& engine);

private:
    void runLoop(ContestManager& cm, ProblemBank& bank, VFSManager& vfs, const Session& session, EvaluationEngine& engine, Contest* contest);
};

}

#endif
