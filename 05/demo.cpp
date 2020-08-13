//
// Created by suncx on 2020/3/20.
//
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {

    vector<int> c;
    c.reserve(1024);

    c.push_back(1);
    c.push_back(2);
    c.push_back(3);
    c.push_back(4);

    for (auto it = c.begin(); it != c.end(); it++) {
        if (*it == 3) c.erase(it--);
    }

    for (auto i:c) {
        cout << i;
    }

    return 0;
}