#include <iostream>
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

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int detach_state;
    pthread_attr_getdetachstate(&attr, &detach_state);
    if (detach_state == PTHREAD_CREATE_JOINABLE) {
        cout << "detach state: PTHREAD_CREATE_JOINABLE" << endl;
    } else if (detach_state == PTHREAD_CREATE_DETACHED) {
        cout << "detach state: PTHREAD_CREATE_DETACHED" << endl;
    }
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    cout << "stack size: " << size << endl;

    pthread_attr_getguardsize(&attr, &size);
    cout << "guard size: " << size << endl;

    int scope;
    pthread_attr_getscope(&attr, &scope);
    if (scope == PTHREAD_SCOPE_PROCESS) {
        cout << "scope: PTHREAD_SCOPE_PROCESS" << endl;
    }
    if (scope == PTHREAD_SCOPE_SYSTEM) {
        cout << "scope: PTHREAD_SCOPE_SYSTEM" << endl;
    }

    int policy;
    pthread_attr_getschedpolicy(&attr, &policy);
    if (policy == SCHED_FIFO) {
        cout << "policy: SCHED_FIFO" << endl;
    } else if (policy == SCHED_RR) {
        cout << "policy: SCHED_RR" << endl;
    } else if (policy == SCHED_OTHER) {
        cout << "policy: SCHED_OTHER" << endl;
    }

    int inherit;
    pthread_attr_getinheritsched(&attr, &inherit);
    if (inherit == PTHREAD_INHERIT_SCHED) {
        cout << "inheritsched: PTHREAD_INHERIT_SCHED" << endl;
    } else if (inherit == PTHREAD_EXPLICIT_SCHED) {
        cout << "inheritsched: PTHREAD_EXPLICIT_SCHED" << endl;
    }

    sched_param param;
    pthread_attr_getschedparam(&attr, &param);
    cout << "ched priority: " << param.sched_priority << endl;

    pthread_attr_destroy(&attr);

    int con = pthread_getconcurrency();
    cout << "concurrency: " << con << endl;

//    pthread_t tid;
//    int ret;
//    if ((ret = pthread_create(&tid, nullptr, thread_routine, nullptr)) < 0) {
//        fprintf(stderr, "pthread create: %s\n", strerror(ret));
//        return -1;
//    }
//    for (int i = 0; i < 20; ++i) {
//        cout << 'A' << endl;
//        usleep(20);
//    }
//    sleep(1);
//    void *val;
//    //等待新创建的线程运行完毕再退出运行
//    pthread_join(tid, &val);
//    cout << (char *) val;
//    printf("return msg = %s\n", (char *) val);
    return 0;
}