#ifndef OJ_HARDCONTEST_H
#define OJ_HARDCONTEST_H

#include "Contest.h"

namespace OJ {

class HardContest : public Contest {
protected:
    void buildDifficultyProfile(DynamicArray<int>& outDiffs) const {
        // 3 problems: 3,4,5
        outDiffs.clear();
        outDiffs.pushBack(3);
        outDiffs.pushBack(4);
        outDiffs.pushBack(5);
    }

public:
    HardContest(int durationSec)
        : Contest("hard", "HardContest", durationSec) {}
};

}

#endif
