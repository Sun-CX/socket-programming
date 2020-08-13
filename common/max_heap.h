#ifndef DS_MINHEAP_H
#define DS_MINHEAP_H

#include <iostream>

using namespace std;

template<typename T>
class max_heap {
private:

    T *heap;
    int sz;
    int capacity;

    void shift_up(int end);

    void shift_down(int begin, int end);

public:

    explicit max_heap(int capacity);

    ~max_heap();

    bool isEmpty();

    bool isFull();

    T pop();

    bool max(T &val);

    void insert(T v);

    void remove(int index);

    T *ptr();

    int size() const;

    friend ostream &operator<<(ostream &os, const max_heap &max_heap) {
        for (int i = 0; i < max_heap.sz; i++)
            cout << max_heap.heap[i] << ' ';
        cout << endl;
    }
};

template<class T>
bool max_heap<T>::isEmpty() {
    return sz == 0;
}

template<class T>
void max_heap<T>::shift_up(int end) {
    int i = end, parent = (i - 1) / 2;
    T temp = heap[i];
    while (i > 0) {
        if (heap[parent] >= temp) break;
        else {
            heap[i] = heap[parent];
            i = parent;
            parent = (i - 1) / 2;
        }
    }
    heap[i] = temp;
}

template<class T>
max_heap<T>::max_heap(int capacity) {
    this->capacity = capacity;
    heap = new T[capacity];
    sz = 0;
}

template<class T>
void max_heap<T>::shift_down(int begin, int end) {
    int parent = begin, child = parent * 2 + 1;
    T temp = heap[parent];
    while (child <= end) {
        if (child < end) child = heap[child] < heap[child + 1] ? child + 1 : child;//取大者的下标
        if (temp >= heap[child]) break;
        else {
            heap[parent] = heap[child];
            parent = child;
            child = 2 * parent + 1;
        }
    }
    heap[parent] = temp;
}

template<class T>
void max_heap<T>::insert(T v) {
    if (sz == capacity) {
        cerr << "heap is already full." << endl;
        return;
    }
    heap[sz] = v;
    shift_up(sz);
    sz++;
}

template<typename T>
void max_heap<T>::remove(int index) {
    if (index < 0 or index >= sz) {
        cerr << "error index";
        exit(-1);
    }
    heap[index] = heap[--sz];
    if (!isEmpty()) {
        int currentPos = (sz - 2) / 2;
        while (currentPos >= 0) {
            shift_down(currentPos, sz - 1);
            currentPos--;
        }
    }
}

template<class T>
T max_heap<T>::pop() {
    if (isEmpty()) {
        cerr << "max_heap is empty!";
        exit(-1);
    }
    T t = heap[0];
    heap[0] = heap[sz - 1];
    sz--;
    shift_down(0, sz - 1);
    return t;
}

template<typename T>
bool max_heap<T>::max(T &val) {
    if (isEmpty()) {
        return false;
    }
    val = heap[0];
    return true;
}

template<typename T>
max_heap<T>::~max_heap() {
    delete[]heap;
    capacity = sz = 0;
}

template<typename T>
bool max_heap<T>::isFull() {
    return sz == capacity;
}

template<typename T>
T *max_heap<T>::ptr() {
    return heap;
}

template<typename T>
int max_heap<T>::size() const {
    return sz;
}

#endif //DS_MINHEAP_H
