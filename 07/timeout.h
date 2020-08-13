//
// Created by suncx on 2020/3/24.
//

#ifndef SOCKET_LAB_TIMEOUT_H
#define SOCKET_LAB_TIMEOUT_H

#include <sys/types.h>
#include <cerrno>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>

/**
 * timeout 实现的三种方式
 *
 * 1. 采用 alarm 信号的方法
 *
 *
 * void handler(int sig){
 *     return;
 * }
 *
 * signal(SIGALRM, handler);
 * alarm(5);
 * int ret = read(fd, buf, sizeof(buf));
 * if(ret == -1 and errno == EINTR){
 *     errno = ETIMEDOUT;
 * }
 * 这种方案有一定的问题，闹钟可能会作其他的用途，多个闹钟可能会产生冲突
 *
 * 2. 设置套接字选项：
 *    * SO_SNDTIMEO
 *    * SO_RCVTIMEO
 *
 * setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO)；
 * int ret = read(fd, buf, sizeof(buf));
 * if(ret == -1 and errno == EWOULDBLOCK){
 *     errno = ETIMEDOUT;
 * }
 * 这种方案可移植性不高，有些操作系统不支持这些套接子选项
 */

int read_timeout(int fd, unsigned wait_seconds) {

    int ret = 0;
    if (wait_seconds > 0) {
        fd_set r_set;
        FD_ZERO(&r_set);
        FD_SET(fd, &r_set);

        timeval timeout;
        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;
        do {
            ret = select(fd + 1, &r_set, nullptr, nullptr, &timeout);
        } while (ret < 0 and errno == EINTR);
        if (ret == 0) {
            ret = -1;
            errno = ETIMEDOUT;
        } else if (ret == 1) {
            ret = 0;
        }
    }
    return ret;
}

int write_timeout(int fd, unsigned wait_seconds) {
    int ret = 0;
    if (wait_seconds > 0) {
        fd_set w_set;
        FD_ZERO(&w_set);
        FD_SET(fd, &w_set);

        timeval timeout;
        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;

        do {
            ret = select(fd + 1, nullptr, &w_set, nullptr, &timeout);
        } while (ret < 0 and errno == EINTR);

        if (ret == 0) {
            ret = -1;
            errno = ETIMEDOUT;
        } else if (ret == 1) {
            return 0;
        }
    }
    return ret;
}

int accept_timeout(int fd, sockaddr_in *addr, unsigned wait_seconds) {
    int ret = 0;
    if (wait_seconds > 0) {
        fd_set r_set;
        FD_ZERO(&r_set);
        FD_SET(fd, &r_set);

        timeval timeout;
        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;
        do {
            ret = select(fd + 1, &r_set, nullptr, nullptr, &timeout);
        } while (ret < 0 and errno == EINTR);
        if (ret == -1) {
            return -1;
        } else if (ret == 0) {
            errno = ETIMEDOUT;
            return -1;
        }
        socklen_t addr_len = sizeof(addr);
        if (addr != nullptr) {
            ret = accept(fd, (sockaddr *) addr, &addr_len);
        } else {
            ret = accept(fd, nullptr, nullptr);
        }
        if (ret == -1) {
            std::cerr << "accept error." << std::endl;
        }
    }
    return ret;
}

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

int connect_timeout(int fd, sockaddr_in *addr, unsigned wait_seconds) {
    int ret = 0;

    if (wait_seconds > 0) active_nonblock(fd);

    ret = connect(fd, (sockaddr *) addr, sizeof(addr));
    if (ret < 0 and errno == EINPROGRESS) {
        fd_set w_set;
        FD_ZERO(&w_set);
        FD_SET(fd, &w_set);

        timeval timeout;
        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;
        do {
            /* 一旦连接建立，套接字就处于可写的状态 */
            ret = select(fd + 1, nullptr, &w_set, nullptr, &timeout);
        } while (ret < 0 and errno == EINTR);
        if (ret == 0) {
            ret = -1;
            errno = ETIMEDOUT;
        } else if (ret < 0) {
            return -1;
        } else if (ret == 1) {
            /* 此时有两种情况：
             * 一种是连接建立成功
             * 一种是套接字产生错误，此时错误信息不会保存在 errno 变量中，需要调用 getsockopt() 来获取
             */
            int err;
            socklen_t len = sizeof(err);
            int getsockopt_ret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &len);
            if (getsockopt_ret == -1) return -1;
            if (err == 0) ret = 0;
            else {
                errno = err;
                ret = -1;
            }
        }
    }

    if (wait_seconds > 0) deactivate_nonblock(fd);
    return ret;
}

#endif //SOCKET_LAB_TIMEOUT_H