#include <string.h> /*memset*/
#include <stdio.h>
#include <stdatomic.h> /*atomic */
#include <pthread.h>   /*threads*/
#include <stdlib.h>    /*malloc*/
#include <assert.h>    /*assert*/
#include <unistd.h>    /*sleep */
#include <semaphore.h> /* semaphore */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>


static key_t KeyCreate();
static int SemIdCreate(key_t key, int val);


#define FSQ_ELEMENTS (10)
#define ERROR (-1)
unsigned int sem_id = 0;


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct fsq
{
    int *arr;
    size_t capacity;
    size_t write;
    size_t read;
} fsq_t;


union semun
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
                                           (Linux-specific) */
};


fsq_t *FsqCreat(size_t size)
{
    fsq_t *new_fsq = malloc(sizeof(fsq_t));
    if (NULL == new_fsq)
    {
        return NULL;
    }

    memset(new_fsq, 0,sizeof(fsq_t));

    new_fsq->arr = malloc(sizeof(int) * size);
    if (NULL == new_fsq->arr)
    {
        free(new_fsq);
        new_fsq = NULL;
        return NULL;
    }
    memset(new_fsq->arr, 0, sizeof(int) * size);
    new_fsq->capacity = size;

    return new_fsq;
}
void FsqDestroy(fsq_t *fsq)
{
    free(fsq->arr);
    fsq->arr = NULL;
    free(fsq);
    fsq = NULL;
}

void FsqEnq(fsq_t *fsq, int data)
{
    

    assert(NULL != fsq);
    fsq->write = (fsq->write + 1) % fsq->capacity;
    fsq->arr[fsq->write] = data;
    printf("data to push = %d\n", data);

}

int FsqDeq(fsq_t *fsq)
{
    int return_value = 0;
    
    assert(NULL != fsq);
    fsq->read = (fsq->read + 1) % fsq->capacity;
    return_value = fsq->arr[fsq->read];
    return return_value;
}

void *ProducerFunc(void *idx)
{
    static size_t i = 1;
    fsq_t *fsq = (fsq_t *)idx;
    struct sembuf sb = {0};
 
    sb.sem_op = 1;
    semop(sem_id, &sb, 1);

    pthread_mutex_lock(&lock);
        FsqEnq(fsq, i);
    ++i;
    pthread_mutex_unlock(&lock);

    return NULL;
}

void *ConsumerFunc(void *idx)
{
    fsq_t *fsq = (fsq_t *)idx;
    int data = 0;
    struct sembuf sb = {0};
    int sem_val = 0;
    union semun a;
    
    sb.sem_op = -1;
    semop(sem_id, &sb, 1);

    pthread_mutex_lock(&lock);
    sem_val = semctl(sem_id, 0, GETVAL);
    if((size_t)sem_val > fsq->capacity)
    {
        fsq->read = (fsq->write + 1) % fsq->capacity;
        a.val = fsq->capacity;
        if (-1 == semctl(sem_id, 0, SETVAL, a))
        {
            perror("semget");
            exit(1);
        }
    }
    data = FsqDeq(fsq);

    pthread_mutex_unlock(&lock);
    printf("data of pop :%d\n", data);

    return NULL;
}

int main()
{
    pthread_t producer[FSQ_ELEMENTS] = {0};
    pthread_t consumer[FSQ_ELEMENTS] = {0};
    int i = 0;
    key_t key = 0;

    fsq_t *fsq = FsqCreat(FSQ_ELEMENTS);
    if (NULL == fsq)
    {
        return 1;
    }

    key = KeyCreate();

    sem_id = SemIdCreate(key,1);

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    while (FSQ_ELEMENTS > i)
    {
        /* code */
        pthread_create(&producer[i], NULL, ProducerFunc, (void *)fsq);
        pthread_create(&consumer[i], NULL, ConsumerFunc, (void *)fsq);
        ++i;
    }

    i = 0;
    while (FSQ_ELEMENTS > i)
    {
        pthread_join(producer[i], NULL);
        pthread_join(consumer[i], NULL);
        ++i;
    }

    FsqDestroy(fsq);
    semctl(sem_id, 0, IPC_RMID, 0);
    return 0;
}


static key_t KeyCreate()
{
    key_t key = 0;
    key = ftok("sem_manipulation", 20);
    if (ERROR == key)
    {
        perror("ftok");
        exit(1);
    }
    return key;
}

static int SemIdCreate(key_t key, int val)
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
        a.val = 0;
        if (-1 == semctl(sem_id, 0, SETVAL, a))
        {
            perror("semget");
            exit(1);
        }
    }
    return sem_id;
}
