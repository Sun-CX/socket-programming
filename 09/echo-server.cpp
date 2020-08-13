#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <sys/epoll.h>
#include <fcntl.h>
#include <algorithm>
#include "io-wrapper.h"

using namespace std;

typedef vector<epoll_event> EventList;

//设置 IO 为非阻塞模式
void active_nonblock(int fd) {
    int ret;
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        std::cerr << "fcntl error." << std::endl;
        exit(-1);
    }
    flags |= O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1) {
        std::cerr << "fcntl error." << std::endl;
        exit(-1);
    }
}

//设置 IO 为非阻塞模式
void deactivate_nonblock(int fd) {
    int ret;
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        std::cerr << "fcntl error." << std::endl;
        exit(-1);
    }
    flags &= ~O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1) {
        std::cerr << "fcntl error." << std::endl;
        exit(-1);
    }
}

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
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.104");
    server_addr.sin_port = htons(8080);

    if (bind(listen_fd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "bind error." << endl;
        return -1;
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {
        cerr << "listen error." << endl;
        return -1;
    }

    vector<int> clients;
    /**
     * epoll_create() 内部使用的是 hash 表，size 表示的是 hash 表的容量
     * epoll_create1() 内部使用的是红黑树，因此不再需要 size 参数了
     * EPOLL_CLOEXEC 作用是当进程被替换的时候文件描述符会关闭
     */
    int epollfd = epoll_create1(EPOLL_CLOEXEC);

    epoll_event event;
    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;

    epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_fd, &event);

    EventList events(16);

    sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int con_fd;
    char recv_buf[1024];

    int ready;//可读取的事件个数

    while (true) {
        ready = epoll_wait(epollfd, &*events.begin(), events.size(), -1);
        if (ready == -1) {
            if (errno == EINTR) continue;
            else return -1;
        } else if (ready == 0) continue;
        else {
            //容器已满，扩容
            if (ready == events.size()) events.resize(events.size() * 2);

            int i;
            for (i = 0; i < ready; i++) {
                //监听套接字产生了可读事件
                if (events[i].data.fd == listen_fd) {
                    if ((con_fd = accept(listen_fd, (sockaddr *) &client_addr, &len)) < 0) {
                        cerr << "accept error." << endl;
                        return -1;
                    }
                    cout << "client ip: " << inet_ntoa(client_addr.sin_addr) << ", client port: "
                         << ntohs(client_addr.sin_port) << endl;
                    clients.push_back(con_fd);
                    active_nonblock(con_fd);
                    event.data.fd = con_fd;
                    event.events = EPOLLIN | EPOLLET;
                    epoll_ctl(epollfd, EPOLL_CTL_ADD, con_fd, &event);
                } else if (events[i].events & EPOLLIN) {
                    con_fd = events[i].data.fd;
                    auto read_ret = readline(con_fd, recv_buf, sizeof(recv_buf));
                    if (read_ret < 0) {
                        cerr << "read error." << endl;
                        continue;
                    } else if (read_ret == 0) {
                        cout << "client close." << endl;
//                        close(ptr[i]);
                        shutdown(con_fd, SHUT_RDWR);
                        event = events[i];
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, con_fd, &event);
                        clients.erase(remove(clients.begin(), clients.end(), con_fd), clients.end());
//                        if (!clients.max(max_fd)) max_fd = listen_fd;
                    } else {
                        printf("%.*s", (int) read_ret, recv_buf);
//                        sleep(8);
                        write(con_fd, recv_buf, read_ret);
                    }
                }

            }
        }
    }
    return 0;
}