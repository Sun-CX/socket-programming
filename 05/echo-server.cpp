#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
    int listen_fd;
    if ((listen_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cerr << "socket open error." << endl;
        return -1;
    }

    sockaddr_in server_addr = {.sin_family=AF_INET, .sin_port = htons(8080), .sin_addr.s_addr = inet_addr(
            "192.168.0.104")};
    //*********************************** REUSEADDR ***********************************
    int on = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        cerr << "setsockopt error." << endl;
        return -1;
    }
    //*********************************** REUSEADDR ***********************************
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

    vector<int> clients;
    clients.reserve(FD_SETSIZE);

    int ready;//检测到的事件个数
    int max_fd = listen_fd;
    fd_set r_set;
    fd_set all_set;
    FD_ZERO(&r_set);//将指定的文件描述符集清空，在对文件描述符集合进行设置前，必须对其进行初始化，如果不清空，由于在系统分配内存空间后，通常并不作清空处理，所以结果是不可知的。
    FD_ZERO(&all_set);
    FD_SET(listen_fd, &all_set);
    while (true) {
        r_set = all_set;
        ready = select(max_fd + 1, &r_set, nullptr, nullptr, nullptr);
        if (ready == -1) {
            if (errno == EINTR) continue;
            else return -1;
        } else if (ready == 0) continue;
        else {
            if (FD_ISSET(listen_fd, &r_set)) {
                if (clients.size() == FD_SETSIZE) {
                    cerr << "too many connections!" << endl;
                    return -1;
                }
                if ((con_fd = accept(listen_fd, (sockaddr *) &client_addr, &len)) < 0) {
                    cerr << "accept error." << endl;
                    return -1;
                }
                cout << "clients ip: " << inet_ntoa(client_addr.sin_addr) << ", clients port: "
                     << ntohs(client_addr.sin_port)
                     << endl;
                clients.push_back(con_fd);

                FD_SET(con_fd, &all_set);
                if (con_fd > max_fd) max_fd = con_fd;
                if (--ready == 0) continue;
            }

            for (auto it = clients.begin(); it != clients.end(); it++) {
                if (FD_ISSET(*it, &r_set)) {
                    auto rr = read(*it, recv_buf, sizeof(recv_buf));
                    if (rr < 0) {
                        cerr << "read error." << endl;
                    } else if (rr == 0) {
                        cout << "clients close." << endl;
                        FD_CLR(*it, &all_set);
                        clients.erase(it--);
                        close(*it);
                    } else {
                        cout << recv_buf;
                        write(*it, recv_buf, rr);
                        memset(recv_buf, 0, sizeof(recv_buf));
                    }
                    if (--ready == 0) break;
                }
            }
        }
    }
    return 0;
}