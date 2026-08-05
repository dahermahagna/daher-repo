/******************************************************************************			 
 *                 Exercise: avl                                               *
 *                 Developer: Eli                                             *
 *                 Reviwer:                                                   *
 *                 Date:                                                      *
 ******************************************************************************/

/******************************************************************************
 *                                 HEADERS                                    * 
 ******************************************************************************/
#include <stdio.h>         /* printf   */
#include <assert.h>        /* assert   */

#include "avl.h"
#include "utilities.h"

/******************************************************************************
 *                       TYPE DEFINITIONS & STRUCTS                           *
 ******************************************************************************/
enum traversal
{
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER,
    NUM_OF_TRAVERSAL
};
/******************************************************************************
 *                          FUNCTION DECLARATIONS                             *
 ******************************************************************************/
static void FirstTest(void);
static void SecondTest(void);
static void ThirdTest(void);
static void FourthTest(void);
static void FifthTest(void);
static void SixthTest(void);


int AVLComparison(const void *data, const void *data_to_compare);
int AvlFillArrayInOrder(const void *data, void *param);
static int IsSortedImp(int *arr, size_t size);
/******************************************************************************
 *                          FUNCTIONS DEFINITIONS                             *
 ******************************************************************************/

/**************************** main function *************************/
int main()
{
	FirstTest();
	SecondTest();
	ThirdTest();
	FourthTest();
	FifthTest();
	SixthTest();

	return 0;
}
int PtintTree(const void *data, void *param)
{
	(void)param;
	printf("the value of this element is:%d\n", *(int *)data);
	return 0;
}
/**************************** test functions ************************/
/* test for: */
static void FirstTest(void)
{
	AVL_ty *tree = AVLCreate(AVLComparison);

	int arr[20] = {20, 10, 30, 5, 15, 4, 3, 6, 12, 13, 
					17, 25, 27, 26, 40, 35, 41, 46, 44, 45};
	int i = 0;

	printf("\033[1;36mFIRST TEST:\033[0m\n");

	TEST((NULL != tree), AVLCreate #1);
	TEST((1 == AVLIsEmpty(tree)), AVLCreate #2);
	TEST((0 == AVLSize(tree)), AVLCreate #3);

	for (i = 0; i < 3; ++i)
	{
		AVLInsert(tree, arr + i);
	}
	TEST((2 == AVLHeight(tree)), AVLCreate #4);
	putchar('\n');

	TEST((0 == AVLIsEmpty(tree)), AVLIsEmpty);
	TEST((3 == AVLSize(tree)), AVLSize);
	TEST((1 == AVLHeight(tree)), AVLHeight);

	AVLDestroy(tree);
	putchar('\n');
}


static void SecondTest(void)
{
	AVL_ty *tree = AVLCreate(AVLComparison);

	int arr[20] = {20, 10, 30, 5, 15, 4, 3, 6, 12, 13, 
					17, 25, 27, 26, 40, 35, 41, 46, 44, 45};
	int i = 0;

	printf("\033[1;36mSECOND TEST:\033[0m\n");

	for (i = 0; i < 20; ++i)
	{
		AVLInsert(tree, arr + i);
	}

	TEST((20 == AVLSize(tree)), AVLSize);
	TEST((4 == AVLHeight(tree)), AVLHeight);
	printf("height: %ld\n", AVLHeight(tree));

	AVLDestroy(tree);
	putchar('\n');
}


static void ThirdTest(void)
{
	AVL_ty *tree = AVLCreate(AVLComparison);

	int arr[20] = {20, 10, 30, 5, 15, 4, 3, 6, 12, 13, 
					17, 25, 27, 26, 40, 35, 41, 46, 44, 45};
	int i = 0;

	printf("\033[1;36mTHIRD TEST:\033[0m\n");

	for (i = 0; i < 20; ++i)
	{
		AVLInsert(tree, arr + i);
	}

	TEST((arr[0] == *(int *)AVLFind(tree, arr + 0)), AVLFind #1);
	TEST((arr[13] == *(int *)AVLFind(tree, arr + 13)), AVLFind #2);
	TEST((arr[19] == *(int *)AVLFind(tree, arr + 19)), AVLFind #3);

	AVLDestroy(tree);
	putchar('\n');
}


static void FourthTest(void)
{
	AVL_ty *tree = AVLCreate(AVLComparison);

	int arr[20] = {20, 10, 30, 5, 15, 4, 3, 6, 12, 13, 
					17, 25, 27, 26, 40, 35, 41, 46, 44, 45};
	int i = 0;

	printf("\033[1;36mFOURTH TEST:\033[0m\n");

	for (i = 0; i < 20; ++i)
	{
		AVLInsert(tree, arr + i);
	}
	AVLForEach(tree,PtintTree,NULL,2);
	AVLRemove(tree, arr + 19);
	TEST((19 == AVLSize(tree)), AVLRemove #1);
	TEST((4 == AVLHeight(tree)), AVLRemove #2);

	AVLRemove(tree, arr + 17);
	TEST((18 == AVLSize(tree)), AVLRemove #3);
	TEST((4 == AVLHeight(tree)), AVLRemove #4);

	AVLRemove(tree, arr + 0);
	TEST((17 == AVLSize(tree)), AVLRemove #5);

	for (i = 0; i < 5; ++i)
	{
		AVLRemove(tree, arr + i);
	}

	AVLDestroy(tree);
	putchar('\n');
}


static void FifthTest(void)
{
	AVL_ty *tree = AVLCreate(AVLComparison);

	int arr[20] = {20, 10, 30, 5, 15, 4, 3, 6, 12, 13, 
					17, 25, 27, 26, 40, 35, 41, 46, 44, 45};
	int test_arr[20] = {0};
	int i = 0;

	printf("\033[1;36mFIFTH TEST:\033[0m\n");

	for (i = 0; i < 20; ++i)
	{
		AVLInsert(tree, arr + i);
	}

	AVLForEach(tree, AvlFillArrayInOrder, test_arr, IN_ORDER);

	TEST((1 == IsSortedImp(test_arr, 20)), ForEach);

	AVLDestroy(tree);
	putchar('\n');
}


static void SixthTest(void)
{
	AVL_ty *tree = AVLCreate(AVLComparison);

	int arr[128] = {0};
	int i = 0;

	printf("\033[1;36mSIXTH TEST:\033[0m\n");

	for (i = 0; i < 128; ++i)
	{
		arr[i] = i;
	}

	for (i = 0; i < 15; ++i)
	{
		AVLInsert(tree, arr + i);
	}
	TEST((3 == AVLHeight(tree)), Balance);

	for (i = 15; i < 31; ++i)
	{
		AVLInsert(tree, arr + i);
	}
	TEST((4 == AVLHeight(tree)), Balance);

	for (i = 31; i < 63; ++i)
	{
		AVLInsert(tree, arr + i);
	}
	TEST((5 == AVLHeight(tree)), Balance);

	for (i = 63; i < 127; ++i)
	{
		AVLInsert(tree, arr + i);
	}
	TEST((6 == AVLHeight(tree)), Balance);

	AVLDestroy(tree);
	putchar('\n');
}






/******************************************************************************
 *                          AUXILIARY FUNCTIONS                               * 
 ******************************************************************************/
/**************************** AVLComparison *************************/
int AVLComparison(const void *data, const void *data_to_compare)
{
	assert(NULL != data);
	assert(NULL != data_to_compare);

	return *(int *)data - *(int *)data_to_compare;
}

/**************************** AvlFillArrayInOrder *******************/
int AvlFillArrayInOrder(const void *data, void *param)
{
	static size_t i = 0;
	int *arr = (int *)param;

	arr[i] = *(int *)data;

	++i;

	return 0;
}

/**************************** IsSortedImp ***************************/
static int IsSortedImp(int *arr, size_t size)
{
	int flag = 1;
	size_t i = 0;

	for (i = 0; i < size - 1; ++i)
	{
		flag *= (arr[i] <= arr[i + 1]);
	}

	return flag;
}


