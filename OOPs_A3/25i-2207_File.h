#ifndef FILE_H
#define FILE_H

#include "25i-2207_FSE.h"
#include "25i-2207_ExtendedAttribute.h"
#include "25i-2207_VersionRecord.h"
#include <iostream>
using namespace std;

class File : public FSE {
private:
    char data[100];

    VersionRecord** versions;
    int vCount;

    ExtendedAttribute** attrs;
    int aCount;

public:
    File(const char* n, User* u, Permission p, const char* d)
        : FSE(n, u, p)
    {
        int i = 0;
        while (d[i] != '\0' && i < 99) {
            data[i] = d[i];
            i++;
        }
        data[i] = '\0';

        versions = NULL;
        vCount = 0;

        attrs = NULL;
        aCount = 0;
    }

    // SIZE
    int getSize() {
        int i = 0;
        while (data[i] != '\0') i++;
        return i;
    }

    // SHOW FILE
    void show() {
        cout << "File: " << name << " Size: " << getSize() << endl;
    }

    // READ CONTENT (needed by Online Judge layer)
    const char* getData() const {
        return data;
    }

    // ADD VERSION
    void addVersion(const char* d, int v) {
        VersionRecord** temp = new VersionRecord*[vCount + 1];

        for (int i = 0; i < vCount; i++) {
            temp[i] = versions[i];
        }

        temp[vCount] = new VersionRecord(d, v);

        delete[] versions;
        versions = temp;
        vCount++;
    }

    // ADD ATTRIBUTE
    void addAttribute(const char* k, const char* v) {
        ExtendedAttribute** temp = new ExtendedAttribute*[aCount + 1];

        for (int i = 0; i < aCount; i++) {
            temp[i] = attrs[i];
        }

        temp[aCount] = new ExtendedAttribute(k, v);

        delete[] attrs;
        attrs = temp;
        aCount++;
    }

    // SHOW HISTORY
    void showHistory() {
        cout << "\nVersions of " << name << ":\n";
        for (int i = 0; i < vCount; i++) {
            versions[i]->show();
        }
    }

    FSE* copy() {
    return new File(*this);
    }

    // SHOW ATTRIBUTES
    void showAttributes() {
        cout << "\nAttributes of " << name << ":\n";
        for (int i = 0; i < aCount; i++) {
            attrs[i]->show();
        }
    }
};

#endif
