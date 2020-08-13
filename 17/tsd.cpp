#include <iostream>
#include <unistd.h>
//#include <pthread.h>

using namespace std;

//
// Created by suncx on 2020/4/18.
//
pthread_key_t key_tsd;

pthread_once_t once_control = PTHREAD_ONCE_INIT;

struct tsd {
    pthread_t tid;
    char *str;
};

void destroy_routine(void *arg) {
    delete (tsd *) arg;
    cout << "destroy ..." << endl;
}

void once_routine() {
    pthread_key_create(&key_tsd, destroy_routine);
    cout << "init..." << endl;
}

void *thread_routine(void *arg) {
    pthread_once(&once_control, once_routine);
    tsd *val = new tsd;
    val->tid = pthread_self();
    val->str = (char *) arg;
    pthread_setspecific(key_tsd, val);
    cout << "pthread_setspecific, tid: " << (char *) arg << ' ' << val << endl;

    val = (tsd *) pthread_getspecific(key_tsd);
    cout << "tid = " << val->tid << " str = " << val->str << endl;
    val = (tsd *) pthread_getspecific(key_tsd);
    cout << "tid = " << val->tid << " str = " << val->str << endl;
    sleep(2);
    return nullptr;
}

int main(int argc, char *argv[]) {

//    pthread_key_create(&key_tsd, destroy_routine);
    pthread_t tid1;
    pthread_t tid2;
    pthread_create(&tid1, nullptr, thread_routine, (void *) "thread-1");
    pthread_create(&tid2, nullptr, thread_routine, (void *) "thread-2");

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);

    pthread_key_delete(key_tsd);
    return 0;
}
