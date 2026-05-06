#ifndef OJ_EASYCONTEST_H
#define OJ_EASYCONTEST_H

#include "Contest.h"

namespace OJ {

class EasyContest : public Contest {
protected:
    void buildDifficultyProfile(DynamicArray<int>& outDiffs) const {
        // 3 problems: 1,1,2
        outDiffs.clear();
        outDiffs.pushBack(1);
        outDiffs.pushBack(1);
        outDiffs.pushBack(2);
    }

public:
    EasyContest(int durationSec)
        : Contest("easy", "EasyContest", durationSec) {}
};

}

#endif
