#include <stdio.h>
#include <pthread.h> /* thread */
#include <semaphore.h>
#include <stdatomic.h>
#include <stdlib.h>  /* malloc */
#include <sys/sem.h> /* struct sembuf, SETALL, GETVAL, SEM_UNDO, IPC_CREAT */

#define CONSUMERS_NUM (10)

static void *ConsumerFunc();
static void *ProducerFunc();

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
sem_t sem;
atomic_int message = 0;

static void *ProducerFunc()
{
    size_t i = 0;
    size_t count = 10;

    while (count--)
    {
        for (i = 0; i < CONSUMERS_NUM; ++i)
        {
            sem_wait(&sem);
        }

        pthread_mutex_lock(&mutex);
        ++message;
        puts("Sending message...\n");
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

static void *ConsumerFunc()
{
    int local_msg = 0;
    size_t count = 10;

    while (count--)
    {
        pthread_mutex_lock(&mutex);
        sem_post(&sem);
        while (local_msg == message)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Message received: %d\n", message);
        pthread_mutex_unlock(&mutex);
        local_msg = message;
    }

    return NULL;
}

int main()
{
    size_t i = 0;
    pthread_t consumer[CONSUMERS_NUM] = {0};
    pthread_t producer = 0;

    sem_init(&sem, 0, 0);
    while (0 != pthread_mutex_init(&mutex, NULL));

    while (0 != pthread_create(&producer, NULL, ProducerFunc, NULL))
        ;

    while (CONSUMERS_NUM > i)
    {
        while (0 != pthread_create(&consumer[i], NULL, ConsumerFunc, NULL))
            ;
        ++i;
    }
    i = 0;
    pthread_join(producer, NULL);
    while (CONSUMERS_NUM > i)
    {
        pthread_join(consumer[i], NULL);
        ++i;
    }

    sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);

    return (0);
}