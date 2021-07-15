#include "../utils/test.h"
#include "../include/bst.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);
static test_status_t CaseTwoTest(void);
static test_status_t CaseThreeTest(void);
static test_status_t CaseFourTest(void);
static test_status_t CaseFiveTest(void);
static test_status_t CaseSixTest(void);

int CmpFunc(const void *data, const void *data_to_compare, void *params);
int PtintTree(const void *data, void *param);

	/*-------------------------MAIN---------------------------*/

	int main()
{
	RUNTEST(CaseOneTest);
	RUNTEST(CaseTwoTest);
	RUNTEST(CaseThreeTest);
	RUNTEST(CaseFourTest);
	RUNTEST(CaseFiveTest);
	RUNTEST(CaseSixTest);

	return PASSED; 
}

/*--------------------------------------------------------*/
 /*you have to arrange the functions in the appropriate stage test */
/*

FUNC_TO_TEST(BSTRemove);

FUNC_TO_TEST(BSTForEach);

FUNC_TO_TEST(BSTBegin);
FUNC_TO_TEST(BSTEnd);
FUNC_TO_TEST(BSTGetData);
FUNC_TO_TEST(BSTIsSameIter);
*/
/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{
	BST_ty *user_bts = NULL;

	FUNC_TO_TEST(BSTCreate);
	FUNC_TO_TEST(BSTDestroy);

	user_bts = BSTCreate(CmpFunc,NULL);
	REQUIRE(NULL != user_bts);

	BSTDestroy(user_bts);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTwoTest(void)
{
	BST_ty *user_bts = NULL;

	FUNC_TO_TEST(BSTCreate);
	FUNC_TO_TEST(BSTInsert);
	FUNC_TO_TEST(BSTDestroy);
	FUNC_TO_TEST(BSTIsEmpty);

	user_bts = BSTCreate(CmpFunc, NULL);
	REQUIRE(NULL != user_bts);
	BSTInsert(user_bts, (void*)5);
	BSTInsert(user_bts, (void *)4);
	BSTInsert(user_bts, (void *)6);
	BSTInsert(user_bts, (void *)3);
	BSTInsert(user_bts, (void *)7);

	
	REQUIRE(0 == BSTIsEmpty(user_bts));
	BSTDestroy(user_bts);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseThreeTest(void)
{
	BST_ty *user_bts = NULL;
	BST_iter_ty runner = NULL;

	FUNC_TO_TEST(BSTCreate);
	FUNC_TO_TEST(BSTBegin);
	FUNC_TO_TEST(BSTInsert);
	FUNC_TO_TEST(BSTGetData);
	FUNC_TO_TEST(BSTDestroy);
	FUNC_TO_TEST(BSTIterNext);
	FUNC_TO_TEST(BSTIterPrev);

	user_bts = BSTCreate(CmpFunc, NULL);
	REQUIRE(NULL != user_bts);
	BSTInsert(user_bts, (void *)5);
	BSTInsert(user_bts, (void *)4);
	BSTInsert(user_bts, (void *)6);
	BSTInsert(user_bts, (void *)3);
	BSTInsert(user_bts, (void *)7);

	runner = BSTBegin(user_bts);
	REQUIRE((void*)3 == BSTGetData(runner));
	runner = BSTIterNext(runner);
	REQUIRE((void *)4 == BSTGetData(runner));
	runner = BSTIterNext(runner);
	REQUIRE((void *)5 == BSTGetData(runner));
	runner = BSTIterNext(runner);
	REQUIRE((void *)6 == BSTGetData(runner));
	runner = BSTIterNext(runner);
	REQUIRE((void *)7 == BSTGetData(runner));
	runner = BSTIterPrev(runner);
	REQUIRE((void *)6 == BSTGetData(runner));
	runner = BSTIterPrev(runner);
	REQUIRE((void *)5 == BSTGetData(runner));
	runner = BSTIterPrev(runner);
	REQUIRE((void *)4 == BSTGetData(runner));
	runner = BSTIterPrev(runner);
	REQUIRE((void *)3 == BSTGetData(runner));


	BSTDestroy(user_bts);


	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFourTest(void)
{
	BST_ty *user_bts = NULL;
	BST_iter_ty runner = NULL;

	FUNC_TO_TEST(BSTCreate);
	FUNC_TO_TEST(BSTInsert);
	FUNC_TO_TEST(BSTSize);
	FUNC_TO_TEST(BSTSearch);
	FUNC_TO_TEST(BSTDestroy);


	user_bts = BSTCreate(CmpFunc, NULL);
	REQUIRE(NULL != user_bts);

	BSTInsert(user_bts, (void *)5);
	REQUIRE(1 == BSTSize(user_bts));
	BSTInsert(user_bts, (void *)4);
	REQUIRE(2 == BSTSize(user_bts));
	BSTInsert(user_bts, (void *)6);
	REQUIRE(3 == BSTSize(user_bts));
	BSTInsert(user_bts, (void *)3);
	REQUIRE(4 == BSTSize(user_bts));
	BSTInsert(user_bts, (void *)7);
	REQUIRE(5 == BSTSize(user_bts));
	BSTInsert(user_bts, (void *)100);
	REQUIRE(6 == BSTSize(user_bts));
	BSTInsert(user_bts, (void *)2);
	REQUIRE(7 == BSTSize(user_bts));
	BSTInsert(user_bts, (void *)35);
	REQUIRE(8 == BSTSize(user_bts));

	runner = BSTSearch(user_bts, (void*)35);
	REQUIRE((void *)35 == BSTGetData(runner));
	
	BSTDestroy(user_bts);


	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFiveTest(void)
{
	BST_ty *user_bts = NULL;

	FUNC_TO_TEST(BSTCreate);
	FUNC_TO_TEST(BSTInsert);
	FUNC_TO_TEST(BSTForEach);
	FUNC_TO_TEST(BSTDestroy);

	user_bts = BSTCreate(CmpFunc, NULL);
	REQUIRE(NULL != user_bts);

	BSTInsert(user_bts, (void *)5);
	BSTInsert(user_bts, (void *)4);
	BSTInsert(user_bts, (void *)6);
	BSTInsert(user_bts, (void *)3);
	BSTInsert(user_bts, (void *)7);
	BSTInsert(user_bts, (void *)100);
	BSTInsert(user_bts, (void *)2);
	BSTInsert(user_bts, (void *)35);

	BSTForEach(BSTBegin(user_bts), BSTEnd(user_bts), PtintTree,NULL);
	

	BSTDestroy(user_bts);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseSixTest(void)
{
	BST_ty *user_bts = NULL;
	BST_iter_ty runner = NULL;

	FUNC_TO_TEST(BSTCreate);
	FUNC_TO_TEST(BSTInsert);
	
	FUNC_TO_TEST(BSTDestroy);

	user_bts = BSTCreate(CmpFunc, NULL);
	REQUIRE(NULL != user_bts);

	BSTInsert(user_bts, (void *)5);
	BSTInsert(user_bts, (void *)3);
	BSTInsert(user_bts, (void *)4);
	BSTInsert(user_bts, (void *)2);
	BSTInsert(user_bts, (void *)6);
	BSTInsert(user_bts, (void *)7);
	BSTInsert(user_bts, (void *)100);
	BSTInsert(user_bts, (void *)35);

	BSTForEach(BSTBegin(user_bts), BSTEnd(user_bts), PtintTree, NULL);
	printf("\n");
	runner = BSTSearch(user_bts,(void*)7);
	REQUIRE((void*)7 == BSTRemove(runner));
	runner = BSTSearch(user_bts, (void *)3);
	REQUIRE((void *)3 == BSTRemove(runner));
	runner = BSTSearch(user_bts, (void *)35);
	REQUIRE((void *)35 == BSTRemove(runner));

	BSTForEach(BSTBegin(user_bts), BSTEnd(user_bts), PtintTree, NULL);

	BSTDestroy(user_bts);

	return PASSED;
}

/*--------------------------------------------------------*/


int CmpFunc(const void *data, const void *data_to_compare, void *params)
{
	(void) params;
	return *(int *)&data - *(int *)&data_to_compare;
}

int PtintTree(const void *data, void *param)
{
	(void) param;
	printf("the value of this element is:%d\n", *(int*)&data);
	return 0;
}