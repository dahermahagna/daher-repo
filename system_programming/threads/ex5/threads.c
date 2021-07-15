#include <stddef.h>    /*size_t*/
#include <stdio.h>     /*printf*/
#include <unistd.h>    /*sleep */
#include <pthread.h>   /*thread_create*/
#include <stdatomic.h> /*atomic*/

#define PROCESS_LIMIT (8)
#define RANGE_OF_THREAD (number / PROCESS_LIMIT)
#define START_OF_RANGE (RANGE_OF_THREAD * index + 1)

atomic_long sum_of_divisors_atomic = 0;
size_t number = 0;

static void OneHundredKArr(size_t number_check_div);
void *ThreadNumOfDivisors(void *thread_idx);

int main()
{
    OneHundredKArr(3200000000);
    printf("%ld\n", sum_of_divisors_atomic);
    /* size_t sum_of_divisors = 0;
    size_t i = 0;
    for (i = 1; i <= 3200000000; ++i)
    {
        if (0 == 3200000000 % i)
        {
            sum_of_divisors += i;
        }
    }

    printf("%ld", sum_of_divisors); */
    return 0;
}

static void OneHundredKArr(size_t number_check_div)
{
    size_t i = 0;
    size_t sum_for_now = 0;
    pthread_t thread[PROCESS_LIMIT] = {0};

    number = number_check_div;

    for (i = 0; i < PROCESS_LIMIT; ++i)
    {
        while (0 != pthread_create(&thread[i], NULL, ThreadNumOfDivisors, (void *)i))
        {
        }
    }
    for (i = 0; i < PROCESS_LIMIT; ++i)
    {
        pthread_join(thread[i], (void *)&sum_for_now);
        sum_of_divisors_atomic += sum_for_now;
    }
}

void *ThreadNumOfDivisors(void *thread_idx)
{
    size_t index = (size_t)thread_idx;
    size_t sum_of_divisors = 0;
    size_t i = 0;

    for (i = START_OF_RANGE; i <= START_OF_RANGE + RANGE_OF_THREAD; ++i)
    {
        if (0 == number % i)
        {
            sum_of_divisors += i;
        }
    }
    return (void *)sum_of_divisors;
}