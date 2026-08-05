#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>

/*------------------------MACRO---------------------------*/
#define MAX(x, y) (x) > (y) ? (x) : (y)
#define LAST_ELEMENT_TO_CHECK (1)
#define ODD_NUM_ADJUSTMENT (num_of_elements % 2)
#define SIZE_OF_ARRAY (num_of_elements * size_of_element)
enum result
{
    FOUND = 0,
    NOT_IN_THE_RANGE,
    LEFT_SIDE_OF_MID,
    RIGHT_SIDE_OF_MID
};
/*-----------------------TYPEDEF--------------------------*/

typedef int (*cmp_func_ty)(const void *data, const void *data_to_compare);
/*---------------FUNCTION DECLERATION---------------------*/

int* IterativeBinarySearch(int *arr, size_t length, int num);
int IsFound(int **first, int **mid, int **last, int num, int **where_found);
int IntCmpFunc(const void *data, const void *data_to_compare);
void *BinarySearchIterative(void *arr,  cmp_func_ty cmp_func, void *data,  size_t num_of_elements, size_t size_of_element);
void *BinarySearchRecursive(void *arr, cmp_func_ty cmp_func, void *data, size_t num_of_elements, size_t size_of_element);
void SwapIfNeed(int *num1, int *num2,size_t num_of_elements);
void MergBySize(int *arr1, int *arr2, size_t length);
/* void partition(int *arr, size_t first, size_t last); */
/* void partition(int *arr1, int first, int last); */
void partition(int *arr1, long first, long last);
size_t GetPivot(size_t first, size_t last);
static void SwapInt(int *first, int *second);
static void *BinSearchRecursive(char *arr, ssize_t left, ssize_t right,
                                void *data, size_t size_of_element, cmp_func_ty cmp_func);

void QuickSort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));
void QuickSortByIndexes(void *base, void *first, void *last, size_t size, int (*compar)(const void *, const void *));
static void VoidSwap(void *first, void *second, size_t size_in_bytes);

/*-----------------------STRUCTS--------------------------*/

int main()
{
    int *res = NULL;
    void * res1 = NULL;
    void * res2 = NULL;
    int check = 1233;
    int arr[9] = {1,3,5,67,98,122,123,1233,12222};
    int arr1[10] = {500, 3, 2, 4, 100, 7, 89, 0, 7, 6};
    int arr2[10] = {500, 3, 2, 4, 100, 7, 89, 0, 7, 6};
    int i = 0;
    res = IterativeBinarySearch(arr,9,check);
    NULL != res ? printf("%d\n",*res) : printf("there is no elemnt in arr\n");
    res1 = BinarySearchIterative(arr,IntCmpFunc,(void*)&check,9,4);
    NULL != res1 ? printf("%d\n", *(int*)res1) : printf("there is no elemnt in arr\n");
    res2 = BinarySearchRecursive(arr, IntCmpFunc, (void *)check, 9, 4);
    NULL != res2 ? printf("%d\n", *(int*)res2) : printf("there is no elemnt in arr\n");
    MergeSort(arr1,10);
    QuickSort(arr2, 10, 4, IntCmpFunc);
    /* QuickSort1(arr2, 10); */
    printf("\n\n");
    while (i < 10)
    {
        /* printf("%d\n",arr1[i]); */
        printf("%d\n", arr2[i++]);
    }
    


    return 0;
}


void *BinarySearchIterative(void *arr,  cmp_func_ty cmp_func, void *data,  size_t num_of_elements, size_t size_of_element)
{
    char *first = NULL;
    char *mid = NULL;
    char *last = NULL;

    first = (char *)arr;
    last = (char *)arr + SIZE_OF_ARRAY;
    mid = (char *)arr + (SIZE_OF_ARRAY / 2) - (SIZE_OF_ARRAY / 2) % size_of_element;

    while ( 0 != cmp_func((mid),data) &&  first < mid)
    {
        if(0 < cmp_func(mid,data))
        {
            last = mid;
            
        }
        else
        {
            first = mid;
        }
        mid = last - ((last - first) / 2) - ((last - first) / 2) % size_of_element;
    }
    if (0 != cmp_func(data, mid))
    {
        return NULL;
    }
    return (void *)mid;
}

