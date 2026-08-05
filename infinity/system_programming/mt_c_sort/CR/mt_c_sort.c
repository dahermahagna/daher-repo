/* ------------- INCLUDES ------------------*/
#include <sys/mman.h>  /*mmap              */
#include <stdio.h>     /*fprintf           */
#include <fcntl.h>     /*struct stat buf   */
#include <stdlib.h>    /*malloc free       */
#include <string.h>    /*strlen            */
#include <pthread.h>   /*thread_create     */
#include <stdatomic.h> /*atomic            */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/*---------------- GLOBALS -----------------*/

atomic_long arr[256] = {0}; /*define for 256*/
size_t number = 0;
/*---------------- STRUCTS -----------------*/
typedef struct mt_c_sort
{
    size_t index;
    char *big_dictionary;
    size_t letter_count[256];/*define for 256*/
} mt_c_sort_t;

/*----------------- MACROS -----------------*/

#define NUM_OF_COPIES (1000)
#define NUM_OF_THREADS (8)
#define RANGE_OF_THREAD (number / NUM_OF_THREADS)
#define START_OF_RANGE ((RANGE_OF_THREAD * index + 1)*!!(index))

/*----------- function decleration -------- */

static void MT_C_Sorting(char *big_dic);
static void *ThreadLettersCounter(void *thread_idx);
static char *GetDictionary(const char *fname);
static char *GetBigDictionary(const char *fname);

/*------------------- main -----------------*/
int main()
{
    char *big_dictionary = NULL;
    int i = 0;
    big_dictionary = GetBigDictionary("american-english");

    MT_C_Sorting(big_dictionary);

    /*make a function print results*/
    for (i = 0; 256 > i; ++i)
    {
        if (0 != arr[i])
            printf("%d: %ld\n", i, arr[i]);
    }
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

    for (i = 0; NUM_OF_THREADS > i; ++i)
    {

        mt1[i].big_dictionary = big_dictionary;
        mt1[i].index = i;

        while (0 != pthread_create(&thread[i], NULL, ThreadLettersCounter, (void *)&mt1[i]))
            ;
    }
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {

        pthread_join(thread[i], (void *)&return_mt);
        for (j = 0; 256 > j; ++j)
        {
            arr[j] += return_mt->letter_count[j];
        }
    }
}

static void *ThreadLettersCounter(void *data)
{
    /*assert for data*/
    mt_c_sort_t *mt1 = (mt_c_sort_t *)data;
    size_t index = 0;
    size_t i = 0;
    size_t number = 0;

    number = strlen(mt1->big_dictionary);/*why to do it every time?*/
    index = mt1->index;

    /*coding convention*/
    for (i = START_OF_RANGE; i <= START_OF_RANGE + RANGE_OF_THREAD; ++i)
    {   
            ++mt1->letter_count[(int)mt1->big_dictionary[i]];
    }
    return data;
}

static char *GetBigDictionary(const char *fname)
{
    /*assert for fname*/
    size_t i = 0;
    size_t size = 0;
    char *dictionary = NULL;
    char *dictionary_big_data = NULL;

    dictionary = GetDictionary(fname); /*what about to check if it's fails*/
    size = strlen(dictionary); /*this is redundant you can use len = buf.st_size in GetDictionary it will be faster */
    dictionary_big_data = malloc(size * NUM_OF_COPIES); /*what about to check if malloc failed?*/

    for (i = 0; i < NUM_OF_COPIES; ++i) /*coding convention*/
    {
        memcpy((void *)(dictionary_big_data + (i * size)), dictionary, size);
    }
    return dictionary_big_data;
}

static char *GetDictionary(const char *fname)
{
    /*assert for fname*/
    unsigned char *dictionary;/*uninitialized*/
    size_t len; /*uninitialized*/
    struct stat buf;
    int fd = open(fname, O_RDONLY);
    if (0 > fd)/*if it failed you should to close the file */
    {
        fprintf(stderr, "Error: Unable to read dictionary file %s\n", fname);
        return NULL;
    }

    if (0 > fstat(fd, &buf)) /*if it failed you should to close the file */
    {
        fprintf(stderr, "Error: Unable to determine file size\n");
        return NULL;
    }

    len = buf.st_size;
    dictionary = (unsigned char *)mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == dictionary)/*if it failed you should to close the file */
    {
        fprintf(stderr, "Error: Unable to memory map dictionary!\n");
        return NULL;
    }
    return (char *)dictionary;
}