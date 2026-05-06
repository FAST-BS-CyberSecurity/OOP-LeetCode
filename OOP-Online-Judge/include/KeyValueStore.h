#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include "CustomString.h"

class KeyValueStore {
private:
    struct Entry {
        CustomString key;
        CustomString value;
        Entry(const CustomString& k, const CustomString& v) : key(k), value(v) {}
    };

    struct Node {
        Entry entry;
        Node* next;
        Node(const Entry& e) : entry(e), next(NULL) {}
    };

    Node* head;
    int count;

    static bool keysEqual(const CustomString& a, const CustomString& b) {
        return a.equals(b);
    }

    void clearNodes() {
        Node* cur = head;
        while (cur != NULL) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = NULL;
        count = 0;
    }

    void copyFrom(const KeyValueStore& other) {
        // Keep insertion order
        Node* cur = other.head;
        while (cur != NULL) {
            put(cur->entry.key, cur->entry.value);
            cur = cur->next;
        }
    }

public:
    KeyValueStore() : head(NULL), count(0) {}

    KeyValueStore(const KeyValueStore& other) : head(NULL), count(0) {
        copyFrom(other);
    }

    ~KeyValueStore() {
        clearNodes();
    }

    KeyValueStore& operator=(const KeyValueStore& other) {
        if (this == &other) return *this;
        clearNodes();
        copyFrom(other);
        return *this;
    }

    int size() const { return count; }
    bool empty() const { return count == 0; }

    void put(const CustomString& key, const CustomString& value) {
        Node* cur = head;
        while (cur != NULL) {
            if (keysEqual(cur->entry.key, key)) {
                cur->entry.value = value;
                return;
            }
            cur = cur->next;
        }

        Node* n = new Node(Entry(key, value));
        n->next = head;
        head = n;
        count++;
    }

    bool get(const CustomString& key, CustomString& outValue) const {
        Node* cur = head;
        while (cur != NULL) {
            if (keysEqual(cur->entry.key, key)) {
                outValue = cur->entry.value;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    bool contains(const CustomString& key) const {
        CustomString dummy;
        return get(key, dummy);
    }

    bool remove(const CustomString& key) {
        Node* cur = head;
        Node* prev = NULL;
        while (cur != NULL) {
            if (keysEqual(cur->entry.key, key)) {
                if (prev == NULL) head = cur->next;
                else prev->next = cur->next;
                delete cur;
                count--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }
};

#endif
