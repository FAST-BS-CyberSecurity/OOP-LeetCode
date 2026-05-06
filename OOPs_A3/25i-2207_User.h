#ifndef USER_H
#define USER_H

#include <iostream>
using namespace std;

class User {
private:
    int id;
    char name[20];
    bool root;

public:
    User() {
        id = 0;
        name[0]='\0';
        root = false;
    }

    User(int i, const char* n, bool r) {
        id = i;
        root = r;

        int j = 0;
        while (n[j] != '\0' && j < 19) {
            name[j] = n[j];
            j++;
        }
        name[j]='\0';
    }

    int getId() {
        return id;
    }

    const char* getName() {
        return name;
    }

    bool isRoot() {
        return root;
    }

    void show() {
        cout<<"ID: "<<id<<" Name: "<<name;
        if (root)
            cout<<" (Root)";
        cout<<endl;
    }
};

#endif