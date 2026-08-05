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

static int LocksInit();

#define FSQ_ELEMENTS (10)
#define ERROR (-1)

sem_t sem_read;
sem_t sem_write;

pthread_mutex_t lock_enq;
pthread_mutex_t lock_deq;

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

    sem_wait(&sem_write);
    pthread_mutex_lock(&lock_enq);

    FsqEnq(fsq, i);
    ++i;

    pthread_mutex_unlock(&lock_enq);
    sem_post(&sem_read);


    return NULL;
}

void *ConsumerFunc(void *idx)
{
    fsq_t *fsq = (fsq_t *)idx;
    int data = 0;

    sem_wait(&sem_read);

    pthread_mutex_lock(&lock_deq);

    data = FsqDeq(fsq);    

    pthread_mutex_unlock(&lock_deq);
    printf("data of pop :%d\n", data);

    sem_post(&sem_write);


    return NULL;
}

int main()
{
    pthread_t producer[FSQ_ELEMENTS] = {0};
    pthread_t consumer[FSQ_ELEMENTS] = {0};
    int i = 0;
 
    fsq_t *fsq = FsqCreat(FSQ_ELEMENTS);
    if (NULL == fsq)
    {
        return 1;
    }

    sem_init(&sem_write, 0, FSQ_ELEMENTS);
    sem_init(&sem_read, 0, 0);
  
    if(1 == LocksInit())
    {
        return 1;
    }

    while (FSQ_ELEMENTS > i)
    {
       
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

    sem_destroy(&sem_write);
    sem_destroy(&sem_read);
    FsqDestroy(fsq);
    
    
    return 0;
}
static int LocksInit()
{
    if (pthread_mutex_init(&lock_enq, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    if (pthread_mutex_init(&lock_deq, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    return 0;
}