void *BinarySearchRecursive(void *arr, cmp_func_ty cmp_func, void *data, size_t num_of_elements, size_t size_of_element)
{
    /* char *mid = (char *)arr + (SIZE_OF_ARRAY / 2) - (SIZE_OF_ARRAY / 2) % size_of_element;

    if (0 == cmp_func(mid, data))
    {
        return mid;
    }
    if(LAST_ELEMENT_TO_CHECK == num_of_elements)
    {
        return NULL;
    }
    if (0 < cmp_func(mid, data))
    {
        return BinarySearchRecursive(arr, cmp_func, data, (num_of_elements / 2) + ODD_NUM_ADJUSTMENT , size_of_element);
    }
    return BinarySearchRecursive(mid, cmp_func, data, (num_of_elements / 2) + ODD_NUM_ADJUSTMENT, size_of_element); */
    return BinSearchRecursive((char *)arr, 0, num_of_elements - 1, data,
                              size_of_element, cmp_func);
}



int* IterativeBinarySearch(int *arr, size_t length, int num) /* API FUNCTION*/
{
    int *first = NULL;
    int *mid = NULL;
    int *last = NULL;
    int treg = LEFT_SIDE_OF_MID;
    int *found = NULL;
    assert(NULL != arr);

    first = arr;
    last = arr + length - 1;
    mid = arr + length / 2;
    while((LEFT_SIDE_OF_MID == treg || RIGHT_SIDE_OF_MID == treg) && NOT_IN_THE_RANGE != treg)
    {
        treg = IsFound(&first,&mid,&last,num,&found);
    }
    if(FOUND == treg)
    {
        return found;
    }
    return NULL;

}
int IsFound(int **first, int **mid, int **last, int num, int **where_found) /* return  0 if found, 1 if( num > * last || num < first), 2 if in the left side of mid, 3 if in the right side of mid*/ 
{
    if(**first == num)
    {
        *where_found = *first;
        return FOUND;
    }
    if (**mid == num)
    {
        *where_found = *mid;
        return FOUND;
    }
    if (**last == num)
    {
        *where_found = *last;
        return FOUND;
    }

    if(num < **first || num > **last)
    {
        return NOT_IN_THE_RANGE;
    }

    if (num < **mid)
    {
        *last = *mid - 1;
        *mid = *last - ((*last - *first) / 2);
        return LEFT_SIDE_OF_MID;
    }

    if(num > **mid)
    {
        *first = *mid + 1;
        *mid += (*last - *first) / 2; 
        return RIGHT_SIDE_OF_MID;
    }
    return 0;
}

int IntCmpFunc(const void *data, const void *data_to_compare)
{
    return *(int*)data - *(int *)data_to_compare;
}



int MergeSort(int *arr_to_sort, size_t num_of_elements)
{
    int *mid = arr_to_sort + (num_of_elements / 2) + ODD_NUM_ADJUSTMENT;
    
    if(num_of_elements <= 3)
    {
        SwapIfNeed(arr_to_sort,mid,num_of_elements);
        return 1;        
    }
    MergeSort(arr_to_sort, num_of_elements / 2 );
    MergeSort(arr_to_sort + num_of_elements / 2 , num_of_elements / 2 + ODD_NUM_ADJUSTMENT);
    MergBySize(arr_to_sort, arr_to_sort + num_of_elements / 2 , num_of_elements);
}
void SwapIfNeed(int *arr1, int *num2,size_t num_of_elements)
{
    int tmp = 0;
    
    if(*arr1 > *(arr1 + 1))
    {
        tmp = *arr1;
        *arr1 = *(arr1 + 1);
        *(arr1 + 1) = tmp;
    }
    if(num_of_elements == 3)
    {
        if (*(arr1 + 1) > *(arr1 + 2))
        {
            tmp = *(arr1 + 1);
            *(arr1 + 1) = *(arr1 + 2);
            *(arr1 + 2) = tmp;
        }
        if (*arr1 > *(arr1 + 1))
        {
            tmp = *arr1;
            *arr1 = *(arr1 + 1);
            *(arr1 + 1) = tmp;
        }
    }
    

}
void MergBySize(int *arr1, int *arr2, size_t length)
{
    int *runner1 = NULL;
    int *runner2 = NULL;
    int *back_runner = NULL;
    int container = 0;

    if (*arr2 > *(arr2-1))
    {
        return;
    }

    runner1 = arr1;
    runner2 = arr2;

    while (runner1 <= runner2 && runner2 < arr1 + length)
    {
        if(*runner1 < *runner2)
        {
            ++runner1;
        }
        else
        {
            container = *runner2;
            back_runner = runner2;
            while (back_runner > runner1)
            {
                *back_runner = *(back_runner - 1);  
                --back_runner;
            }
            *runner1 = container;
            ++runner1;
            ++runner2;
        }
        
    }
    
}

