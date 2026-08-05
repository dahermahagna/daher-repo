#include "../utils/test.h"
#include "heap.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);
static test_status_t CaseTwoTest(void);
static test_status_t CaseThreeTest(void);
static test_status_t CaseFourTest(void);
static test_status_t CaseFiveTest(void);
static test_status_t CaseSixTest(void);
static test_status_t CaseSevenTest(void);

int CmpFun(const void *data, const void *data_to_compare);
/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(CaseOneTest);
	RUNTEST(CaseTwoTest);
	RUNTEST(CaseThreeTest);
	RUNTEST(CaseFourTest);
	RUNTEST(CaseFiveTest);
	RUNTEST(CaseSixTest);
	RUNTEST(CaseSevenTest);

	return PASSED; 
}

/*--------------------------------------------------------*/
 /*you have to arrange the functions in the appropriate stage test */
/*
FUNC_TO_TEST(HeapCreate);
FUNC_TO_TEST(HeapDestroy);
FUNC_TO_TEST(HeapPop);
FUNC_TO_TEST(HeapPush);
FUNC_TO_TEST(HeapPeek);
FUNC_TO_TEST(HeapRemove);
FUNC_TO_TEST(HeapIsEmpty);
FUNC_TO_TEST(HeapSize);
*/
/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{
	Heap_ty *user_heap = HeapCreate(CmpFun);
	FUNC_TO_TEST(HeapCreate->HeapPush->HeapPeek->HeapDestroy);

	REQUIRE(NULL != user_heap);
	HeapPush(user_heap, (void*)5);
	REQUIRE((void*)5 == HeapPeek(user_heap));
	HeapPush(user_heap, (void *)6);
	REQUIRE((void *)6 == HeapPeek(user_heap));
	HeapPush(user_heap, (void *)4);
	REQUIRE((void *)6 == HeapPeek(user_heap));
	HeapPush(user_heap, (void *)7);
	REQUIRE((void *)7 == HeapPeek(user_heap));
	HeapPush(user_heap, (void *)8);
	REQUIRE((void *)8 == HeapPeek(user_heap));
	HeapRemove(user_heap,CmpFun, (void *)8);
	REQUIRE((void *)7 == HeapPeek(user_heap));
	REQUIRE(NULL == HeapRemove(user_heap, CmpFun, (void *)20));
	REQUIRE((void *)7 == HeapPeek(user_heap));
	HeapPush(user_heap, (void *)10);
	REQUIRE((void *)10 == HeapPeek(user_heap));
	HeapPop(user_heap);
	REQUIRE((void *)7 == HeapPeek(user_heap));
	HeapPop(user_heap);
	REQUIRE((void *)6 == HeapPeek(user_heap));
	HeapPop(user_heap);
	REQUIRE((void *)5 == HeapPeek(user_heap));
	HeapPop(user_heap);
	REQUIRE((void *)4 == HeapPeek(user_heap));

	HeapPop(user_heap);
	REQUIRE(0 == HeapSize(user_heap));

	HeapDestroy(user_heap);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTwoTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseThreeTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFourTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFiveTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseSixTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseSevenTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}
int CmpFun(const void *data, const void *data_to_compare)
{
	return *(int *)&data - *(int *)&data_to_compare;
}