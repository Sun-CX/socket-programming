#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "io-wrapper.h"
#include "max_heap.h"

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

    //使用最大堆来保存已连接套接字，可以方便地知道最大套接字的值
    max_heap<int> clients(FD_SETSIZE);

    int ready;//可读取的事件个数
    int max_fd = listen_fd;
    fd_set r_set;
    fd_set all_set;
    //将指定的文件描述符集清空，在对文件描述符集合进行设置前，必须对其进行初始化，如果不清空，由于在系统分配内存空间后，通常并不作清空处理，所以结果是不可知的。
    FD_ZERO(&r_set);
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

                if (clients.isFull()) {
                    cerr << "too many connections!" << endl;
                    return -1;
                }

                if ((con_fd = accept(listen_fd, (sockaddr *) &client_addr, &len)) < 0) {
                    cerr << "accept error." << endl;
                    return -1;
                }
                cout << "client ip: " << inet_ntoa(client_addr.sin_addr) << ", client port: "
                     << ntohs(client_addr.sin_port) << endl;

                FD_SET(con_fd, &all_set);
                clients.insert(con_fd);
//                cout << clients;
                clients.max(max_fd);
                if (ready == 1) continue;
            }

            auto ptr = clients.ptr();
            for (int i = 0; i < clients.size(); i++) {
                if (FD_ISSET(ptr[i], &r_set)) {
                    auto read_ret = readline(ptr[i], recv_buf, sizeof(recv_buf));
                    if (read_ret < 0) {
                        cerr << "read error." << endl;
                        continue;
                    } else if (read_ret == 0) {
                        cout << "client close." << endl;
//                        close(ptr[i]);
                        shutdown(ptr[i], SHUT_RDWR);
                        FD_CLR(ptr[i], &all_set);
                        clients.remove(i--);
                        if (!clients.max(max_fd)) max_fd = listen_fd;
                    } else {
                        printf("%.*s", (int) read_ret, recv_buf);
                        sleep(8);
                        write(ptr[i], recv_buf, read_ret);
                    }
                    if (ready == 1) break;
                }
            }
        }
    }
    return 0;
}