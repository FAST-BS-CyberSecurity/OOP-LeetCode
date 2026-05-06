#ifndef VFSMANAGER_H
#define VFSMANAGER_H

// Header-only on purpose (as requested).
// Thin wrapper over the Assignment-3 VFS implementation in `OOPs_A3/`.

#include <iostream>

#include "../../OOPs_A3/25i-2207_Volume.h"
#include "../../OOPs_A3/25i-2207_Directory.h"
#include "../../OOPs_A3/25i-2207_File.h"
#include "../../OOPs_A3/25i-2207_User.h"
#include "../../OOPs_A3/25i-2207_Permission.h"

class VFSManager {
private:
    Volume* volume;
    Directory* root;
    User* systemUser;
    Permission systemPerm;

public:
    VFSManager()
        : volume(NULL), root(NULL), systemUser(NULL), systemPerm(true, true, true) {}

    ~VFSManager() {
        // Volume has no destructor in A3 code, so we cleanup what we own.
        delete root;
        root = NULL;

        delete systemUser;
        systemUser = NULL;

        delete volume;
        volume = NULL;
    }

    bool init(const char* volumeName) {
        if (volume != NULL) return false;

        volume = new Volume(volumeName);
        systemUser = new User(0, "system", true);
        volume->addUser(systemUser);

        root = new Directory("/", systemUser, systemPerm);
        volume->setRoot(root);
        return true;
    }

    Directory* getRoot() const { return root; }

    Directory* createDirectory(Directory* parent, const char* name) {
        if (parent == NULL) parent = root;
        if (parent == NULL) return NULL;

        Directory* dir = new Directory(name, systemUser, systemPerm);
        parent->add(dir);
        return dir;
    }

    File* createFile(Directory* parent, const char* name, const char* data) {
        if (parent == NULL) parent = root;
        if (parent == NULL) return NULL;

        File* f = new File(name, systemUser, systemPerm, data);
        parent->add(f);
        return f;
    }

    FSE* findByName(const char* name) const {
        if (root == NULL) return NULL;
        return root->search(name);
    }

    bool removeFrom(Directory* parent, const char* name) {
        if (parent == NULL) parent = root;
        if (parent == NULL) return false;
        return parent->remove(name);
    }

    void listTree() const {
        if (root == NULL) {
            std::cout << "(VFS not initialised)\n";
            return;
        }
        root->list(0);
    }

    void showVolume() const {
        if (volume == NULL) {
            std::cout << "(VFS not initialised)\n";
            return;
        }
        volume->show();
    }
};

#endif
