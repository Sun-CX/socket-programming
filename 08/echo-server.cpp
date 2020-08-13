#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include "io-wrapper.h"

using namespace std;

int main(int argc, char *argv[]) {
    int listen_fd;
    if ((listen_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cerr << "socket open error." << endl;
        return -1;
    }

    //*********************************** REUSEADDR ***********************************
    int on = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        cerr << "setsockopt error." << endl;
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
    char recv_buf[1024];

    int max_i = 0;
    pollfd clients[2048];

    for (auto &client : clients) client.fd = -1;

    int ready;//可读取的事件个数

    clients[0].fd = listen_fd;
    clients[0].events = POLLIN;

    while (true) {
        ready = poll(clients, max_i + 1, -1);
        if (ready == -1) {
            if (errno == EINTR) continue;
            else return -1;
        } else if (ready == 0) continue;
        else {
            if (clients[0].revents & POLLIN) {
                if ((con_fd = accept(listen_fd, (sockaddr *) &client_addr, &len)) < 0) {
                    cerr << "accept error." << endl;
                    return -1;
                }
                cout << "client ip: " << inet_ntoa(client_addr.sin_addr) << ", client port: "
                     << ntohs(client_addr.sin_port) << endl;
                int i;
                for (i = 0; i < 2048; i++) {
                    if (clients[i].fd < 0) {
                        clients[i].fd = con_fd;
                        clients[i].events = POLLIN;
                        if (i > max_i) max_i = i;
                        break;
                    }
                }
                if (i == 2048) {
                    cerr << "too many connections." << endl;
                    return -1;
                }
                if (ready == 1) continue;
            }

            for (int i = 1; i <= max_i; i++) {
                con_fd = clients[i].fd;
                if (clients[i].revents & POLLIN) {
                    auto read_ret = readline(con_fd, recv_buf, sizeof(recv_buf));
                    if (read_ret < 0) {
                        cerr << "read error." << endl;
                        continue;
                    } else if (read_ret == 0) {
                        cout << "client close." << endl;
//                        close(ptr[i]);
                        shutdown(con_fd, SHUT_RDWR);
                        clients[i].fd = -1;
//                        clients.remove(i--);
//                        if (!clients.max(max_fd)) max_fd = listen_fd;
                    } else {
                        printf("%.*s", (int) read_ret, recv_buf);
//                        sleep(8);
                        write(con_fd, recv_buf, read_ret);
                    }
                    if (ready == 1) break;
                }
            }
        }
    }
    return 0;
}