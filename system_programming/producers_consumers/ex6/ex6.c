
#include <stdio.h>
#include <pthread.h> /* thread */
#include <semaphore.h>
#include <stdatomic.h>
#include <stdlib.h>  /* malloc */
#include <sys/sem.h> /* struct sembuf, SETALL, GETVAL, SEM_UNDO, IPC_CREAT */
#include "uid.h"

static void *Threads()
{

    UID_t new_uid = UIDCreate();
    if (UIDIsSame(new_uid, UIDGetBadUid()))
    {
        puts("ERROR: BadUID!");
        return NULL;
    }

    printf("thread uid: %d\nthread time_stamp: %ld\nthread counter: %ld\n",
           new_uid.pid, new_uid.time_stamp, new_uid.counter);
    return NULL;
}
int main()
{
    size_t i = 0;
    pthread_t producer = 0;
    while (i < 1000)
    {
        while (0 != pthread_create(&producer, NULL, Threads, NULL));
        ++i;
    }
    i = 0;
    while (i < 1000)
    {
        pthread_join(producer, NULL);
        ++i;
    }

    return (0);
}