/* 
Name:Tasawer Sadiq
Section: CY-B
Roll Number: 25i-2207
Instructor: Sana Aurangzeb
TA: Muhammad Rayyan, Muhammad Zaeem
*/


#include <iostream>

#include "25i-2207_User.h"
#include "25i-2207_Permission.h"
#include "25i-2207_FSE.h"
#include "25i-2207_File.h"
#include "25i-2207_Directory.h"
#include "25i-2207_SymbolicLink.h"
#include "25i-2207_Partition.h"
#include "25i-2207_StorageMedium.h"
#include "25i-2207_Volume.h"

using namespace std;

int main() {

    cout << "===== FILE SYSTEM SIMULATION =====\n\n";

    // Users
    User* admin = new User(1, "Admin", true);
    User* u1 = new User(2, "Ali", false);

    Permission p(true, true, false);

    // Volume
    Volume* v = new Volume("System");

     v->addUser(admin);
     v->addUser(u1);

    //Storage
 StorageMedium* s1=new StorageMedium("SSD", 1000);
 v->addStorage(s1);

    // Partition
Partition* cDrive = new Partition("C", 500);
    s1->addPartition(cDrive);


    // Root directo
    Directory* root = new Directory("root", admin, p);

    // Files
File* f1 = new File("file1", admin, p, "HelloWorld");
   File* f2 = new File("file2", admin, p, "Data123");

    root->add(f1);
    root->add(f2);

    // Sub directory
    Directory* sub = new Directory("docs", admin, p);

    File* f3 = new File("file3", admin, p, "DocumentText");
     sub->add(f3);

     root->add(sub);

    // Symbolic Link
    SymbolicLink* link1 = new SymbolicLink("link1", admin, p, f1);
    root->add(link1);

    //Set root
v->setRoot(root);



    // Display system
    cout << "\n===== SYSTEM STRUCTURE =====\n";
    v->show();

   cout << "\n===== TREE LISTING =====\n";
    root->show();

   cout << "\n===== SEARCH TEST =====\n";

    FSE* found = root->search("file3");

    if (found)
        cout << "Found: " << found->getName() << endl;
   
        else
        cout << "Not Found\n";

    cout << "\n===== VERSION + ATTRIBUTES TEST =====\n";
    f1->addVersion("HelloWorld", 1);

f1->addVersion("HelloWorld Updated", 2);

    f1->addAttribute("author", "Ali");
    f1->addAttribute("secure", "yes");

    f1->showHistory();

    f1->showAttributes();

    cout << "\n===== MOVE TEST =====\n";
    root->move("file2", sub);

    cout << "After moving file2 to docs:\n";
    
    root->show();

    cout << "\n===== COPY TEST =====\n";
  FSE* copyFile = f1->copy();
     cout << "Copied File: " << copyFile->getName() << endl;

   cout << "\n===== CAPACITY TEST =====\n";
     cout << "Partition Free Space: " << cDrive->getFree() << endl;
    cout << "\n===== DANGING LINK TEST =====\n";
    link1->breakLink();
    root->show();

    
    // Cleanup
    delete v;

    cout << "\n===== SYSTEM SHUTDOWN  =====\n";

    return 0;
}
