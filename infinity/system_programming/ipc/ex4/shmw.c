#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "include/ssemaphore.h"

int main()
{
    /* ftok to generate unique key*/
    key_t key = KeyCreate(5);

    /* shmget returns an identifier in shmid*/
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    /* shmat to attach to shared memory*/
    char *str = (char *)shmat(shmid, (void *)0, 0);
    /* char name[] = "daher"; */
    char *runner = str;
    *runner = 'd';
    *++runner = 'a';
    *++runner = 'h';
    *++runner = 'e';
    *++runner = 'r';

    printf("Data written in memory: %s\n", str);

    /*detach from shared memory*/
    shmdt(str);

    return 0;
}