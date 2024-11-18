#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include <unistd.h>

using namespace std;

queue<int> buffer;
const int BUFFER_SIZE = 5;

sem_t emptySlot;
sem_t fullSlot;

pthread_mutex_t bufferMutex;

void* producer(void* arg)
{
    int id = (intptr_t)arg;
    int item =0;

    while(true)
    {
        item++;
        sleep(1);

        sem_wait(&emptySlot);
        pthread_mutex_lock(&bufferMutex);

        buffer.push(item);
        cout << "Producer " << id << " produced item: " << item << endl;

        pthread_mutex_unlock(&bufferMutex);
        sem_post(&fullSlot);
    }
    return nullptr;
}

void* consumer(void* arg)
{
    int id = (intptr_t)arg;

    while(true)
    {
        sem_wait(&fullSlot);
        pthread_mutex_lock(&bufferMutex);

        int item = buffer.front();
        buffer.pop();
        cout << "Consumer " << id << " consumed item: " << item << endl;

        pthread_mutex_unlock(&bufferMutex);
        sem_post(&emptySlot);

        sleep(2);
    }
    return nullptr;
}

int main()
{
    const int num_producer = 2;
    const int num_consumer = 2;

    sem_init(&emptySlot, 0, BUFFER_SIZE);
    sem_init(&fullSlot, 0, 0);
    pthread_mutex_init(&bufferMutex, nullptr);

    pthread_t producers[num_producer];
    pthread_t consumers[num_consumer];

    for(int i=0; i<num_producer; i++)
    {
        pthread_create(&producers[i], nullptr, producer, (void*)(intptr_t)(i+1));
    }

    for(int i=0; i<num_consumer; i++)
    {
        pthread_create(&consumers[i], nullptr, consumer, (void*)(intptr_t)(i+1));
    }

    for(int i=0; i<num_producer; i++)
    {
        pthread_join(producers[i], nullptr);
    }

    for(int i=0; i<num_consumer; i++)
    {
        pthread_join(consumers[i], nullptr);
    }

    sem_destroy(&emptySlot);
    sem_destroy(&fullSlot);
    pthread_mutex_destroy(&bufferMutex);

    return 0;
}