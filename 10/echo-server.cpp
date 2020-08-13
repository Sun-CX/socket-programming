#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

using namespace std;

void echo_svr(int fd) {
    char recv_buf[1024];
    sockaddr_in peer_addr;
    socklen_t len = sizeof(peer_addr);
    ssize_t n;
    while (true) {
        memset(recv_buf, 0, sizeof(recv_buf));
        n = recvfrom(fd, recv_buf, sizeof(recv_buf), 0, (sockaddr *) &peer_addr, &len);
        if (n < 0) {
            if (errno == EINTR) continue;
            cerr << "recvfrom error." << endl;
            break;
        } else if (n > 0) {
            cout << recv_buf;
            sendto(fd, recv_buf, n, 0, (sockaddr *) &peer_addr, len);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    int sock_fd;
    if ((sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        cerr << "socket open error." << endl;
        return -1;
    }

    //*********************************** REUSEADDR ***********************************
//    int on = 1;
//    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
//        cerr << "setsockopt error." << endl;
//        return -1;
//    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.105");
    server_addr.sin_port = htons(8080);

    if (bind(sock_fd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "bind error." << endl;
        return -1;
    }

    echo_svr(sock_fd);
    return 0;
}