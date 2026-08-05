#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h> /*threads*/
#include <stdlib.h>  /*malloc*/
#include <assert.h>  /*assert*/
#include <unistd.h> /*sleep */

#define _GNU_SOURCE
#define LIST_ELEMENTS (10)
int pthread_yield(void);
atomic_long index = 0;
atomic_long counter = 0;

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

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
    tmp = NULL;

    return return_value;
}

void *ProducerFunc(void *idx)
{
    static size_t i = 1;
    node_t *where = (node_t *)idx;
    
    pthread_mutex_lock(&lock);
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

    } while (NULL == where->next);
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

    node_t *head = malloc(sizeof(node_t));
    if (NULL == head)
    {
        return 1;
    }
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    head->next = NULL;

    while (LIST_ELEMENTS > i)
    {
        while(0 != pthread_create(&producer[i], NULL, ProducerFunc, (void *)head));
        while(0 != pthread_create(&consumer[i], NULL, ConsumerFunc, (void *)head));
        ++i;
    }

    i = 0;
    while (LIST_ELEMENTS > i)
    {
        pthread_join(producer[i], NULL);
        pthread_join(consumer[i], NULL);
        ++i;
    }
    pthread_mutex_destroy(&lock);
    free(head);
    head = NULL;
    return 0;
}