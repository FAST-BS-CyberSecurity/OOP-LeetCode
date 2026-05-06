#ifndef OJ_BALANCEDCONTEST_H
#define OJ_BALANCEDCONTEST_H

#include "Contest.h"

namespace OJ {

class BalancedContest : public Contest {
protected:
    void buildDifficultyProfile(DynamicArray<int>& outDiffs) const {
        // 3 problems: 1,2,3
        outDiffs.clear();
        outDiffs.pushBack(1);
        outDiffs.pushBack(2);
        outDiffs.pushBack(3);
    }

public:
    BalancedContest(int durationSec)
        : Contest("balanced", "BalancedContest", durationSec) {}
};

}

#endif
