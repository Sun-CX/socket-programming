//
// Created by suncx on 2020/3/23.
//
#include <queue>
#include <iostream>
#include "max_heap.h"

using namespace std;

int main(int argc, char *argv[]) {

//    priority_queue<int> q;
//    q.push(1);
//    q.push(2);
//    q.push(12);
//    q.push(59);
//
//
//    auto t = q.top();
//
//    cout << t << endl;
    max_heap<int> heap(10);
    heap.insert(1);
    heap.insert(-2);
    heap.insert(8);
    heap.insert(-10);
    heap.insert(16);
    heap.insert(24);

    cout << heap;
//    int r;
//    while (!heap.isEmpty()) {
//        r = heap.pop();
//        cout << r << ' ';
//    }

    heap.remove(0);
    cout << heap;

    heap.remove(0);
    cout << heap;

    heap.remove(0);
    cout << heap;

    heap.remove(0);
    cout << heap;

    heap.remove(0);
    cout << heap;

    heap.remove(0);
    cout << heap;
    return 0;
}