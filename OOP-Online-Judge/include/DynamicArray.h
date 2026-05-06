#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

template <typename T>
class DynamicArray {
private:
    T* data;
    int count;
    int cap;

    void growIfNeeded(int minCapacity) {
        if (cap >= minCapacity) return;

        int newCap = (cap == 0) ? 4 : cap;
        while (newCap < minCapacity) newCap *= 2;

        T* newData = new T[newCap];
        for (int i = 0; i < count; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        cap = newCap;
    }

public:
    DynamicArray() : data(NULL), count(0), cap(0) {}

    DynamicArray(const DynamicArray& other) : data(NULL), count(0), cap(0) {
        growIfNeeded(other.count);
        count = other.count;
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }
    }

    ~DynamicArray() {
        delete[] data;
        data = NULL;
        count = 0;
        cap = 0;
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;
        delete[] data;
        data = NULL;
        count = 0;
        cap = 0;

        growIfNeeded(other.count);
        count = other.count;
        for (int i = 0; i < count; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }

    int size() const { return count; }
    int capacity() const { return cap; }
    bool empty() const { return count == 0; }

    void pushBack(const T& value) {
        growIfNeeded(count + 1);
        data[count] = value;
        count++;
    }

    bool popBack() {
        if (count == 0) return false;
        count--;
        return true;
    }

    bool removeAt(int index) {
        if (index < 0 || index >= count) return false;
        for (int i = index; i < count - 1; i++) {
            data[i] = data[i + 1];
        }
        count--;
        return true;
    }

    void clear() {
        count = 0;
    }

    T& operator[](int index) { return data[index]; }
    const T& operator[](int index) const { return data[index]; }
};

#endif
