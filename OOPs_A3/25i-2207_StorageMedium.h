#ifndef STORAGE_H
#define STORAGE_H

#include "25i-2207_Partition.h"
#include <iostream>
using namespace std;

class StorageMedium {
private:
    char type[10];   // HDD / SSD
    int capacity;

    Partition** parts;
    int count;

public:
    StorageMedium() {
        type[0] = '\0';
        capacity = 0;
        parts = NULL;
        count = 0;
    }

    StorageMedium(const char* t, int c) {
        int i = 0;
        while (t[i] != '\0' && i < 9) {
            type[i] = t[i];
            i++;
        }
        type[i] = '\0';

        capacity = c;
        parts = NULL;
        count = 0;
    }

    void addPartition(Partition* p) {
        Partition** temp = new Partition*[count + 1];

        for (int i = 0; i < count; i++) {
            temp[i] = parts[i];
        }

        temp[count] = p;

        delete[] parts;
        parts = temp;
        count++;
    }

    void show() {
        cout << "Storage: " << type
             << " Capacity: " << capacity << endl;

        for (int i = 0; i < count; i++) {
            parts[i]->show();
        }
    }

    int getFreeSpace() {
        int used = 0;

        for (int i = 0; i < count; i++) {
            used += parts[i]->getUsed();
        }

        return capacity - used;
    }
};

#endif
