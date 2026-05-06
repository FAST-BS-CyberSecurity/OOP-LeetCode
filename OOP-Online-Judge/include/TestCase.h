#ifndef OJ_TESTCASE_H
#define OJ_TESTCASE_H

#include "CustomString.h"

namespace OJ {

class TestCase {
private:
    CustomString input;
    CustomString expected;

public:
    TestCase() : input(""), expected("") {}
    TestCase(const char* in, const char* out) : input(in), expected(out) {}

    const CustomString& getInput() const { return input; }
    const CustomString& getExpectedOutput() const { return expected; }
};

}

#endif
