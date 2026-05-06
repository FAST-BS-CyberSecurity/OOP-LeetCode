#ifndef PARTITION_H
#define PARTITION_H

#include <iostream>
using namespace std;

class Directory;   // forward declaration

class Partition {
private:
    char label[20];
    int capacity;     // total size
    int used;         // used space
    Directory* root;  // file system root

public:
    Partition() {
        label[0] ='\0';
        capacity=0;
        used=0;

        root=NULL;
    }

    Partition(const char* l, int c) {
        int i=0;
        while (l[i]!='\0' &&i< 19) {
            label[i] = l[i];
            i++;
        }
        label[i] = '\0';

        capacity = c;
        used = 0;
        root = NULL;
    }

    const char* getLabel() {
        return label;
    }

    int getCapacity() {
        return capacity;
    
    }

    int getUsed() {

    return used;
    }

    int getFree() {
        return capacity - used;
    }

    void setRoot(Directory* r) {
        root=r;
    }

    // add space usage
    bool allocate(int size) {
        if (used + size >capacity)
            return false;

        used+=size;
        return true;
    }

    void show() 
    {
        cout<< "Partition: " << label << " Used: " << used << " / " << capacity << endl;
    
    }

};

#endif