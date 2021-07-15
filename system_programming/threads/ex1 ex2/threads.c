#include <pthread.h> /*threads*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 

#define SIZE_OF_ARR (100000)
int arr[SIZE_OF_ARR] = {0};

static void *UpdateTheArrayByIndex(void *index)
{
    arr[(int)index] = (int)index;
    
    return NULL;
}

static void *PrintTheArrayByIndex(void *index)
{
    if (0 == (int)index % 25)
    {
        printf(" \n");
    }
    
    printf("%05d ", arr[(int)index]);
      
    return NULL;
}

int main()
{
    int i = 0;
    /* pthread_t thread[SIZE_OF_ARR] = {0}; */
    pthread_t thread = 0;
    time_t start = 0, end = 0;
    start = time(NULL);
    while (SIZE_OF_ARR > i)
    {
        /* while(0 !=  */pthread_create(&thread,NULL, UpdateTheArrayByIndex, (void *)i);/* ) */
        {}
        /* pthread_join(thread,NULL); */
        ++i;
    }
    i = 0;
    end = time(NULL);
    /* printf("%ld\n", thread[i-2]); */
    /* sleep(10); */
        pthread_join(thread, NULL);
    while (SIZE_OF_ARR > i)
    {
        /* PrintTheArrayByIndex((void *)i); */
        pthread_create(&thread,NULL, PrintTheArrayByIndex, (void *)i);
        pthread_join(thread, NULL);

        ++i;
    }
    printf("\n");
    printf("\nexecuting time is -> %ld\n",end - start);
    /* printf("%d\n", arr[9999]); */
    return 0;
}
