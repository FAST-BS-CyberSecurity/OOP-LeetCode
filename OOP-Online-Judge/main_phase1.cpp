#include <iostream>

#include "include/CustomString.h"
#include "include/DynamicArray.h"
#include "include/LinkedList.h"
#include "include/KeyValueStore.h"
#include "include/VFSManager.h"

static void testCustomString() {
    std::cout << "--- CustomString ---\n";
    CustomString a("hello");
    CustomString b("world");
    CustomString c = a;
    c.append(" ");
    c.append(b);
    std::cout << "a=" << a << " (len=" << a.length() << ")\n";
    std::cout << "b=" << b << " (len=" << b.length() << ")\n";
    std::cout << "c=" << c << " (len=" << c.length() << ")\n";
    std::cout << "c equals 'hello world'? " << (c.equals("hello world") ? "yes" : "no") << "\n";
}

static void testDynamicArray() {
    std::cout << "\n--- DynamicArray<int> ---\n";
    DynamicArray<int> arr;
    for (int i = 1; i <= 5; i++) arr.pushBack(i * 10);
    std::cout << "size=" << arr.size() << " capacity=" << arr.capacity() << "\n";
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << (i + 1 == arr.size() ? "\n" : " ");
    }
    arr.removeAt(2);
    std::cout << "after removeAt(2): ";
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << (i + 1 == arr.size() ? "\n" : " ");
    }
}

static void testLinkedList() {
    std::cout << "\n--- LinkedList<CustomString> ---\n";
    LinkedList<CustomString> list;
    list.pushBack(CustomString("A"));
    list.pushBack(CustomString("B"));
    list.pushFront(CustomString("Start"));
    std::cout << "size=" << list.size() << " values: ";
    list.forEach([](const CustomString& s) {
        std::cout << s << " ";
    });
    std::cout << "\n";
}

static void testKeyValueStore() {
    std::cout << "\n--- KeyValueStore ---\n";
    KeyValueStore kv;
    kv.put(CustomString("username"), CustomString("fatima"));
    kv.put(CustomString("email"), CustomString("fatima@nu.edu.pk"));
    kv.put(CustomString("username"), CustomString("musa"));

    CustomString out;
    if (kv.get(CustomString("username"), out)) {
        std::cout << "username=" << out << "\n";
    }
    std::cout << "size=" << kv.size() << "\n";
    kv.remove(CustomString("email"));
    std::cout << "after remove(email) size=" << kv.size() << "\n";
}

static void testVFSManager() {
    std::cout << "\n--- VFSManager (A3 wrapper) ---\n";
    VFSManager vfs;
    vfs.init("OJ_VOL");

    Directory* root = vfs.getRoot();
    Directory* users = vfs.createDirectory(root, "users");
    Directory* fatima = vfs.createDirectory(users, "fatima");
    vfs.createFile(fatima, "draft.cpp", "// in-progress solution\nint main(){}\n");

    std::cout << "Tree:\n";
    vfs.listTree();

    FSE* found = vfs.findByName("draft.cpp");
    std::cout << "findByName('draft.cpp'): " << (found ? "found" : "not found") << "\n";

    vfs.removeFrom(fatima, "draft.cpp");
    std::cout << "After delete draft.cpp:\n";
    vfs.listTree();
}

int main() {
    std::cout << "Phase 1 OK: running foundation tests.\n\n";
    testCustomString();
    testDynamicArray();
    testLinkedList();
    testKeyValueStore();
    testVFSManager();
    return 0;
}
