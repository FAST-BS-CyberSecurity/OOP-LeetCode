#include <iostream>

#include "include/VFSManager.h"
#include "include/UserManager.h"

int main() {
    std::cout << "Phase 2: User System demo\n\n";

    VFSManager vfs;
    vfs.init("OJ_VOL");

    OJ::UserManager um(vfs);
    OJ::Session session;

    CustomString token;
    bool ok = um.registerUser("fatima", "fatima@nu.edu.pk", "pass123", token);
    std::cout << "register fatima: " << (ok ? "OK" : "FAIL") << "\n";
    std::cout << "token: " << token << "\n";

    ok = um.login("fatima", "pass123", session);
    std::cout << "login before verify: " << (ok ? "OK" : "FAIL") << "\n";

    ok = um.verifyUser("fatima", token.c_str());
    std::cout << "verify fatima: " << (ok ? "OK" : "FAIL") << "\n";

    ok = um.login("fatima", "pass123", session);
    std::cout << "login after verify: " << (ok ? "OK" : "FAIL") << "\n";
    std::cout << "session active: " << (session.isActive() ? "yes" : "no") << "\n";

    std::cout << "\nVFS tree after registration:\n";
    vfs.listTree();

    ok = um.deleteUser("fatima");
    std::cout << "\ndelete fatima: " << (ok ? "OK" : "FAIL") << "\n";

    std::cout << "\nVFS tree after deletion:\n";
    vfs.listTree();

    return 0;
}