void QuickSort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
   
    QuickSortByIndexes(base, base, (char *)base + (nmemb - 1) * size, size, compar);
}
void QuickSortByIndexes(void *base, void *first, void *last, size_t size, int (*compar)(const void *, const void *))
{

    long container = 0;
    void *rnd_pivot = NULL;
    char *runner = NULL;
    char *back_runner = NULL;
    char *end_of_arr = NULL;
    char *first_of_arr = NULL;
    if (first < last)
    {
        
        rnd_pivot = first;
        runner = (char *)first;
        back_runner = (char *)last;
        end_of_arr = (char *)last;
        first_of_arr = (char *)first;

        while (runner < back_runner)
        {

            while (runner < end_of_arr && 0 <= compar(rnd_pivot, (void *)runner))
            {
                runner += size;
            }

            while (0 < compar((void *)back_runner, rnd_pivot) && back_runner >= first_of_arr)
            {
                back_runner -= size;
            }

            if (runner < back_runner)
            {
                VoidSwap(runner, back_runner, size);
            }
        }

        VoidSwap(rnd_pivot, back_runner, size);
        QuickSortByIndexes(base, first, back_runner - size, size, compar);
        QuickSortByIndexes(base, back_runner + size, last, size, compar);
    }
}

static void VoidSwap(void *first, void *second, size_t size_in_bytes)
{
    int *tmp = 0;
    memcpy(&tmp, (first), (size_in_bytes));
    memcpy((first), (second), (size_in_bytes));
    memcpy((second), &tmp, (size_in_bytes));
}

int QuickSort1(int *arr_to_sort, size_t num_of_elements)
{
    
    partition(arr_to_sort,0,num_of_elements - 1);

}
void partition(int *arr1, long first, long last)
{
   
    long rnd_pivot = 0, runner = 0, back_runner = 0, container = 0;
    if (first < last)
    {
        
        rnd_pivot = first;
        runner = first;
        back_runner = last;
        while (runner < back_runner)
        {
           
            while (arr1[runner] <= arr1[rnd_pivot] && runner <= last)
            {
                runner++;
            }
            
            while (arr1[back_runner] > arr1[rnd_pivot] && back_runner >= first)
            {
                back_runner--;
            }
            
            if (runner < back_runner)
            {
                SwapInt(&arr1[runner],&arr1[back_runner]);
            }
        }

        SwapInt(&arr1[rnd_pivot], &arr1[back_runner]);

        partition(arr1, first, back_runner - 1);
        partition(arr1, back_runner + 1, last);
    }
}
size_t GetPivot(size_t first, size_t last)
{
    size_t rnd_pivot = 0;
    long long_num = (long)last - first;
    srand(time(0));
    rnd_pivot = rand() % (long_num);
    return long_num <= 0 ? 0 : rnd_pivot;
} 
static void SwapInt(int *first, int *second)
{
    int tmp = *first;
    *first = *second;
    *second = tmp;
}

static void *BinSearchRecursive(char *arr1, ssize_t left, ssize_t right,
                                void *data, size_t size_of_element, cmp_func_ty cmp_func) /* GOOD */ 
{
    ssize_t mid = 0;
    int comper_res = 0;

    assert(NULL != arr1);

    if (left > right)
    {
        return NULL;
    }

    mid = (left + right) / 2;
    comper_res = cmp_func(&data, (char *)arr1 + (mid * size_of_element));
    if (0 == comper_res)
    {
        return (char *)arr1 + (mid * size_of_element);
    }

    if (0 < comper_res)
    {
        return BinSearchRecursive(arr1, mid + 1, right, data, size_of_element, cmp_func);
    }
    else
    {
        return BinSearchRecursive(arr1, left, mid - 1, data, size_of_element, cmp_func);
    }
}