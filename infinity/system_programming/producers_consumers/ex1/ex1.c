#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h> /*threads*/

#define ARR_SIZE (100)
#define MAX_IDX (15)

atomic_int arr[ARR_SIZE];

atomic_int flag = 0;

void UpdateArr()
{
    size_t i = 0;
    static size_t num = 0;

    while(ARR_SIZE > i)
    {
        arr[i] = num;
        ++i;
    }
    ++num;
}

void SumPrint()
{
    size_t i = 0;
    size_t sum = 0;

    while(ARR_SIZE > i)
    {
        sum += arr[i];
        ++i;
    }
    printf("%ld\n",sum);
}

void *ProducerFunc(void *idx)
{
    int i = 0;
    (void)idx;

    while(MAX_IDX > i)
    {
        while (1 == flag);
        UpdateArr();
        ++i;
        flag = 1;
    }
    return NULL;
}

void *ConsumerFunc(void *idx)
{
    int i = 0;
    (void)idx;

    while (MAX_IDX > i)
    {
        while (0 == flag);
        SumPrint();
        ++i;
        flag = 0;
    }
    return NULL;
}

int main()
{

    pthread_t producer = 0;
    pthread_t consumer = 0;
    
    pthread_create(&producer, NULL, ProducerFunc, NULL);
    pthread_create(&consumer, NULL, ConsumerFunc, NULL);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    
    return 0;
}