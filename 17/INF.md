线程的属性：

初始化与销毁属性：
pthread_attr_init();
pthread_attr_destroy();

获取与设置分离属性：
pthread_attr_getdetachstate();
pthread_attr_setdetachstate();

获取与设置栈大小：
pthread_attr_setstacksize();
pthread_attr_getstacksize();

获取与设置栈溢出保护区大小：
pthread_attr_setguardsize();
pthread_attr_getguardsize();

获取与设置线程竞争范围：
pthread_attr_getscope();
pthread_attr_setscope();

获取与设置调度策略：
pthread_attr_getschedpolicy();
pthread_attr_setschedpolicy();

获取与设置继承的调度策略：
pthread_attr_getinheritsched();
pthread_attr_setinheritsched();

获取与设置调度参数：
pthread_attr_getschedparam();
pthread_attr_setschedparam();

获取与设置并发级别：
pthread_setconcurrency();
pthread_getconcurrency();
仅适用于 N：M 线程模型中有效，设置并发级别，给内核一个提示，表示提供给定级别数量的核心线程来映射用户线程是高效的。


线程私有数据（TSD）：Thread-specific Data
pthread_key_create():
pthread_key_delete();

pthread_once();
pthread_once_t once_control = PTHREAD_ONCE_INIT;



