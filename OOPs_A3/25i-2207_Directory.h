#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "25i-2207_FSE.h"
#include "25i-2207_File.h"
#include <iostream>
using namespace std;

class Directory: public FSE{
private:
    FSE** arr;
    int count;

    bool isEqual(const char* a, const char* b) {
        int i = 0;
        while (a[i] !='\0' && b[i] != '\0')  {
            if (a[i] != b[i])
                return false;

            i++;
        }
        return (a[i]=='\0' && b[i]=='\0');
    }

public:
    Directory(const char* n, User* u, Permission p)
        : FSE(n, u, p)
    {
        arr =NULL;
        count = 0;
    }

    // add child
    void add(FSE* obj) {
        FSE** temp = new FSE*[count + 1];

        for (int i = 0; i < count; i++) {
            temp[i] = arr[i];
        }

        temp[count] = obj;

        delete[] arr;

        arr=temp;
        count++;
    }

    // Minimal accessors (needed by Online Judge layer)
    int childCount() const {
        return count;
    }

    FSE* childAt(int index) const {
        if (index < 0 || index >= count) return NULL;
        return arr[index];
    }

    // remove
    bool remove(const char* key) {

        for (int i = 0; i < count; i++) {
            if(isEqual(arr[i]->getName(), key)) {
                delete arr[i];
                for (int j = i; j<count-1; j++) {
                    arr[j] = arr[j+1];
                }

                count--;

                if (count == 0) {
                    delete[] arr;
                    arr = NULL;
                }

                return true;
            }
        }

        return false;
    }

    // Show
    void show() {
        cout<< "Directory: " <<name << endl;

        for (int i = 0; i < count; i++) {
            arr[i]->show();
        }
    }

    // size
    int getSize() {
        int total = 0;

        for (int i = 0; i < count; i++) {
            total += arr[i]->getSize();
        }

        return total;
    }

    // search
    FSE* search(const char* key) {

        if (isEqual(name, key))
            return this;

        for (int i = 0; i < count; i++) {

            if (isEqual(arr[i]->getName(), key))
            return arr[i];
            Directory* d=dynamic_cast<Directory*>(arr[i]);
            if(d != NULL) {
                FSE* found = d->search(key);
                if (found != NULL)
                    return found;
            }
        }

        return NULL;
    }

    // move
    void move(const char* key, Directory* dest) {

        for (int i = 0; i< count; i++) {

            if (isEqual(arr[i]->getName(), key)) {

                dest->add(arr[i]);

                for (int j = i; j < count - 1; j++) {
                    arr[j]=arr[j+1];
                }

                count--;
                return;
            }
        }
    }

    // list
    void list(int level = 0) {

        for (int i = 0; i < level; i++)
            cout << "  ";

        cout<<name<<endl;
        for (int i = 0; i < count; i++) {
            Directory* d=dynamic_cast<Directory*>(arr[i]);
            if (d != NULL)
                d->list(level + 1);
            else {
                for (int j = 0; j < level + 1; j++)
                    cout << "  ";
                cout << arr[i]->getName() << endl;
            }
            
        }
    }

    // COPY (SAFE VERSION)
    FSE* copy() {
        Directory* newDir = new Directory(name, owner, perm);

        for (int i = 0; i < count; i++) {
   newDir->add(arr[i]);  // shallow copy
        }
        return newDir;
    }

    // DESTRUCTOR
    ~Directory() {
        for (int i = 0; i < count; i++) {
            delete arr[i];
        }
        delete[] arr;
    }
};
#endif
