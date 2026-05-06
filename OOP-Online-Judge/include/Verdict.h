#ifndef OJ_VERDICT_H
#define OJ_VERDICT_H

#include "CustomString.h"

namespace OJ {

class Verdict {
public:
    enum Code {
        Accepted = 0,
        WrongAnswer = 1,
        RuntimeError = 2,
        CompileError = 3,
        InternalError = 4
    };

private:
    Code code;
    CustomString message;

public:
    Verdict() : code(InternalError), message("") {}
    Verdict(Code c, const char* msg) : code(c), message(msg) {}

    Code getCode() const { return code; }
    const CustomString& getMessage() const { return message; }

    bool isAccepted() const { return code == Accepted; }

    static const char* codeToString(Code c) {
        switch (c) {
        case Accepted: return "Accepted";
        case WrongAnswer: return "WrongAnswer";
        case RuntimeError: return "RuntimeError";
        case CompileError: return "CompileError";
        default: return "InternalError";
        }
    }
};

}

#endif
