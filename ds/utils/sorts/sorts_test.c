#include <stdlib.h> /*rand, qsort*/
#include <time.h>	/*time, clock*/


#include "../test.h"
#include "sorts.h"



/*------------------------MACRO---------------------------*/
#define ARR_LENGTH (5000)

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t BubbleSortTest(void);
static test_status_t SelectionSortTest(void);
static test_status_t InsertionSortTest(void);
static test_status_t CountingSortTest(void);
static test_status_t RadixSortTest(void);
static test_status_t BinarySearchTest(void);
static test_status_t MergeSortTest(void);
static test_status_t QuickSortTest(void);

	static int IsSorted(int arr[], size_t length);
void PrintFirsthundred(int arr[]);
int CmpFunc(const void *a, const void *b);
static void CheckTimeForSortion(void);

/*-------------------------MAIN---------------------------*/

	int main()
{
	RUNTEST(BubbleSortTest);
	RUNTEST(SelectionSortTest);
	RUNTEST(InsertionSortTest);
	RUNTEST(CountingSortTest);
	RUNTEST(RadixSortTest);
	RUNTEST(MergeSortTest);
	RUNTEST(QuickSortTest);
	RUNTEST(BinarySearchTest);

	CheckTimeForSortion();

	return PASSED;
}

/*--------------------------------------------------------*/

 /*you have to arrange the functions in the appropriate stage test */
/*


*/

