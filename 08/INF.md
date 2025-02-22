###### 用 select 实现的并发服务器，能达到的并发数受两方面的限制：
* 一个进程能打开的最大文件描述符的限制。这可以通过调整内核参数来克服
> 通过 `ulimit -n` 查看

* select 中的 fd_set 集合的容量的限制（FD_SETSIZE），这需要重新编译内核才能解决

###### 用 poll 实现的并发服务器，能达到的并发数的限制：
*  一个进程能打开的最大文件描述符是有限的，可以通过 `ulimit -n number` 命令来更改。但是这个数字也不能改成无限大，它还受到操作系统所能打开的最大文件描述符个数的限制。
操作系统所能打开的最大文件描述符的个数与内存大小有关，可以通过 `cat /proc/sys/fs/file-max` 命令来查看。


select 与 poll 的共同点在于：内核要遍历所有文件描述符，直到找到发生事件的文件描述符，
当服务器并发数增长的时候，服务器要遍历的次数也随之增长，那么效率就会随之下降。

可以通过 epoll() 来解决这个问题。