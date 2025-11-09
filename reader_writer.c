/* reader_writer_reader_priority.c
 * Simple Reader-Writer (Reader Priority) demonstration using pthreads & mutexes.
 *
 * Compile:
 *   gcc reader_writer_reader_priority.c -o rw -lpthread
 * Run:
 *   ./rw
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2
#define READER_ITER 4
#define WRITER_ITER 3

/* Shared resource */
int shared_data = 0;

/* Synchronization primitives */
pthread_mutex_t readcount_mutex = PTHREAD_MUTEX_INITIALIZER; // protects read_count
pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;        // gives exclusive access to writers
int read_count = 0;

/* Reader thread function */
void *reader(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < READER_ITER; ++i) {
        /* --- reader entry section --- */
        pthread_mutex_lock(&readcount_mutex);
        read_count++;
        if (read_count == 1) {
            /* first reader locks rw_mutex to block writers */
            pthread_mutex_lock(&rw_mutex);
        }
        pthread_mutex_unlock(&readcount_mutex);

        /* --- reading (critical section) --- */
        printf("[Reader %d] reading shared_data = %d (read_count=%d)\n", id, shared_data, read_count);
        usleep(100000 + (rand() % 200000)); // simulate reading time

        /* --- reader exit section --- */
        pthread_mutex_lock(&readcount_mutex);
        read_count--;
        if (read_count == 0) {
            /* last reader unlocks rw_mutex to allow writers */
            pthread_mutex_unlock(&rw_mutex);
        }
        pthread_mutex_unlock(&readcount_mutex);

        /* non-critical section */
        usleep(100000 + (rand() % 300000));
    }
    return NULL;
}

/* Writer thread function */
void *writer(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < WRITER_ITER; ++i) {
        /* writers need exclusive access */
        pthread_mutex_lock(&rw_mutex);

        /* --- writing (critical section) --- */
        shared_data += 10;  // simple write operation
        printf(">>> [Writer %d] wrote shared_data = %d\n", id, shared_data);
        usleep(150000 + (rand() % 200000)); // simulate writing time

        pthread_mutex_unlock(&rw_mutex);

        /* non-critical section */
        usleep(200000 + (rand() % 300000));
    }
    return NULL;
}

int main(void) {
    srand((unsigned)time(NULL));
    pthread_t rthreads[NUM_READERS], wthreads[NUM_WRITERS];
    int rids[NUM_READERS], wids[NUM_WRITERS];

    /* create reader threads */
    for (int i = 0; i < NUM_READERS; ++i) {
        rids[i] = i + 1;
        if (pthread_create(&rthreads[i], NULL, reader, &rids[i]) != 0) {
            perror("pthread_create reader");
            exit(1);
        }
    }

    /* create writer threads */
    for (int i = 0; i < NUM_WRITERS; ++i) {
        wids[i] = i + 1;
        if (pthread_create(&wthreads[i], NULL, writer, &wids[i]) != 0) {
            perror("pthread_create writer");
            exit(1);
        }
    }

    /* wait for all threads to finish */
    for (int i = 0; i < NUM_READERS; ++i) pthread_join(rthreads[i], NULL);
    for (int i = 0; i < NUM_WRITERS; ++i) pthread_join(wthreads[i], NULL);

    printf("\nAll threads finished. Final shared_data = %d\n", shared_data);

    /* cleanup */
    pthread_mutex_destroy(&readcount_mutex);
    pthread_mutex_destroy(&rw_mutex);

    return 0;
}
