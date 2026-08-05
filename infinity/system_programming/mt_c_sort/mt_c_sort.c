/* ------------- INCLUDES ------------------*/
#include <sys/mman.h>   /*mmap              */
#include <stdio.h>      /*fprintf           */
#include <fcntl.h>      /*struct stat buf   */
#include <stdlib.h>     /*malloc free       */
#include <string.h>     /*strlen            */
#include <pthread.h>    /*thread_create     */
#include <stdatomic.h>  /*atomic            */
#include <assert.h>     /*assert            */    
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/*----------------- MACROS -----------------*/

#define NUM_OF_COPIES (1000)
#define NUM_OF_THREADS (4)
#define RANGE_OF_THREAD (number / NUM_OF_THREADS)
#define START_OF_RANGE (RANGE_OF_THREAD * index)
#define ASCII_RANGE (256)

/*-------------------ENUM ------------------*/

enum handling_errors
{
    SUCCESS = 0,
    MALLOC_ERR,
    DICT_OPEN_ERR,
    DICT_FSTAT_ERR,
    DICT_MMAP_ERR,
    THREAD_CREATE,
    THREAD_JOIN
};
/*---------------- GLOBALS -----------------*/

atomic_long arr[ASCII_RANGE] = {0};
size_t number = 0;

/*---------------- STRUCTS -----------------*/
typedef struct mt_c_sort
{
    size_t index;
    char *big_dictionary;
    size_t letter_count[ASCII_RANGE];
} mt_c_sort_t;



/*----------- function decleration -------- */

static void MT_C_Sorting(char *big_dic);
static void *ThreadLettersCounter(void *thread_idx);

static char *GetDictionary(const char *fname, size_t *size, int *exit_status);
static char *GetBigDictionary(const char *fname, int *exit_status);
static void PrintLettersCounters(void);

    /*------------------- main -----------------*/
    int main()
{
    char *big_dictionary = NULL;
    int exit_status = 0;
    big_dictionary = GetBigDictionary("american-english",&exit_status);
    if(NULL == big_dictionary)
    {
        if(exit_status != MALLOC_ERR)
        {
            free(big_dictionary);
        }
        return exit_status;
    }
    MT_C_Sorting(big_dictionary);
    PrintLettersCounters();
    free(big_dictionary);
    big_dictionary = NULL;

    return 0;
}

/*------------ SERVICE FUNCTIONS -----------*/

static void MT_C_Sorting(char *big_dictionary)
{
    int i = 0, j = 0;
    pthread_t thread[NUM_OF_THREADS] = {0};

    mt_c_sort_t mt1[NUM_OF_THREADS];
    mt_c_sort_t *return_mt = {0};
    memset(mt1, 0, sizeof(mt_c_sort_t) * NUM_OF_THREADS);

    number = strlen(big_dictionary);

    for (i = 0; NUM_OF_THREADS > (i); ++i)
    {
        mt1[i].big_dictionary = big_dictionary;
        mt1[i].index = i;
        while (0 != pthread_create(&thread[i], NULL, ThreadLettersCounter, (void *)&mt1[i]));
    }

    for (i = 0; NUM_OF_THREADS > i; ++i)
    {

        pthread_join(thread[i], (void *)&return_mt);
        for (j = 0; ASCII_RANGE > j; ++j)
        {
            arr[j] += return_mt->letter_count[j];
        }
    }
}

static void *ThreadLettersCounter(void *data)
{
    mt_c_sort_t *mt1 = NULL;
    size_t index = 0;
    size_t i = 0;

    assert(NULL != data);
    mt1 = (mt_c_sort_t *)data;

    index = mt1->index;

    for (i = START_OF_RANGE; START_OF_RANGE + RANGE_OF_THREAD > i; ++i)
    {  
         
        ++mt1->letter_count[(int)mt1->big_dictionary[i]];
    }
    return data;
}


static char *GetBigDictionary(const char *fname, int *exit_status)
{
    size_t i = 0;
    size_t size = 0;
    char *dictionary = NULL;
    char *dictionary_big_data = NULL;

    dictionary = GetDictionary(fname, &size,exit_status);
    if(NULL == dictionary)
    {
        return NULL;
    }
    
    dictionary_big_data = malloc(size * NUM_OF_COPIES);
    if(NULL == dictionary_big_data)
    {
        *exit_status = MALLOC_ERR;
        return NULL;
    }

    for (i = 0; NUM_OF_COPIES > i; ++i)
    {
        memcpy((void *)(dictionary_big_data + (i * size)), dictionary, size);
    }
    return dictionary_big_data;
}

static char *GetDictionary(const char *fname, size_t *size, int *exit_status)
{
    unsigned char *dictionary = NULL;
    size_t len;
    struct stat buf;
    int fd = 0;
    assert(NULL != fname);
    assert(NULL != size);

    fd = open(fname, O_RDONLY);
    if (0 > fd)
    {
        fprintf(stderr, "Error: Unable to read dictionary file %s\n", fname);
        close(fd);
        *exit_status = DICT_OPEN_ERR;
        return NULL;
    }

    if (0 > fstat(fd, &buf))
    {
        fprintf(stderr, "Error: Unable to determine file size\n");
        close(fd);
        *exit_status = DICT_FSTAT_ERR;
        return NULL;
    }

    len = buf.st_size;
    *size = len;
    dictionary = (unsigned char *)mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == dictionary)
    {
        fprintf(stderr, "Error: Unable to memory map dictionary!\n");
        close(fd);
        *exit_status = DICT_MMAP_ERR;
        return NULL;
    }
    close(fd);
    return (char *)dictionary;
}

static void PrintLettersCounters(void)
{
    size_t i = 0;
    size_t j = 0;

    for (i = 'A', j = 'a'; 'Z' >= i && 'z' >= j; ++i, ++j)
    {
        printf("[%c] - %07ld , [%c] - %08ld \n", (char)i, arr[i], (char)j, arr[j]);
    }
}
