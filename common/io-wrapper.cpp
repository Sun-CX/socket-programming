//
// Created by suncx on 2020/3/22.
//

#include "io-wrapper.h"

ssize_t read_nbytes(int fd, void *buf, size_t count) {
    size_t left = count;//剩余要读取的字节数
    ssize_t has_read;//已经读取的字节数
    char *p = static_cast<char *>(buf);
    while (left > 0) {
        if ((has_read = read(fd, p, left)) < 0) {
            if (errno == EINTR) {
                continue;//信号中断的情况下不认为其出错
            } else return -1;
        } else if (has_read == 0) break;
        else {
            left -= has_read;
            p += has_read;
        }
    }
    return count - left;
}

ssize_t write_nbytes(int fd, const void *buf, size_t count) {
    size_t left = count;
    ssize_t has_written;
    char *p = (char *) buf;
    while (left > 0) {
        if ((has_written = write(fd, p, left)) == -1) {
            if (errno == EINTR) {
                std::cerr << "write interrupt." << std::endl;
                continue;
            } else return -1;
        } else {
            left -= has_written;
            p += has_written;
        }
    }
    return count - left;
}

ssize_t recv_peek(int fd, void *buf, size_t n) {
    ssize_t rr;
    while (true) {
        rr = recv(fd, buf, n, MSG_PEEK);
        if (rr == -1 and errno == EINTR) {
            continue;
        } else return rr;
    }
}

bool contains_enter(const char *arr, size_t len, size_t &pos) {
    for (size_t i = 0; i < len; i++) {
        if (arr[i] == 10) {
            pos = i;
            return true;
        }
    }
    return false;
}

ssize_t readline(int fd, void *buf, size_t max_len) {
    size_t left = max_len;
    ssize_t peek_ret;
    ssize_t read_ret;
    char *p = static_cast<char *>(buf);
    size_t pos;
    while (true) {
        peek_ret = recv_peek(fd, p, left);
        if (peek_ret <= 0) return peek_ret;//如果没有读到 \n 客户端进程关闭了，则之前读取的部分数据作废
        else {
            if (contains_enter(p, peek_ret, pos)) {
                read_ret = read_nbytes(fd, p, pos + 1);
                return read_ret == pos + 1 ? read_ret : -1;
            }

            if (peek_ret >= left) {
                std::cerr << "buffer overflow." << std::endl;
                return -1;
            }//在 n 个字节的缓冲区内还没有遇到 \n，则异常返回

            read_ret = read_nbytes(fd, p, peek_ret);
            if (read_ret != peek_ret) return -1;
            left -= read_ret;
            p += read_ret;
        }
    }
}