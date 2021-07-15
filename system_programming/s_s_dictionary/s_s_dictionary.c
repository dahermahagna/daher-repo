/* ------------- INCLUDES ------------------*/
#include <sys/mman.h>  /*mmap              */
#include <stdio.h>     /*fprintf           */
#include <fcntl.h>     /*struct stat buf   */
#include <stdlib.h>    /*malloc free       */
#include <string.h>    /*strlen, strtok    */
#include <pthread.h>   /*thread_create     */
#include <stdatomic.h> /*atomic            */
#include <assert.h>    /*assert            */
#include <time.h>      /* now              */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/*----------------- MACROS -----------------*/

#define NUM_OF_COPIES (4)
#define NUM_OF_THREADS (5)
#define RANGE_OF_THREAD (number / NUM_OF_THREADS)
#define START_OF_RANGE (RANGE_OF_THREAD * index)
#define ASCII_RANGE (256)
#define LAST_THREAD_ADJ (last_thread_flag * ((lines_num * NUM_OF_COPIES) % NUM_OF_THREADS))
#define ODD_NUM_ADJUSTMENT (num_of_elements % 2)

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


/*---------------- STRUCTS -----------------*/
typedef struct mt_s_sort
{
    
    size_t thread_range;
    char **start_of_thread;
} mt_s_sort_t;

/*-------------- TYPEDEF --------------------*/

typedef int (*key_func_t)(int num, size_t *param);
/*----------- function decleration -------- */

static char **CreatArray(size_t length);
static char *GetDictionary(const char *fname, int *exit_status);
static size_t CountLines(char *dictionary);
static void SplitAndPoint(char **pointers_arr, char *dictionary);
static void PointersArrDuplicate(char **small_arr, char **big_Arr, size_t lines_num);
static void ShufflePointers(char **big_arr, size_t lines_num);

static int RndCmpFunc(const void * a, const void * b);
static int CmpFunc(const void *a, const void *b);

static void MtShuffleSorting(char **big_dictionary, size_t lines_num);
static void *ThreadSort(void *thread_idx);
static void MergeArr(char **big_dictionary, size_t lines_num);
static int MergeBySize(char *arr1[], char *arr2[], size_t length);
static void DestroyArray(char ** str);
static int MergeSort(char **arr_to_sort, size_t num_of_elements);
static void PrintBigDictionary(char **big_dictionary, size_t lines_num);
/* static void CountingSort(char *arr[], size_t length);
static void CountingSort1(char *arr[], size_t length, key_func_t get_key, size_t *param);
static int GetKey(int num, size_t *param);
 */
/*------------------- main -----------------*/
int main()
{
    size_t lines_num = 0;
    char **pointers_arr = NULL;
    char **big_pointers_arr = NULL;
    char *dictionary = NULL;
    int exit_status = 0;

    /*1) read the linux dictionary*/

    dictionary = GetDictionary("american-english", &exit_status);
    if (NULL == dictionary)
    {
        if (exit_status != MALLOC_ERR)
        {
            free(dictionary);
        }
        return exit_status;
    }
    
    /*2)creat array of pointers*/
    lines_num = CountLines(dictionary);
    pointers_arr = CreatArray(lines_num);
    if(NULL == pointers_arr)
    {
        return MALLOC_ERR;
    }
    big_pointers_arr = CreatArray(lines_num * NUM_OF_COPIES);
    if(NULL == big_pointers_arr)
    {
        DestroyArray(pointers_arr);
        return MALLOC_ERR;
    }
    SplitAndPoint(pointers_arr, dictionary);
    
    /*3) make several copies of pointers array*/
    PointersArrDuplicate(pointers_arr,big_pointers_arr,lines_num);
    
    /*4)shuffle the combined arrays*/
    ShufflePointers(big_pointers_arr,lines_num);

    /*5)sort threads segments*/
    MtShuffleSorting(big_pointers_arr, lines_num);

    /*6)merge the segments*/
    MergeArr(big_pointers_arr, lines_num);
    
    PrintBigDictionary(big_pointers_arr,lines_num);

    DestroyArray(pointers_arr);
    DestroyArray(big_pointers_arr);

    return 0;
}

