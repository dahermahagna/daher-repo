/********************************************************************************
 *																				*
 *							Exercise: compareison Sorts							*
 *																				*
 *							Developer: Daher									*
 *																				*
 * 							Reviewer: Rafael								        *
 *																				*
 *							Special comments: none								*
 *																				*
 ********************************************************************************/
#include <assert.h>    /* assert */
#include <stdlib.h>    /* malloc, calloc, free*/
#include <stddef.h>    /* size_t */
#include <sys/types.h> /* ssize_t*/
#include <string.h>    /* memcpy */ 
#include <time.h>      /* time*/
#include "sorts.h"

/*------------------------MACRO---------------------------*/

#define MAX(x,y) (x) > (y) ? (x) : (y)
#define MIN(x,y) (x) < (y) ? (x) : (y)
#define LAST_ELEMENT_TO_CHECK (1)
#define ODD_NUM_ADJUSTMENT (num_of_elements & 1)
#define SIZE_OF_ARRAY (num_of_elements * size_of_element)

/*---------------FUNCTION DECLERATION---------------------*/

static void SwapInt(int *first, int *second);
static void VoidSwap(void *first, void*second, size_t size_in_bytes);
static int GetKey(const int base, size_t *param);
void CountingSort1(int arr[], size_t length, key_func_t get_key, size_t *param);
static int GetNumOfDigit(int num);
static void SwapIfNeed(int *arr1, size_t num_of_elements);
/* static void MergeBySize(int *arr1, int *arr2, size_t length); */
static int MergeBySize(int *arr1, int *arr2, size_t length);
void QuickSortByIndexes(void *base, void *first, void *last, size_t size, cmp_func_ty compare);
static size_t GetPivot(size_t num);

/*--------------------------------------------------------*/

void BubbleSort(int arr[], size_t length)
{
	size_t runner = 0;
	size_t sort_index = 0;
	assert(NULL != arr);

	while (length - 1 > runner)
	{
		sort_index = 0; 
		while (length - runner - 1 > sort_index)
		{
			if (arr[sort_index] > arr[sort_index + 1])
			{
				SwapInt(&arr[sort_index], &arr[sort_index + 1]);
			}

			++sort_index;
		}
		++runner;
	}
}
/*--------------------------------------------------------*/
void SelectionSort(int arr[], size_t length)
{
	size_t runner = 0;
	size_t min_index = 0;
	size_t sort_index = 0;

	assert(NULL != arr);

	while (length - 1 > sort_index)
	{
		runner = sort_index;
		min_index = sort_index;

		while (length > runner)
		{
			if (arr[runner] < arr[min_index])
			{
				min_index = runner;
			}
			++runner;
		}
		SwapInt(&arr[sort_index++], &arr[min_index]);
	}
} 
/*--------------------------------------------------------*/
void InsertionSort(int arr[], size_t length)
{
	size_t runner = 1;
	size_t sort_runner = 1;

	assert(NULL != arr);

	while (length > runner)
	{
		sort_runner = runner;
		while (0 < sort_runner && arr[sort_runner - 1] > arr[sort_runner])
		{
			SwapInt(&arr[sort_runner], &arr[sort_runner - 1]);
			--sort_runner;
		}
		++runner;
	}
} 
/*--------------------------------------------------------*/
void CountingSort(int arr[], size_t length)
{
	size_t digits = 0;
	CountingSort1(arr,length,GetKey,&digits);
}
/*--------------------------------------------------------*/
void RadixSort(int arr[], size_t length)
{
	int max = arr[0];
	int min = arr[0];
	int max_digits = 0;
	size_t runner = 0;
	size_t digits = 1;
	while (length > runner)
	{
		max = MAX(max, arr[runner]);
		min = MIN(min, arr[runner]);
		++runner;
	}

	max_digits = MAX(GetNumOfDigit(max),GetNumOfDigit(min));
	while(0 < max_digits)
	{
		CountingSort1(arr, length, GetKey, &digits);
		digits *= 10;
		--max_digits;
	}
	
	
}

/*---------------SERVICE FUNCTIONS---------------------*/
void CountingSort1(int arr[], size_t length, key_func_t get_key, size_t *param)
{
	ssize_t runner = 0;
	/*size_t sort_runner = 1;*/
	int max = GetKey(arr[0], param);
	int min = GetKey(arr[0], param);
	int offset = 0;
	int *digit_array;
	int *buffer; 
	int curr = 0;

	assert(NULL != arr);

	buffer = calloc(length ,sizeof(int));
	if(NULL == buffer)
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

	digit_array = calloc((max - min + 1),sizeof(int));
	if (NULL == digit_array)
	{
		free(buffer);
		buffer = NULL;
		return;
	}
	
	offset = - min;
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

	for (runner = length - 1; runner >= 0 ; runner --)
	{
		
		/* -- digit_array[get_key(buffer[runner], param)+offset]; */
		curr = get_key(buffer[runner], param) + offset;
		arr[digit_array[curr] - 1] = buffer[runner];
		digit_array[curr]--;
	}
	free(buffer);
	buffer = NULL;

	free(digit_array);
	digit_array = NULL;


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
    char *mid = (char *)arr + (SIZE_OF_ARRAY / 2) - (SIZE_OF_ARRAY / 2) % size_of_element;

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
    return BinarySearchRecursive(mid, cmp_func, data, (num_of_elements / 2) + ODD_NUM_ADJUSTMENT, size_of_element);
}


