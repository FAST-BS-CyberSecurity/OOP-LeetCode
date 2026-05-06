#ifndef OJ_SUBMISSION_H
#define OJ_SUBMISSION_H

#include "CustomString.h"

namespace OJ {

class Submission {
private:
    CustomString sourcePath;

public:
    Submission() : sourcePath("") {}
    explicit Submission(const char* path) : sourcePath(path) {}

    const CustomString& getSourcePath() const { return sourcePath; }
};

}

#endif
