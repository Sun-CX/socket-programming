线程的创建:

pthread_detach():脱离线程，不会产生僵尸线程
pthread_self():返回一个线程的线程 ID
pthread_cancel():取消一个执行中的线程，类比为进程 kill 函数

线程自杀：pthread_exit();线程他杀：pthread_cancel()

echo-server 多线程版