/* int MergeSort(int *arr_to_sort, size_t num_of_elements)
{

	if (num_of_elements <= 3)
	{
		SwapIfNeed(arr_to_sort, num_of_elements);
		return 1;
	}
	MergeSort(arr_to_sort, num_of_elements / 2);
	MergeSort(arr_to_sort + num_of_elements / 2, num_of_elements / 2 + ODD_NUM_ADJUSTMENT);
	MergeBySize(arr_to_sort, arr_to_sort + num_of_elements / 2, num_of_elements);
	
} */
static void SwapIfNeed(int *arr1, size_t num_of_elements)
{
	int tmp = 0;

	if (*arr1 > *(arr1 + 1))
	{
		tmp = *arr1;
		*arr1 = *(arr1 + 1);
		*(arr1 + 1) = tmp;
	}
	if (num_of_elements == 3)
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

int MergeSort(int *arr_to_sort, size_t num_of_elements)
{

	if (num_of_elements == 1)
	{
		return 1;
	}
	MergeSort(arr_to_sort, num_of_elements / 2);
	MergeSort(arr_to_sort + num_of_elements / 2, num_of_elements / 2 + ODD_NUM_ADJUSTMENT);
	MergeBySize(arr_to_sort, arr_to_sort + num_of_elements / 2, num_of_elements);
}

static int MergeBySize(int arr1[], int arr2[], size_t length)
{
	int *runner1 = NULL;
	int *runner2 = NULL;
	int *swap_runner = NULL;
	int container = 0;
	int *arr_of_swap = NULL;

	if (*arr2 > *(arr2 - 1))
	{
		return;
	}

	arr_of_swap = malloc(sizeof(int)*length);
	if(arr_of_swap == NULL)
	{
		return 1;
	}
	swap_runner = arr_of_swap;
	runner1 = arr1;
	runner2 = arr2;

	while (runner1 != arr2 && runner2 != arr1 + length)
	{
		if(*runner1 < *runner2)
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
	memcpy(arr1,arr_of_swap,length * sizeof(int));
	free(arr_of_swap);
	arr_of_swap = NULL;
	return 0;
	/* while (runner1 < runner2 && runner2 < arr1 + length)
	{
		if (*runner1 < *runner2)
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
	} */
}

/* static void MergeBySize(int *arr1, int *arr2, size_t length)
{
	int *runner1 = NULL;
	int *runner2 = NULL;
	int *back_runner = NULL;
	int container = 0;

	if (*arr2 > *(arr2 - 1))
	{
		return;
	}

	runner1 = arr1;
	runner2 = arr2;

	while (runner1 < runner2 && runner2 < arr1 + length)
	{
		if (*runner1 < *runner2)
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
} */

void QuickSort(void *base, size_t nmemb, size_t size, cmp_func_ty compare)
{
	/*partition*/
	QuickSortByIndexes(base, base, (char *)base + (nmemb - 1) * size, size, compare);
}
void QuickSortByIndexes(void *base, void *first, void *last, size_t size, cmp_func_ty compare)
{

	/* long  container = 0; */
	void *pivot = NULL;
	char *runner = NULL;
	char *back_runner = NULL;

	if (first < last)
	{
		/* pivot = GetPivot(last - first); */
		pivot = first;
		runner = (char*)first;
		back_runner = (char*)last;

		while (runner < back_runner)
		{
			while (runner < back_runner && 0 <= compare(pivot, (void *)runner))
			{
				runner += size;
			}

			while (0 < compare((void *)back_runner, pivot) && back_runner >= runner)
			{
				back_runner-= size;
			}

			if (runner < back_runner)
			{
				VoidSwap(runner,back_runner,size);
			}
		}
		VoidSwap(pivot, back_runner, size);
		QuickSortByIndexes(base, first, back_runner - size,size,compare);
		QuickSortByIndexes(base, back_runner + size,last, size,compare);
	}
}
size_t GetPivot(size_t num)
{
	size_t rnd_pivot = 0;
	long long_num = (long)num;
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

static void VoidSwap(void *first, void*second, size_t size_in_bytes) 
{
	void *tmp = NULL;
	memcpy(&tmp, (first), (size_in_bytes));
	memcpy((first), (second), (size_in_bytes));
	memcpy((second), &tmp, (size_in_bytes));
}

static int GetKey(int num, size_t *param)
{
	if (*param == 0)
	{
		return num;
	}
	num /= *param;
	return (num % 10);
}

static int GetNumOfDigit(int num)
{
	int count = 0;
	while(0 != num)
	{
		num /=10;
		++count;
	}
	return count;
}