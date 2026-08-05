#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h> /*threads*/
#include <stdlib.h>  /*malloc*/
#include <assert.h>  /*assert*/
#include <unistd.h> /*sleep */
#include <semaphore.h> /* semaphore */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>

static key_t KeyCreate();
static int SemIdCreate(key_t key);

#define _GNU_SOURCE
#define LIST_ELEMENTS (10)
#define ERROR (-1)
unsigned int sem_id = 0;
int pthread_yield(void);
atomic_long index = 0;
atomic_long counter = 0;
struct sembuf sb = {0};

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

union semun
{
    int val;               /* Value for SETVAL */
    struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
                                           (Linux-specific) */
};

pthread_mutex_t lock;

node_t *PushFront(node_t *where, void *data)
{
    node_t *new_node = NULL;
    
    assert(NULL != where);

    new_node = malloc(sizeof(node_t));
    if (NULL == new_node)
    {
        return NULL;
    }

    new_node->next = where->next;
    new_node->data = data;
    where->next = new_node;
    
    printf("data to push = %ld\n", (long)data);
    return where;
}

void *PopFront(node_t *head)
{
    void *return_value = NULL;
    node_t *tmp = NULL;
    assert(NULL != head);
    tmp = head->next;
    return_value = head->next->data;
    head->next = tmp->next;

    

    free(tmp);
    return return_value;
}

void *ProducerFunc(void *idx)
{
    static size_t i = 1;
    node_t *where = (node_t *)idx;
    
    pthread_mutex_lock(&lock);

    sb.sem_op = 1;
    semop(sem_id, &sb, 1);

    PushFront(where, (void *)i);
    ++i;

    pthread_mutex_unlock(&lock);
    
    return NULL;
}

void *ConsumerFunc(void *idx)
{
    node_t *where = (node_t *)idx;
    void *data = NULL;
 
    pthread_mutex_lock(&lock);
    do
    {
        pthread_mutex_unlock(&lock);
        pthread_yield();
        pthread_mutex_lock(&lock);

    } while (0 == semctl(sem_id, 0, GETVAL));

    sb.sem_op = -1;
    semop(sem_id, &sb, 1);

    data = PopFront(where);

    pthread_mutex_unlock(&lock);
    printf("data of pop :%ld\n", (long)data);

    return NULL;
}

int main()
{

    pthread_t producer[LIST_ELEMENTS] = {0};
    pthread_t consumer[LIST_ELEMENTS] = {0};
    int i = 0;
    key_t key = 0;
    

    node_t *head = malloc(sizeof(node_t));
    if (NULL == head)
    {
        return 1;
    }

    key = KeyCreate();

    sem_id = SemIdCreate(key);
    
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    head->next = NULL;

    while (LIST_ELEMENTS > i)
    {
        /* code */
        pthread_create(&producer[i], NULL, ProducerFunc, (void *)head);
        pthread_create(&consumer[i], NULL, ConsumerFunc, (void *)head);
        ++i;
        
    }

    i = 0;
    while (LIST_ELEMENTS > i)
    {
        pthread_join(producer[i], NULL);
        pthread_join(consumer[i], NULL);
        ++i;
    }
    free(head);
    head =  NULL;
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

static int SemIdCreate(key_t key)
{
    int sem_id = 0;
    union semun a;
    a.val = 1;
    sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (errno == EEXIST)
    {
        sem_id = semget(key, 1, 0);
    }
    else if (0 <= sem_id)
    {
        a.val = 1;
        if (-1 == semctl(sem_id, 0, SETVAL, a))
        {
            perror("semget");
            exit(1);
        }
    }
    return sem_id;
}
