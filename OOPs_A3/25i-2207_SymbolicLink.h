#ifndef SYMLINK_H
#define SYMLINK_H

#include "25i-2207_FSE.h"
#include <iostream>
using namespace std;

class SymbolicLink : public FSE {
private:
    FSE* target;

public:
    SymbolicLink(const char* n, User* u, Permission p, FSE* t)
        : FSE(n, u, p)
    {
        target = t;
    }

    int getSize() {
        return 0;
    }

    void show() {
        cout << "Link: " << name << " -> ";

        if (target == NULL)
            cout << "NULL";
        else
            cout<< target->getName();

        cout<<endl;
    }

    FSE* copy() {
        return new SymbolicLink(*this);
    }

    void breakLink() {
        target=NULL;
    }
};

#endif
