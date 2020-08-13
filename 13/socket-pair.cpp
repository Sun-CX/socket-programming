#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {
    int socks[2];
    if (socketpair(PF_UNIX, SOCK_STREAM, 0, socks) < 0) {
        cerr << "socketpair error." << endl;
        return -1;
    }
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "fork error." << endl;
        return -1;
    } else if (pid == 0) {
        close(socks[0]);
        int val;
        while (true) {
            read(socks[1], &val, sizeof(val));
            ++val;
            write(socks[1], &val, sizeof(val));
        }
    } else {
        close(socks[1]);
        int val = 0;
        while (true) {
            ++val;
            cout << "send val: " << val << endl;
            write(socks[0], &val, sizeof(val));
            read(socks[0], &val, sizeof(val));
            cout << "recv val: " << val << endl;
            sleep(1);
        }
    }
    return 0;
}