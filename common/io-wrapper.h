//
// Created by suncx on 2020/3/22.
//

#ifndef SOCKET_LAB_IO_WRAPPER_H
#define SOCKET_LAB_IO_WRAPPER_H

#include <cerrno>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

/**
 * 读取固定长度的字节
 * @param fd
 * @param buf
 * @param count 字节长度
 * @return
 */
ssize_t read_nbytes(int fd, void *buf, size_t count);

ssize_t write_nbytes(int fd, const void *buf, size_t count);

/**
 * 从指定文件描述符中偷窥 n 个字节的数据，但是并不从缓冲区中消耗它们，不一定能接收到 n 个字节的数据，只要偷窥到数据就返回
 * @param fd
 * @param buf
 * @param n
 * @return 偷窥到的字节数
 */
ssize_t recv_peek(int fd, void *buf, size_t n);

/**
 * @param arr
 * @param len 字符数组长度
 * @param pos '\n' 在数组中的位置，如果存在
 * @return 判断 arr 中是否包含 '\n' 字符
 */
bool contains_enter(const char *arr, size_t len, size_t &pos);

/**
 * 从 fd 读取一行最多 max_len 个字节的数据，如果一行的数据超过了 max_len，则产生 buffer overflow
 * @param fd
 * @param buf
 * @param max_len
 * @return 一行数据的长度
 */
ssize_t readline(int fd, void *buf, size_t max_len);

#endif //SOCKET_LAB_IO_WRAPPER_H
