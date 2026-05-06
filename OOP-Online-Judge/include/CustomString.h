#ifndef CUSTOMSTRING_H
#define CUSTOMSTRING_H

#include <iostream>

class CustomString {
private:
    char* buffer;
    int len;

    static int strLen(const char* s) {
        if (!s) return 0;
        int i = 0;
        while (s[i] != '\0') { i++; }
        return i;
    }

    static void strCpy(char* dst, const char* src, int maxCount) {
        if (!dst || maxCount <= 0) return;
        if (!src) {
            dst[0] = '\0';
            return;
        }
        int i = 0;
        while (src[i] != '\0' && i < maxCount - 1) {
            dst[i] = src[i];
            i++;
        }
        dst[i] = '\0';
    }

    static int strCmp(const char* a, const char* b) {
        if (a == b) return 0;
        if (!a) return -1;
        if (!b) return 1;
        int i = 0;
        while (a[i] != '\0' && b[i] != '\0') {
            if (a[i] != b[i]) return (a[i] < b[i]) ? -1 : 1;
            i++;
        }
        if (a[i] == '\0' && b[i] == '\0') return 0;
        return (a[i] == '\0') ? -1 : 1;
    }

    void allocAndCopy(const char* s) {
        int n = strLen(s);
        char* newBuf = new char[n + 1];
        strCpy(newBuf, s, n + 1);
        delete[] buffer;
        buffer = newBuf;
        len = n;
    }

public:
    CustomString() : buffer(NULL), len(0) {
        buffer = new char[1];
        buffer[0] = '\0';
    }

    CustomString(const char* s) : buffer(NULL), len(0) {
        buffer = new char[1];
        buffer[0] = '\0';
        allocAndCopy(s);
    }

    CustomString(const CustomString& other) : buffer(NULL), len(0) {
        buffer = new char[1];
        buffer[0] = '\0';
        allocAndCopy(other.buffer);
    }

    ~CustomString() {
        delete[] buffer;
        buffer = NULL;
        len = 0;
    }

    CustomString& operator=(const CustomString& other) {
        if (this == &other) return *this;
        allocAndCopy(other.buffer);
        return *this;
    }

    CustomString& operator=(const char* s) {
        allocAndCopy(s);
        return *this;
    }

    int length() const { return len; }
    bool empty() const { return len == 0; }

    const char* c_str() const { return buffer; }

    int compare(const CustomString& other) const {
        return strCmp(buffer, other.buffer);
    }

    bool equals(const char* s) const {
        return strCmp(buffer, s) == 0;
    }

    bool equals(const CustomString& other) const {
        return strCmp(buffer, other.buffer) == 0;
    }

    void clear() {
        allocAndCopy("");
    }

    void append(const char* s) {
        int otherLen = strLen(s);
        int newLen = len + otherLen;

        char* newBuf = new char[newLen + 1];
        // copy current
        for (int i = 0; i < len; i++) {
            newBuf[i] = buffer[i];
        }
        // copy other
        for (int j = 0; j < otherLen; j++) {
            newBuf[len + j] = s[j];
        }
        newBuf[newLen] = '\0';

        delete[] buffer;
        buffer = newBuf;
        len = newLen;
    }

    void append(const CustomString& other) {
        append(other.buffer);
    }

    char& operator[](int index) {
        return buffer[index];
    }

    const char& operator[](int index) const {
        return buffer[index];
    }

    friend std::ostream& operator<<(std::ostream& os, const CustomString& s) {
        os << s.c_str();
        return os;
    }
};

#endif
