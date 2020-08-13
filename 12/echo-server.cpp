#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[]) {

    int listen_fd;
    if ((listen_fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
        cerr << "socket error." << endl;
        return -1;
    }

    unlink("unix-socket");

    //不需要设置地址重复利用

    sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    //server_addr.sun_path = "unix-test";
    //路径最好用绝对路径
    strcpy(server_addr.sun_path, "unix-socket");

    if (bind(listen_fd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "bind error." << endl;
        return -1;
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {
        cerr << "listen error." << endl;
        return -1;
    }

    sockaddr_un peer_addr;
    socklen_t len = sizeof(peer_addr);
    int con_fd;
    if ((con_fd = accept(listen_fd, (sockaddr *) &peer_addr, &len)) < 0) {
        cerr << "accept error." << endl;
        return -1;
    }
    char recv_buf[1024];
    ssize_t r = read(con_fd, recv_buf, sizeof(recv_buf));
    if (r < 0) {
        cerr << "read error." << endl;
        return -1;
    } else if (r == 0) {
        cout << "peer close." << endl;
        return 0;
    } else {
        cout << recv_buf;
    }
    return 0;
}