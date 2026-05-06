#ifndef MOUNTPOINT_H
#define MOUNTPOINT_H

#include "25i-2207_Directory.h"
#include <iostream>
using namespace std;

// forward declaration
class Partition;

class MountPoint :public Directory {
private:
    Partition* part;

public:
    MountPoint(const char* n, User* u, Permission p)
        : Directory(n, u, p)
    {
        part =NULL;
    }

    void mount(Partition* p) {
        part=p;
    }

    void show() {
        cout<<"MountPoint: "<<name;

        if (part==NULL)
            cout<<" (empty)";
    else
           cout<<" (mounted)";
        cout<<endl;
        // show directory contents 
        Directory::show();
    }
    
};

#endif
