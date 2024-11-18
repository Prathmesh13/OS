#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int sharedData = 0;
int read_count = 0;

pthread_mutex_t mutex_data; 
pthread_mutex_t mutex_read_count;
pthread_cond_t cond_writer; 

void* reader(void* arg) {
    int id = *(int*)arg;

    while (true) {
        pthread_mutex_lock(&mutex_read_count);

        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&mutex_data);
        }

        pthread_mutex_unlock(&mutex_read_count);

        cout << "Reader " << id << " is reading data: " << sharedData << endl;
        sleep(1); 

        pthread_mutex_lock(&mutex_read_count);
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&mutex_data); 
        }
        pthread_mutex_unlock(&mutex_read_count);

        sleep(1); 
    }

    return nullptr;
}

void* writer(void* arg) {
    int id = *(int*)arg;

    while (true) {
        pthread_mutex_lock(&mutex_data);

        sharedData++;
        cout << "Writer " << id << " is writing data: " << sharedData << endl;
        sleep(2); 

        pthread_mutex_unlock(&mutex_data);

        sleep(1); 
    }
    return nullptr;
}

int main() {
    const int NUM_READERS = 3;
    const int NUM_WRITERS = 2;

    pthread_mutex_init(&mutex_data, nullptr);
    pthread_mutex_init(&mutex_read_count, nullptr);
    pthread_cond_init(&cond_writer, nullptr);

    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];

    for (int i = 0; i < NUM_READERS; i++) {
        int* id = new int(i + 1);
        pthread_create(&readers[i], nullptr, reader, id);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        int* id = new int(i + 1);
        pthread_create(&writers[i], nullptr, writer, id);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], nullptr);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], nullptr);
    }

    pthread_mutex_destroy(&mutex_data);
    pthread_mutex_destroy(&mutex_read_count);
    pthread_cond_destroy(&cond_writer);

    return 0;
}