static test_status_t BubbleSortTest(void)
{
	int arr_mysort[ARR_LENGTH] = {0};
	int arr_qsort[ARR_LENGTH] = {0};
	int runner = 0;
	clock_t start, end;
	FUNC_TO_TEST(BubbleSortTest);
	srand(time(0));
	while (ARR_LENGTH > runner)
	{
		arr_mysort[runner] = rand() % 300;
		arr_qsort[runner] = arr_mysort[runner];
		++runner;
	}
	REQUIRE(0 == IsSorted(arr_mysort, ARR_LENGTH));
	start = clock();
	BubbleSort(arr_mysort, ARR_LENGTH);
	end = clock();
	printf("\ntime for BubbleSort: %ld",end - start );

		qsort(arr_qsort, ARR_LENGTH, sizeof(int), CmpFunc);
	runner = 0;
	while (ARR_LENGTH > runner)
	{
		REQUIRE(arr_qsort[runner] == arr_mysort[runner]);
		++runner;
	}

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SelectionSortTest(void)
{
	int arr_mysort[ARR_LENGTH] = {0};
	int arr_qsort[ARR_LENGTH] = {0};
	int runner = 0;
	clock_t start, end;
	FUNC_TO_TEST(SelectionSort);
	srand(time(0));
	while (ARR_LENGTH > runner)
	{
		arr_mysort[runner] = rand() % 300;
		arr_qsort[runner] = arr_mysort[runner];
		++runner;
	}
	REQUIRE(0 == IsSorted(arr_mysort, ARR_LENGTH));
	start = clock();
	SelectionSort(arr_mysort, ARR_LENGTH);
	end = clock();
	printf("\ntime for SelectionSort: %ld", end - start);

	qsort(arr_qsort, ARR_LENGTH, sizeof(int), CmpFunc);
	runner = 0;
	while (ARR_LENGTH > runner)
	{
		REQUIRE(arr_qsort[runner] == arr_mysort[runner]);
		++runner;
	}

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t InsertionSortTest(void)
{
	int arr_mysort[ARR_LENGTH] = {0};
	int arr_qsort[ARR_LENGTH] = {0};
	int runner = 0;
	clock_t start, end;
	FUNC_TO_TEST(InsertionSort);
	srand(time(0));
	while (ARR_LENGTH > runner)
	{
		arr_mysort[runner] = rand() % 300;
		arr_qsort[runner] = arr_mysort[runner];
		++runner;
	}
	REQUIRE(0 == IsSorted(arr_mysort, ARR_LENGTH));
	start = clock();
	InsertionSort(arr_mysort, ARR_LENGTH);
	end = clock();
	printf("\ntime for InsertionSort: %ld", end - start);

	qsort(arr_qsort, ARR_LENGTH, sizeof(int), CmpFunc);
	runner = 0;
	while (ARR_LENGTH > runner)
	{
		REQUIRE(arr_qsort[runner] == arr_mysort[runner]);
		++runner;
	}

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t CountingSortTest(void)
{
	int arr_mysort[ARR_LENGTH] = {0};
	int arr_qsort[ARR_LENGTH] = {0};
	int runner = 0;
	clock_t start, end;
	FUNC_TO_TEST(CountingSort);
	srand(time(0));
	while (ARR_LENGTH > runner)
	{
		arr_mysort[runner] = rand() % 300 - 400;
		arr_qsort[runner] = arr_mysort[runner];
		++runner;
	}
	REQUIRE(0 == IsSorted(arr_mysort, ARR_LENGTH));
	start = clock();
	
	CountingSort(arr_mysort, ARR_LENGTH);
	
	end = clock();
	printf("\ntime for CountingSort: %ld", end - start);

	qsort(arr_qsort, ARR_LENGTH, sizeof(int), CmpFunc);
	runner = 0;
	while (ARR_LENGTH > runner)
	{
		REQUIRE(arr_qsort[runner] == arr_mysort[runner]);
		++runner;
	}

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t RadixSortTest(void)
{
	int arr_mysort[ARR_LENGTH] = {0};
	int arr_qsort[ARR_LENGTH] = {0};
	int runner = 0;
	clock_t start, end;
	FUNC_TO_TEST(RadixSort);
	srand(time(0));
	while (ARR_LENGTH > runner)
	{
		arr_mysort[runner] = ((rand() % 100));
		arr_qsort[runner] = arr_mysort[runner];
		++runner;
	}
	REQUIRE(0 == IsSorted(arr_mysort, ARR_LENGTH));
	start = clock();

	RadixSort(arr_mysort, ARR_LENGTH);

	end = clock();
	printf("\ntime for RadixSort: %ld", end - start);

	qsort(arr_qsort, ARR_LENGTH, sizeof(int), CmpFunc);
	runner = 0;
	while (ARR_LENGTH > runner)
	{
		REQUIRE(arr_qsort[runner] == arr_mysort[runner]);
		++runner;
	}

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t MergeSortTest(void)
{
	int arr_mysort[ARR_LENGTH] = {0};
	int arr_qsort[ARR_LENGTH] = {0};
	int runner = 0;
	clock_t start, end;
	FUNC_TO_TEST(MergeSort);
	srand(time(0));
	while (ARR_LENGTH > runner)
	{
		arr_mysort[runner] = ((rand() % 100));
		arr_qsort[runner] = arr_mysort[runner];
		++runner;
	}
	REQUIRE(0 == IsSorted(arr_mysort, ARR_LENGTH));
	start = clock();

	MergeSort(arr_mysort, ARR_LENGTH);

	end = clock();
	printf("\ntime for MergeSort: %ld", end - start);

	qsort(arr_qsort, ARR_LENGTH, sizeof(int), CmpFunc);
	runner = 0;
	while (ARR_LENGTH > runner)
	{
		REQUIRE(arr_qsort[runner] == arr_mysort[runner]);
		++runner;
	}

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t QuickSortTest(void)
{
	int arr_mysort[ARR_LENGTH] = {0};
	int arr_qsort[ARR_LENGTH] = {0};
	int runner = 0;
	clock_t start, end;
	FUNC_TO_TEST(QuickSort);
	srand(time(0));
	while (ARR_LENGTH > runner)
	{
		arr_mysort[runner] = ((rand() % 100));
		arr_qsort[runner] = arr_mysort[runner];
		++runner;
	}
	/* REQUIRE(0 == IsSorted(arr_mysort, ARR_LENGTH)); */
	start = clock();

	QuickSort(arr_mysort,ARR_LENGTH,4,CmpFunc);

	end = clock();
	printf("\ntime for QuickSort: %ld", end - start);

	qsort(arr_qsort, ARR_LENGTH, sizeof(int), CmpFunc);
	runner = 0;
	while (ARR_LENGTH > runner)
	{
		REQUIRE(arr_qsort[runner] == arr_mysort[runner]);
		++runner;
	}

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t BinarySearchTest(void)
{
    void * res1 = NULL;
    void * res2 = NULL;
    int check1 = 1;
    int check2 = 0;
    int check3 = 5;
    int check4 = 7;
    int arr1[1] = {1};
    int arr2[] = {1,2,3,4,5,6,7};
    res1 = BinarySearchIterative(arr1,CmpFunc,(void*)&check1,1,4);
    res2 = BinarySearchRecursive(arr1,CmpFunc,(void*)&check1,1,4);

    REQUIRE(1 == *(int*)res1);
    REQUIRE(1 == *(int*)res2);
    REQUIRE(NULL == BinarySearchIterative(arr1,CmpFunc,(void*)&check2,1,4));
    REQUIRE(NULL == BinarySearchRecursive(arr1,CmpFunc,(void*)&check2,1,4));	
   	res1 = BinarySearchIterative(arr2,CmpFunc,(void*)&check1,7,4);
    res2 = BinarySearchRecursive(arr2,CmpFunc,(void*)&check1,7,4);
    REQUIRE(1 == *(int*)res1);
    REQUIRE(1 == *(int*)res2);
    res1 = BinarySearchIterative(arr2,CmpFunc,(void*)&check3,7,4);
    res2 = BinarySearchRecursive(arr2,CmpFunc,(void*)&check3,7,4);
    REQUIRE(5 == *(int*)res1);
    REQUIRE(5 == *(int*)res2);
    res1 = BinarySearchIterative(arr2,CmpFunc,(void*)&check4,7,4);
    res2 = BinarySearchRecursive(arr2,CmpFunc,(void*)&check4,7,4);
    REQUIRE(7 == *(int*)res1);
    REQUIRE(7 == *(int*)res2);


    return PASSED;
}



static void CheckTimeForSortion(void)
{
	int arr_bubble[ARR_LENGTH] = {0};
	int arr_selection[ARR_LENGTH] = {0};
	int arr_insertion[ARR_LENGTH] = {0};
	int arr_counting[ARR_LENGTH] = {0};
	int arr_radix[ARR_LENGTH] = {0};
	int arr_merge[ARR_LENGTH] = {0};
	int arr_quick[ARR_LENGTH] = {0};
	int arr_qsort[ARR_LENGTH] = {0};

	int runner = 0;
	clock_t start, end;
	FUNC_TO_TEST(InsertionSort);
	srand(time(0));
	while (ARR_LENGTH > runner)
	{
		arr_bubble[runner] = rand() % 100000;
		arr_selection[runner] = arr_bubble[runner];
		arr_insertion[runner] = arr_bubble[runner];
		arr_counting[runner] = arr_bubble[runner];
		arr_radix[runner] = arr_bubble[runner];
		arr_merge[runner] = arr_bubble[runner];
		arr_quick[runner] = arr_bubble[runner];

		arr_qsort[runner] = arr_bubble[runner];

		++ runner;
	}
	start = clock();
	BubbleSort(arr_bubble, ARR_LENGTH);
	end = clock();
	printf("\ntime for BubbleSort: %ld", end - start);
	
	start = clock();
	SelectionSort(arr_selection, ARR_LENGTH);
	end = clock();
	printf("\ntime for SelectionSort: %ld", end - start);

	start = clock();
	InsertionSort(arr_insertion, ARR_LENGTH);
	end = clock();
	printf("\ntime for InsertionSort: %ld", end - start);

	start = clock();
	CountingSort(arr_counting, ARR_LENGTH);
	end = clock();
	printf("\ntime for CountingSort: %ld", end - start);

	start = clock();
	RadixSort(arr_radix, ARR_LENGTH);
	end = clock();
	printf("\ntime for RadixSort: %ld", end - start);

	start = clock();
	MergeSort(arr_merge, ARR_LENGTH);
	end = clock();
	printf("\ntime for MergeSort: %ld", end - start);

	start = clock();
	QuickSort(arr_quick, ARR_LENGTH, 4, CmpFunc);
	end = clock();
	printf("\ntime for QuickSort: %ld", end - start);

	start = clock();
	qsort(arr_qsort, ARR_LENGTH, sizeof(int), CmpFunc);
	end = clock();
	printf("\ntime for qsort: %ld\n", end - start);

}

static int IsSorted(int arr[], size_t length)
{
	size_t runner = 0;
	while (length > runner + 1)
	{
		if (arr[runner] > arr[runner + 1])
		{
			return 0;
		}
		++runner;
	}
	return 1;
}

void PrintFirsthundred(int arr[])
{
	int i = 0;
	while(ARR_LENGTH > i)
	{
		/*if (i % 100 == 0)
		{*/
			printf("%d ", arr[i]);
		/*}
		if (i % 10 == 0 && i > 0)
		{
			printf("\n");
		}
		*/
		++i;
	}
	printf("\n\n\n\n\n\n\n\n");
}


int CmpFunc(const void *data, const void *data_to_compare)
{
    return *(int*)data - *(int *)data_to_compare;
}