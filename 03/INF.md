##### 解决 TCP 粘包问题
* 发送定长包：`echo-server-1.cpp`
* 包尾添加 `\r\n` (譬如 FTP)
> recv, send 函数的使用，
> readline 实现回射服务器
> getsockname , getpeername,gethostname,gethostbyname,gethostbyaddr `get_ip.cpp`
* 通过包头加上包体的长度 `echo-server-3.cpp`
* 更加复杂的应用层协议来处理粘包问题
