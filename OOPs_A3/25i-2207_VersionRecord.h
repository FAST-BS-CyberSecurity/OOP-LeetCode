#ifndef VERSION_H
#define VERSION_H

#include <iostream>
using namespace std;

class VersionRecord {
private:
    char data[100];
    int version;

public:
    VersionRecord() {
        data[0] = '\0';
        version = 0;
    }

    VersionRecord(const char* d, int v) {
        int i = 0;
        while (d[i] != '\0' && i < 99) {
         data[i] = d[i];
            i++;
        }
        data[i] = '\0';

        version = v;
    }

    int getVersion() {
        return version;
    }

    const char* getData() {
        return data;
    }

    void show() {
        cout<< "Version " << version<< " : " <<data << endl;
    }
};

#endif