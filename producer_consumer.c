/* producer_consumer.c
 * Simple demonstration of Producer-Consumer problem
 * using counting semaphores and mutex.
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;   // circular buffer indexes

pthread_mutex_t mutex;
sem_t empty;   // counts empty slots
sem_t full;    // counts filled slots

void* producer(void* arg) {
    int item = 1;
    while (1) {
        sem_wait(&empty);          // wait if buffer is full
        pthread_mutex_lock(&mutex);

        // critical section
        buffer[in] = item;
        printf("Producer produced item %d at position %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        item++;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);           // signal that buffer has an item

        sleep(1);  // slow down to visualize
    }
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);           // wait if buffer is empty
        pthread_mutex_lock(&mutex);

        // critical section
        int item = buffer[out];
        printf("Consumer consumed item %d from position %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);          // signal that buffer has space

        sleep(2);  // slow down consumer
    }
}

int main() {
    pthread_t prod, cons;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE); // initially all slots empty
    sem_init(&full, 0, 0);            // no items yet

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
