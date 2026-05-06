#ifndef EXTATTR_H
#define EXTATTR_H

#include <iostream>
using namespace std;

class ExtendedAttribute {
private:
    char key[20];
    char value[50];

public:
    ExtendedAttribute() {
        key[0]='\0';
        value[0]='\0';
    }

    ExtendedAttribute(const char* k, const char* v) {
        int i=0;
        while (k[i] !='\0' && i<19) {
            key[i]=k[i];
            i++;
        }
        key[i]='\0';

        int j=0;
        while (v[j]!='\0' && j<49) {
            value[j]=v[j];
            j++;
        }
        
        value[j]='\0';
    }

    const char* getKey() {
        return key;
    }

    const char* getValue() {
        return value;
    }

    void show() {
        cout <<key << " : "<<value<<endl;
    }
};

#endif