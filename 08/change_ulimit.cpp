//
// Created by suncx on 2020/3/28.
//

#include <iostream>
#include <sys/resource.h>

using namespace std;

int main(int argc, char *argv[]) {

    rlimit lim;

    if (getrlimit(RLIMIT_NOFILE, &lim) < 0) {
        cerr << "getrlimit error." << endl;
    } else {
        cout << "cur fds: " << lim.rlim_cur << endl;
        cout << "max fds: " << lim.rlim_max << endl;
    }


    lim.rlim_cur = lim.rlim_max = 2048;
    setrlimit(RLIMIT_NOFILE, &lim);
    getrlimit(RLIMIT_NOFILE, &lim);
    cout << "cur fds: " << lim.rlim_cur << endl;
    cout << "max fds: " << lim.rlim_max << endl;
    return 0;
}