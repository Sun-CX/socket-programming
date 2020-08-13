#include <iostream>
#include <semaphore.h>
#include <cstring>
#include <unistd.h>
//#include <pthread.h>

using namespace std;

#define CONSUMERS_COUNT 1
#define PRODUCERS_COUNT 2
#define BUFFER_SIZE 10

int g_buffer[BUFFER_SIZE];

unsigned short in = 0;
unsigned short out = 0;
unsigned short produce_id = 0;
unsigned short consume_id = 0;

sem_t sem_full;
sem_t sem_empty;

pthread_mutex_t mutex;

pthread_t threads[CONSUMERS_COUNT + PRODUCERS_COUNT];

void *consume(void *arg) {
    int i;
    while (true) {
        sem_wait(&sem_empty);
        pthread_mutex_lock(&mutex);
        for (i = 0; i < BUFFER_SIZE; i++) {
            cout << "producer i: " << i << endl;
            if (g_buffer[i] == -1) {
                cout << "null";
            } else
                cout << "g_buffer[i]: " << g_buffer[i] << endl;
            if (i == out) {
                cout << "consume --->" << endl;
            }
        }
        consume_id = g_buffer[out];
        cout << "begin consume..." << consume_id;
        g_buffer[out] = -1;
        out = (out + 1) % BUFFER_SIZE;
        cout << "consume finished, consume id: " << consume_id << endl;

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_full);
        sleep(1);
    }
    return nullptr;
}

void *produce(void *arg) {
    int i;
    while (true) {
        sem_wait(&sem_full);
        pthread_mutex_lock(&mutex);

        for (i = 0; i < BUFFER_SIZE; i++) {
            cout << "producer i: " << i << endl;
            if (g_buffer[i] == -1)
                cout << "null";
            else
                cout << "g_buffer[i]: " << g_buffer[i] << endl;
            if (i == in) {
                cout << "produce --->" << endl;
            }
        }
        cout << "begin produce..." << produce_id;
        g_buffer[in] = produce_id;
        in = (in + 1) % BUFFER_SIZE;
        cout << "produce finished." << endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_empty);
        sleep(1);
    }
    return nullptr;
}

int main(int argc, char *argv[]) {

    memset(g_buffer, 1, sizeof(g_buffer));

    sem_init(&sem_full, 0, BUFFER_SIZE);
    sem_init(&sem_empty, 0, 0);

    pthread_mutex_init(&mutex, nullptr);

    for (int i = 0; i < CONSUMERS_COUNT; ++i) {
        pthread_create(&threads[i], nullptr, consume, (void *) i);
    }
    for (int i = 0; i < PRODUCERS_COUNT; ++i) {
        pthread_create(&threads[i + CONSUMERS_COUNT], nullptr, produce, (void *) i);
    }

    for (pthread_t thread : threads) {
        pthread_join(thread, nullptr);
    }

    sem_destroy(&sem_full);
    sem_destroy(&sem_empty);
    pthread_mutex_destroy(&mutex);
    return 0;
}
