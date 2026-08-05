/* #include <fcntl.h> */
/* #include <sys/stat.h> */
/* #include <semaphore.h> */
/* #include <sys/ipc.h> */
/* #include <sys/types.h> */
#include <sys/sem.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "ssemaphore.h"

union semun
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

key_t KeyCreate(int int_key)
{
    key_t key = 0;
    key = ftok("/home/daher/git/projects/watch_dog/include/sem_manipulation", int_key);
    if (-1 == key)
    {
        puts("ftok");
        exit(1);
    }
    return key;
}

int SemIdCreate(key_t key, int val)
{
    int sem_id = 0;
    union semun a;
    a.val = val;
    sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (errno == EEXIST)
    {
        sem_id = semget(key, 1, 0);
    }
    else if (0 <= sem_id)
    {

        if (-1 == semctl(sem_id, 0, SETVAL, a))
        {
            puts("semget");
            exit(1);
        }
    }
    return sem_id;
}

void IncDecSem(int sem_id, int oper, int undo_flag)
{
    struct sembuf sb = {0};
    sb.sem_op = oper;
    if (1 == undo_flag)
    {

        sb.sem_flg = SEM_UNDO;
        semop(sem_id, &sb, 1);
        sb.sem_flg = 0;
    }
    else
    {
        semop(sem_id, &sb, 1);
    }
}
