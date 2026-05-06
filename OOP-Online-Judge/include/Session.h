#ifndef OJ_SESSION_H
#define OJ_SESSION_H

#include "User.h"

namespace OJ {

class Session {
private:
    User* current;
    bool active;

public:
    Session() : current(NULL), active(false) {}

    void start(User* u) {
        current = u;
        active = (u != NULL);
    }

    void end() {
        current = NULL;
        active = false;
    }

    bool isActive() const { return active; }
    User* getUser() const { return current; }
};

}

#endif
