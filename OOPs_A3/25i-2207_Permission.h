#ifndef PERMISSION_H
#define PERMISSION_H

#include <iostream>

using namespace std;

class Permission {
private:
   bool r, w, x;

public:
  
 Permission() {
        r = w = x = false;
    }

    Permission(bool re, bool we, bool xe) {
        r=re;
        w=we;
        x=xe;
    }

    void set(bool re, bool we, bool xe) {
        r = re;
        w = we;
        x = xe;
    }

    void show() {
        cout<<"R:"<<r<<" W:"<<w<<" X:"<<x<<endl;
    }
};

#endif