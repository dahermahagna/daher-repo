#include "../utils/test.h"
#include "../include/avl.h"



/*------------------------MACRO---------------------------*/

typedef enum traverser
{
	PRE_ORDER = 1,
	In_ORDER,
	POST_ORDER
} traverser;

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);
static test_status_t CaseTwoTest(void);
static test_status_t CaseThreeTest(void);
static test_status_t CaseFourTest(void);
static test_status_t CaseFiveTest(void);
static test_status_t CaseSixTest(void);
static test_status_t CaseSevenTest(void);

int CmpFunc(const void *data, const void *data_to_compare);
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
	RUNTEST(CaseSevenTest);

	return PASSED;
}

/*--------------------------------------------------------*/
 /*you have to arrange the functions in the appropriate stage test */
/*


FUNC_TO_TEST(AVLCreate);
FUNC_TO_TEST(AVLDestroy);
FUNC_TO_TEST(AVLFind);
FUNC_TO_TEST(AVLInsert);
FUNC_TO_TEST(AVLRemove);
FUNC_TO_TEST(AVLIsEmpty);
FUNC_TO_TEST(AVLSize);
FUNC_TO_TEST(AVLHeight);
FUNC_TO_TEST(AVLForEach);
*/
/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{
	AVL_ty *user_avl = NULL;
	FUNC_TO_TEST(AVLCreate);

	user_avl = AVLCreate(CmpFunc);
	
	REQUIRE(NULL != user_avl);
	AVLDestroy(user_avl);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTwoTest(void)
{
	AVL_ty *user_avl = NULL;
	FUNC_TO_TEST(AVLCreate);

	user_avl = AVLCreate(CmpFunc);
	AVLInsert(user_avl, (void *)5);
	AVLInsert(user_avl, (void *)7);
	AVLInsert(user_avl, (void *)6);
	AVLInsert(user_avl, (void *)4);
	AVLInsert(user_avl, (void *)3);
	AVLInsert(user_avl, (void *)8);
	AVLInsert(user_avl, (void *)2);
	AVLInsert(user_avl, (void *)1);

	print2D(user_avl);
	printf("\n------------------------\n");
	printH(user_avl);

	AVLRemove(user_avl, (void *)1);
	REQUIRE(NULL == AVLFind(user_avl, (void *)1));
	printf("\n\n\n\n");
	print2D(user_avl);
	printf("\n------------------------\n");
	printH(user_avl);

	AVLRemove(user_avl, (void *)8);
	REQUIRE(NULL == AVLFind(user_avl, (void *)8));
	printf("\n\n\n\n");
	print2D(user_avl);
	printf("\n------------------------\n");
	printH(user_avl);

	AVLRemove(user_avl, (void *)4);
	REQUIRE(NULL == AVLFind(user_avl, (void *)4));

	printf("\n\n\n\n");
	print2D(user_avl);
	printf("\n------------------------\n");
	printH(user_avl);

	AVLRemove(user_avl, (void *)2);
	REQUIRE(NULL == AVLFind(user_avl, (void *)2));

	

	REQUIRE(NULL != user_avl);
	AVLDestroy(user_avl);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseThreeTest(void)
{
	AVL_ty *user_avl = NULL;
	FUNC_TO_TEST(AVLCreate->AVLInsert->AVLForEach->AVLDestroy);

	user_avl = AVLCreate(CmpFunc);
	AVLInsert(user_avl, (void *)5);
	AVLInsert(user_avl, (void *)6);
	AVLInsert(user_avl, (void *)4);
	AVLInsert(user_avl, (void *)3);
	AVLInsert(user_avl, (void *)8);
	AVLInsert(user_avl, (void *)1);
	AVLInsert(user_avl, (void *)7);
	AVLInsert(user_avl, (void *)2);
	printf("\n");
	AVLForEach(user_avl, PtintTree, NULL, PRE_ORDER);
	printf("\n");
	AVLForEach(user_avl, PtintTree, NULL, In_ORDER);
	printf("\n");
	AVLForEach(user_avl, PtintTree, NULL, POST_ORDER);

	AVLRemove(user_avl,(void*)6);
	REQUIRE(NULL != user_avl);
	AVLDestroy(user_avl);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFourTest(void)
{
	AVL_ty *user_avl = NULL;
	FUNC_TO_TEST(AVLCreate->AVLInsert->AVLSize->AVLHeight->AVLDestroy);

	user_avl = AVLCreate(CmpFunc);
	REQUIRE(0 == AVLHeight(user_avl));
	AVLInsert(user_avl, (void *)5);
	REQUIRE(1 == AVLHeight(user_avl));
	AVLInsert(user_avl, (void *)6);
	REQUIRE(2 == AVLHeight(user_avl));
	AVLInsert(user_avl, (void *)4);
	REQUIRE(2 == AVLHeight(user_avl));
	AVLInsert(user_avl, (void *)3);
	REQUIRE(3 == AVLHeight(user_avl));
	AVLInsert(user_avl, (void *)8);
	REQUIRE(3 == AVLHeight(user_avl));
	AVLInsert(user_avl, (void *)1);
	REQUIRE(3 == AVLHeight(user_avl));
	AVLInsert(user_avl, (void *)7);
	REQUIRE(3 == AVLHeight(user_avl));
	AVLInsert(user_avl, (void *)2);
	REQUIRE(8 == AVLSize(user_avl));

	REQUIRE(4 == AVLHeight(user_avl));
	AVLDestroy(user_avl);
	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseFiveTest(void)
{
	
	AVL_ty *user_avl = NULL;
	FUNC_TO_TEST(AVLCreate->AVLInsert->AVLHeight->AVLDestroy);

	user_avl = AVLCreate(CmpFunc);
	AVLInsert(user_avl, (void *)5);
	AVLInsert(user_avl, (void *)6);
	AVLInsert(user_avl, (void *)4);
	AVLInsert(user_avl, (void *)3);
	AVLInsert(user_avl, (void *)8);
	AVLInsert(user_avl, (void *)1);
	AVLInsert(user_avl, (void *)7);
	AVLInsert(user_avl, (void *)2);
	REQUIRE((void *)5 == AVLFind(user_avl,(void *)5));
	REQUIRE((void *)4 == AVLFind(user_avl, (void *)4));

	REQUIRE(NULL == AVLFind(user_avl, (void *)9));

	AVLDestroy(user_avl);
	return PASSED;
	
}
static test_status_t CaseSixTest(void)
{

	AVL_ty *user_avl = NULL;
	FUNC_TO_TEST(AVLCreate->AVLInsert->AVLHeight->AVLDestroy);

	user_avl = AVLCreate(CmpFunc);
	AVLInsert(user_avl, (void *)3);
	AVLInsert(user_avl, (void *)2);
	AVLInsert(user_avl, (void *)1);
  	AVLInsert(user_avl, (void *)8);
	AVLInsert(user_avl, (void *)10);
	AVLInsert(user_avl, (void *)4); 
	AVLInsert(user_avl, (void *)5);
	AVLInsert(user_avl, (void *)100);
	AVLInsert(user_avl, (void *)20);


	print2D(user_avl);
	printf("\n------------------------\n");
	printH(user_avl);

	AVLForEach(user_avl, PtintTree, NULL, PRE_ORDER);
	/*	AVLInsert(user_avl, (void *)1);
	AVLInsert(user_avl, (void *)7);
	AVLInsert(user_avl, (void *)2); */

	REQUIRE((void *)3 == AVLFind(user_avl, (void *)3));


	AVLDestroy(user_avl);
	return PASSED;
}

static test_status_t CaseSevenTest(void)
{

	AVL_ty *user_avl = NULL;
	FUNC_TO_TEST(AVLCreate->AVLInsert->AVLHeight->AVLDestroy);

	user_avl = AVLCreate(CmpFunc);
	AVLInsert(user_avl, (void *)3);
	AVLInsert(user_avl, (void *)2);
	AVLInsert(user_avl, (void *)1);
	AVLInsert(user_avl, (void *)8);
	AVLInsert(user_avl, (void *)10);
	AVLInsert(user_avl, (void *)4);
	AVLInsert(user_avl, (void *)5);
	AVLInsert(user_avl, (void *)100);
	AVLInsert(user_avl, (void *)20);

	AVLRemove(user_avl, (void *)10);
	AVLRemove(user_avl, (void *)100);
	AVLRemove(user_avl, (void *)20);
	AVLRemove(user_avl, (void *)1);
	AVLRemove(user_avl, (void *)2);
	AVLRemove(user_avl, (void *)3);
	AVLRemove(user_avl, (void *)4);
	AVLRemove(user_avl, (void *)8);
	AVLRemove(user_avl, (void *)5);

	AVLInsert(user_avl, (void *)5);
	AVLInsert(user_avl, (void *)100);
	AVLInsert(user_avl, (void *)20);
	AVLInsert(user_avl, (void *)8);
	AVLInsert(user_avl, (void *)10);
	AVLInsert(user_avl, (void *)4);

	print2D(user_avl);
	printf("\n------------------------\n");
	printH(user_avl);

	AVLForEach(user_avl, PtintTree, NULL, PRE_ORDER);
	/*	AVLInsert(user_avl, (void *)1);
	AVLInsert(user_avl, (void *)7);
	AVLInsert(user_avl, (void *)2); */

	/* REQUIRE((void *)3 == AVLFind(user_avl, (void *)3)); */

	AVLDestroy(user_avl);
	return PASSED;
}

int CmpFunc(const void *data, const void *data_to_compare)
{
	return *(int *)&data - *(int *)&data_to_compare;
}

int PtintTree(const void *data, void *param)
{
	(void)param;
	printf("the value of this element is:%d\n", *(int *)&data);
	return 0;
}