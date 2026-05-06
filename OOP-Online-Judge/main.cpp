 #include "include/VFSManager.h"
 #include "include/UserManager.h"
 #include "include/UIManager.h"

int main() {
    VFSManager vfs;
    vfs.init("OJ_VOL");

    OJ::UserManager um(vfs);
    OJ::UIManager ui(vfs, um);
    ui.run();
    return 0;
}
