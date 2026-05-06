#ifndef OJ_USERMANAGER_H
#define OJ_USERMANAGER_H

#include <cstdio>

#include "DynamicArray.h"
#include "User.h"
#include "Session.h"
#include "VerificationService.h"
#include "VFSManager.h"

namespace OJ {

class UserManager {
private:
    VFSManager* vfs;
    Directory* usersDir;
    DynamicArray<User*> users;
    int nextId;

    static bool sameUsername(const User* u, const char* name) {
        if (!u) return false;
        return u->getUsername().equals(name);
    }

    int findIndexByUsername(const char* username) const {
        for (int i = 0; i < users.size(); i++) {
            if (sameUsername(users[i], username)) return i;
        }
        return -1;
    }

    Directory* ensureUsersDir() {
        if (usersDir != NULL) return usersDir;
        Directory* root = vfs->getRoot();
        if (root == NULL) return NULL;

        // If a directory named "users" already exists, Directory::search returns it.
        FSE* found = root->search("users");
        Directory* asDir = dynamic_cast<Directory*>(found);
        if (asDir != NULL) {
            usersDir = asDir;
            return usersDir;
        }

        usersDir = vfs->createDirectory(root, "users");
        return usersDir;
    }

    void writeUserFiles(const User& u) {
        Directory* uDir = vfs->createDirectory(usersDir, u.getUsername().c_str());
        if (uDir == NULL) return;

        // Keep payloads under 100 chars due to A3 File::data[100].
        char profile[96];
        std::snprintf(profile, sizeof(profile), "id=%d;u=%s;v=%d;", u.getId(), u.getUsername().c_str(), u.isVerified() ? 1 : 0);

        char token[96];
        std::snprintf(token, sizeof(token), "token=%s;", u.getVerificationToken().c_str());

        vfs->createFile(uDir, "profile.txt", profile);
        vfs->createFile(uDir, "verify.txt", token);
        vfs->createFile(uDir, "progress.txt", "");
    }

    void rewriteUserFiles(const User& u) {
        // A3 File has no setter, so easiest Phase-2 approach is delete+recreate.
        if (!usersDir) return;
        FSE* found = usersDir->search(u.getUsername().c_str());
        Directory* uDir = dynamic_cast<Directory*>(found);
        if (uDir == NULL) return;

        uDir->remove("profile.txt");
        uDir->remove("verify.txt");

        char profile[96];
        std::snprintf(profile, sizeof(profile), "id=%d;u=%s;v=%d;", u.getId(), u.getUsername().c_str(), u.isVerified() ? 1 : 0);

        char token[96];
        std::snprintf(token, sizeof(token), "token=%s;", u.getVerificationToken().c_str());

        vfs->createFile(uDir, "profile.txt", profile);
        vfs->createFile(uDir, "verify.txt", token);
    }

public:
    UserManager(VFSManager& manager)
        : vfs(&manager), usersDir(NULL), users(), nextId(1) {
        usersDir = ensureUsersDir();
    }

    ~UserManager() {
        for (int i = 0; i < users.size(); i++) {
            delete users[i];
        }
    }

    bool registerUser(const char* username, const char* email, const char* password, CustomString& outToken) {
        if (!vfs || !username || !email || !password) return false;
        if (usersDir == NULL) usersDir = ensureUsersDir();
        if (usersDir == NULL) return false;

        if (findIndexByUsername(username) != -1) return false;

        User* u = new User(nextId, username, email, password);
        nextId++;

        CustomString token = VerificationService::generateToken(*u);
        u->setVerificationToken(token);
        u->setVerified(false);

        users.pushBack(u);
        outToken = token;

        writeUserFiles(*u);
        return true;
    }

    bool verifyUser(const char* username, const char* token) {
        int idx = findIndexByUsername(username);
        if (idx == -1) return false;

        User* u = users[idx];
        if (u->isVerified()) return true;
        if (!u->getVerificationToken().equals(token)) return false;

        u->setVerified(true);
        rewriteUserFiles(*u);
        return true;
    }

    bool login(const char* username, const char* password, Session& session) {
        int idx = findIndexByUsername(username);
        if (idx == -1) return false;

        User* u = users[idx];
        if (!u->isVerified()) return false;
        if (!u->checkPassword(password)) return false;

        session.start(u);
        return true;
    }

    void logout(Session& session) {
        session.end();
    }

    bool deleteUser(const char* username) {
        int idx = findIndexByUsername(username);
        if (idx == -1) return false;

        // Delete VFS directory recursively
        if (usersDir == NULL) usersDir = ensureUsersDir();
        if (usersDir != NULL) {
            usersDir->remove(username);
        }

        delete users[idx];
        users.removeAt(idx);
        return true;
    }

    int userCount() const { return users.size(); }
};

}

#endif
