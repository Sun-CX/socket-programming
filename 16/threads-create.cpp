#include <iostream>
#include <cstring>
#include <unistd.h>
//#include <pthread.h>

using namespace std;

void *thread_routine(void *arg) {
    for (int i = 0; i < 20; ++i) {
        cout << 'B' << endl;
        usleep(20);
        if (i == 5) {
            pthread_exit((void *) "ABC");
        }
    }
    sleep(3);
    return nullptr;
}

int main(int argc, char *argv[]) {

    pthread_t tid;
    pthread_attr_t attr;

    int ret;
    if ((ret = pthread_create(&tid, nullptr, thread_routine, nullptr)) < 0) {
        fprintf(stderr, "pthread create: %s\n", strerror(ret));
        return -1;
    }
    for (int i = 0; i < 20; ++i) {
        cout << 'A' << endl;
        usleep(20);
    }
//    sleep(1);
    void *val;
    //等待新创建的线程运行完毕再退出运行
    pthread_join(tid, &val);
//    cout << (char *) val;
    printf("return msg = %s\n", (char *) val);
    return 0;
}