/* client.c — reads the message from shared memory */
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main() {
    key_t key = ftok("shmfile", 65);  // same key as server
    int shmid = shmget(key, 1024, 0666);  // get existing shared memory
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    char *str = (char *) shmat(shmid, NULL, 0);  // attach to shared memory
    if (str == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Client: message read from shared memory:\n");
    printf(">>> %s\n", str);

    // detach and destroy shared memory
    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);  // remove shared memory segment

    return 0;
}
