Unix 域协议的实现

注意：
1）启动 server 后，bind 后会在对应目录创建一个文件（权限是 0777&~umask）。这文件的类型是s。表示是套接口文件。
可以通过ls -al查看。

srwxrwxr-x  1 xcy xcy     0  1月  3 10:29 test_socket

2）若套接口文件存在，则 bind 会出错。为此可以先把该文件删掉。（server 中的 unlink 就干这个的）
3）创建的套接口文件最好为绝对路径。建议指定在/tmp目录下。比如把上面的目录改成 `/tmp/test_socket`
3）UNIX 域流式套接字 connect 发现监听队列满时，会立刻返回一个 ECONNREFUSED，这和 TCP 不同，如果监听队列满了，会忽略到来的 SYN，这会导致对方重传 SYN。