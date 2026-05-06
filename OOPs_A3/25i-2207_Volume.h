#ifndef VOLUME_H
#define VOLUME_H

#include <iostream>
#include "25i-2207_StorageMedium.h"
#include "25i-2207_Directory.h"
#include "25i-2207_User.h"

using namespace std;

class Volume {
private:
    char name[20];

 StorageMedium** storageList;
     int storageCount;

   User** users;
    int userCount;
    Directory* root;

public:

    Volume(const char* n) {
        int i=0;
        while (n[i] != '\0' && i < 19) {
            name[i] = n[i];
            i++;
        }
        name[i]='\0';

        storageList = NULL;
        storageCount = 0;
        users = NULL;
        userCount = 0;

        root=NULL;
    }

    // set root Directory
    void setRoot(Directory* r) {

        root=r;
    }

    // Add user
    void addUser(User* u) {
        User** temp=new User*[userCount + 1];

        for (int i = 0; i < userCount; i++) {

            temp[i] = users[i];
        }

        temp[userCount] = u;

        delete[] users;
        
        users = temp;
        userCount++;
    }

    // Add Storage
    void addStorage(StorageMedium* s) {
        StorageMedium** temp=new StorageMedium*[storageCount + 1];

        for (int i = 0; i<storageCount; i++) {
        temp[i]=storageList[i];
        }

        temp[storageCount]=s;

        delete[] storageList;
        storageList=temp;
        storageCount++;
    }

    // SHOW SYSTEM INFO
    void show() {
        cout << "\n==== VOLUME: " << name << " ====\n";

        cout << "\nUsers:\n";
        for (int i = 0; i < userCount; i++) {
            cout << "- " << users[i]->getName() << endl;
        }

        cout << "\nStorage Devices:\n";
        for (int i = 0; i < storageCount; i++) {
            storageList[i]->show();

        }

        cout<< "\nRoot Directory:\n";

    if (root != NULL)
            root->show();
     else
            cout << "No root set\n";
            
    }
};

#endif