static char **CreatArray(size_t length)
{
    char **dictionary = NULL;
    dictionary = malloc(length * sizeof(char*));
    return dictionary;
}
static char *GetDictionary(const char *fname, int *exit_status)
{
    unsigned char *dictionary = NULL;
    size_t len;
    struct stat buf;
    int fd = 0;
    assert(NULL != fname);

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

static size_t CountLines(char *dictionary)
{
    size_t count = 0;
    while('\0' != *dictionary)
    {
        count += *dictionary == '\n';
        ++dictionary;
    }
    return count + 1;
}

static void SplitAndPoint(char **pointers_arr, char *dictionary)
{
    char *token = NULL;

    assert(NULL != pointers_arr);
    assert(NULL != dictionary);

    token = strtok(dictionary,"\n");
    while(NULL != token)
    {
        *pointers_arr = token;
        token = strtok('\0', "\n");
        ++pointers_arr;
    }

}

static void PointersArrDuplicate(char **small_arr, char **big_Arr, size_t lines_num)
{
    size_t i = 0;

    assert(NULL != small_arr);
    assert(NULL != big_Arr);

    while (NUM_OF_COPIES > i)
    {
        memcpy(big_Arr + (i * lines_num),small_arr,lines_num * (sizeof(char*)));
        ++i;
    }
}

static void ShufflePointers(char **big_arr, size_t lines_num)
{
    qsort(big_arr,lines_num * NUM_OF_COPIES ,sizeof(char*),RndCmpFunc);
}

static int RndCmpFunc(const void * a, const void * b) 
{
    (void)a;
    (void)b;
    /* srand(time(0)); */
    return 10 -rand()%20;
}

static void MtShuffleSorting(char **big_dictionary, size_t lines_num)
{
    size_t i = 0;
    int last_thread_flag = 0;
    pthread_t thread[NUM_OF_THREADS] = {0};
    mt_s_sort_t strcut_arr[NUM_OF_THREADS];
    
    assert(NULL != big_dictionary);

    memset(strcut_arr, 0, sizeof(mt_s_sort_t) * NUM_OF_THREADS);

    for (i = 0; NUM_OF_THREADS > i; ++i)
    {
        last_thread_flag = NUM_OF_THREADS == (i + 1);
        strcut_arr[i].thread_range = (lines_num * NUM_OF_COPIES) / NUM_OF_THREADS + LAST_THREAD_ADJ;
        strcut_arr[i].start_of_thread = big_dictionary + i * (lines_num * NUM_OF_COPIES / NUM_OF_THREADS);
        while (0 != pthread_create(&thread[i], NULL, ThreadSort, (void *)&strcut_arr[i]));
    }
    

    for (i = 0; NUM_OF_THREADS > i; ++i)
    {
        pthread_join(thread[i], NULL);
    }
    /*can add the merge here !!*/
}

static void *ThreadSort(void *data)
{
    mt_s_sort_t *struct_pointer = NULL;

    assert(NULL != data);

    struct_pointer = (mt_s_sort_t *)data;
    /* qsort(struct_pointer->start_of_thread, struct_pointer->thread_range, sizeof(char *), CmpFunc); */
    MergeSort(struct_pointer->start_of_thread,struct_pointer->thread_range);
    /* CountingSort(struct_pointer->start_of_thread,struct_pointer->thread_range); */
    return NULL;
}

static int CmpFunc(const void *a, const void *b)
{

    return (int)(*(size_t*)a - *(size_t *)b);
}

static void MergeArr(char **big_dictionary, size_t lines_num)
{
    size_t i = 0;
    size_t length = 0;
    assert(NULL != big_dictionary);

    length = lines_num * NUM_OF_COPIES / NUM_OF_THREADS;

    for (i = 1;NUM_OF_THREADS - 1 > i; ++i)
    {
      if (1 == MergeBySize(big_dictionary, big_dictionary + length * i, length * (i + 1)))
      {
          return;
      }
    }
    if(1 == MergeBySize(big_dictionary, big_dictionary + length * i, lines_num * NUM_OF_COPIES))
    {
        return;
    }
}

static int MergeBySize(char *arr1[], char *arr2[], size_t length)
{
    char **runner1 = NULL;
    char **runner2 = NULL;
    char **swap_runner = NULL;
    char **arr_of_swap = NULL;

    if (*arr2 > *(arr2 - 1))
    {
        return 1;
    }

    arr_of_swap = malloc(sizeof(char*) * length);
    if (arr_of_swap == NULL)
    {
        return 1;
    }
    swap_runner = arr_of_swap;
    runner1 = arr1;
    runner2 = arr2;

    while (runner1 != arr2 && runner2 != arr1 + length)
    {
        if (*runner1 < *runner2)
        {
            *swap_runner = *runner1;
            ++runner1;
            ++swap_runner;
        }
        else
        {
            *swap_runner = *runner2;
            ++runner2;
            ++swap_runner;
        }
    }
    while (runner1 != arr2)
    {
        *swap_runner = *runner1;
        ++runner1;
        ++swap_runner;
    }
    while (runner2 != arr1 + length)
    {
        *swap_runner = *runner2;
        ++runner2;
        ++swap_runner;
    }
    memcpy(arr1, arr_of_swap, length * sizeof(char *));
    free(arr_of_swap);
    arr_of_swap = NULL;
    return 0;
    
}

static void DestroyArray(char ** str)
{
    free(str);
    str = NULL;
}

static int MergeSort(char **arr_to_sort, size_t num_of_elements)
{

    if (num_of_elements == 1)
    {
        return 1;
    }
    MergeSort(arr_to_sort, num_of_elements / 2);
    MergeSort(arr_to_sort + num_of_elements / 2, num_of_elements / 2 + ODD_NUM_ADJUSTMENT);
    MergeBySize(arr_to_sort, arr_to_sort + num_of_elements / 2, num_of_elements);
    return 0;
}

static void PrintBigDictionary(char **big_dictionary, size_t lines_num)
{
    size_t i = 0;
    assert(NULL != big_dictionary);

    for (i = 0; lines_num * NUM_OF_COPIES > i; ++i)
    {
        printf("%s\n", *(big_dictionary + i));
    }
}

/* static void CountingSort(char *arr[], size_t length)
{
    size_t digits = 0;
    CountingSort1(arr, length, GetKey, &digits);
}

static void CountingSort1(char *arr[], size_t length, key_func_t get_key, size_t *param)
{
    ssize_t runner = 0;

    int max = GetKey(arr[0], param);
    int min = GetKey(arr[0], param);
    int offset = 0;
    int *digit_array;
    int *buffer;
    int curr = 0;

    assert(NULL != arr);

    buffer = calloc(length, sizeof(int));
    if (NULL == buffer)
    {
        return;
    }
    while ((ssize_t)length > runner)
    {
        buffer[runner] = arr[runner];
        max = MAX(max, get_key(arr[runner], param));
        min = MIN(min, get_key(arr[runner], param));
        ++runner;
    }

    digit_array = calloc((max - min + 1), sizeof(int));
    if (NULL == digit_array)
    {
        free(buffer);
        buffer = NULL;
        return;
    }

    offset = -min;
    runner = 0;
    while ((ssize_t)length > runner)
    {
        ++digit_array[get_key(arr[runner], param) + offset];
        ++runner;
    }

    runner = 1;
    while ((ssize_t)max - min + 1 > runner)
    {
        digit_array[runner] += digit_array[runner - 1];
        ++runner;
    }

    for (runner = length - 1; runner >= 0; runner--)
    {

        curr = get_key(buffer[runner], param) + offset;
        arr[digit_array[curr] - 1] = buffer[runner];
        digit_array[curr]--;
    }
    free(buffer);
    buffer = NULL;

    free(digit_array);
    digit_array = NULL;
}

static int GetKey(int num, size_t *param)
{
    if (*param == 0)
    {
        return num;
    }
    num /= *param;
    return (num % 10);
} */