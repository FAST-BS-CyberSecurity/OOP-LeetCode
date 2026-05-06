#ifndef OJ_USER_H
#define OJ_USER_H

#include "CustomString.h"

namespace OJ {

class User {
private:
    int id;
    CustomString username;
    CustomString email;
    CustomString password;
    bool verified;
    CustomString verificationToken;

public:
    User()
        : id(0), username(""), email(""), password(""), verified(false), verificationToken("") {}

    User(int userId, const char* u, const char* e, const char* p)
        : id(userId), username(u), email(e), password(p), verified(false), verificationToken("") {}

    int getId() const { return id; }
    const CustomString& getUsername() const { return username; }
    const CustomString& getEmail() const { return email; }

    bool isVerified() const { return verified; }
    void setVerified(bool v) { verified = v; }

    const CustomString& getVerificationToken() const { return verificationToken; }
    void setVerificationToken(const CustomString& t) { verificationToken = t; }

    bool checkPassword(const char* p) const { return password.equals(p); }
};

}

#endif
