#ifndef LINKEDLIST_H
#define LINKEDLIST_H

template <typename T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& v) : value(v), next(NULL) {}
    };

    Node* head;
    Node* tail;
    int count;

    void copyFrom(const LinkedList& other) {
        Node* cur = other.head;
        while (cur != NULL) {
            pushBack(cur->value);
            cur = cur->next;
        }
    }

public:
    LinkedList() : head(NULL), tail(NULL), count(0) {}

    LinkedList(const LinkedList& other) : head(NULL), tail(NULL), count(0) {
        copyFrom(other);
    }

    ~LinkedList() {
        clear();
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this;
        clear();
        copyFrom(other);
        return *this;
    }

    int size() const { return count; }
    bool empty() const { return count == 0; }

    void pushFront(const T& value) {
        Node* n = new Node(value);
        n->next = head;
        head = n;
        if (tail == NULL) tail = head;
        count++;
    }

    void pushBack(const T& value) {
        Node* n = new Node(value);
        if (tail == NULL) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
        count++;
    }

    bool popFront() {
        if (head == NULL) return false;
        Node* old = head;
        head = head->next;
        if (head == NULL) tail = NULL;
        delete old;
        count--;
        return true;
    }

    void clear() {
        while (head != NULL) {
            popFront();
        }
    }

    template <typename Func>
    void forEach(Func f) {
        Node* cur = head;
        while (cur != NULL) {
            f(cur->value);
            cur = cur->next;
        }
    }

    template <typename Func>
    void forEach(Func f) const {
        Node* cur = head;
        while (cur != NULL) {
            f(cur->value);
            cur = cur->next;
        }
    }
};

#endif
