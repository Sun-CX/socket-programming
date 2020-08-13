#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "io-wrapper.h"

using namespace std;

int main(int argc, char *argv[]) {
    int listen_fd;
    if ((listen_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cerr << "socket open error." << endl;
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.105");
    server_addr.sin_port = htons(8080);

    if (bind(listen_fd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "bind error." << endl;
        return -1;
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {
        cerr << "listen error." << endl;
        return -1;
    }

    sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int con_fd;
    if ((con_fd = accept(listen_fd, (sockaddr *) &client_addr, &len)) < 0) {
        cerr << "accept error." << endl;
        return -1;
    }

    char recv_buf[1024];
    while (true) {
        auto r = readline(con_fd, recv_buf, sizeof(recv_buf));
        if (r > 0) {
            printf("%.*s", (int) r, recv_buf);
            write(con_fd, recv_buf, r);
        } else if (r == 0) break;
        else {
            cerr << "read error." << endl;
            break;
        }
    }
//    close(con_fd);
    shutdown(con_fd, SHUT_RD);
    close(listen_fd);
//    shutdown(listen_fd, SHUT_RD);
    return 0;
}