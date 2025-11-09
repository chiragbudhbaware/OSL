/* server.c — creates shared memory and writes a message */
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

int main() {
    key_t key = ftok("shmfile", 65);  // unique key for shared memory
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);  // create shared memory
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    char *str = (char *) shmat(shmid, NULL, 0);  // attach to shared memory
    if (str == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Enter a message to write to shared memory: ");
    fgets(str, 1024, stdin);  // read message from user

    printf("Server: message written to shared memory.\n");

    // detach from shared memory
    shmdt(str);

    return 0;
}
