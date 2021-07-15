#include "../utils/test.h"
#include "sorted_list.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/


int compare_func(const void *data, const void *data_to_compare);
static int SumAllElements(void *data, void *param);

static test_status_t SortedListCreateTest(void);
static test_status_t SortedListDestroyTest(void);
static test_status_t SortedListIsEmptyTest(void);
static test_status_t SortedListSizeTest(void);
static test_status_t SortedListInsertTest(void);
static test_status_t SortedListRemoveTest(void);
static test_status_t SortedListBeginTest(void);
static test_status_t SortedListEndTest(void);
static test_status_t SortedListIsSameIterTest(void);
static test_status_t SortedListPopFrontTest(void);
static test_status_t SortedListPopBackTest(void);
static test_status_t SortedListNextTest(void);
static test_status_t SortedListPrevTest(void);
static test_status_t SortedListGetDataTest(void);
static test_status_t SortedListFindTest(void);
static test_status_t SortedListFindIfTest(void);
static test_status_t SortedListForEachTest(void);
static test_status_t SortedListMergeTest(void);

/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(SortedListCreateTest);
	RUNTEST(SortedListDestroyTest);
	RUNTEST(SortedListIsEmptyTest);
	RUNTEST(SortedListSizeTest);
	RUNTEST(SortedListInsertTest);
	RUNTEST(SortedListRemoveTest);
	RUNTEST(SortedListBeginTest);
	RUNTEST(SortedListEndTest);
	RUNTEST(SortedListIsSameIterTest);
	RUNTEST(SortedListPopFrontTest);
	RUNTEST(SortedListPopBackTest);
	RUNTEST(SortedListNextTest);
	RUNTEST(SortedListPrevTest);
	RUNTEST(SortedListGetDataTest);
	RUNTEST(SortedListFindTest);
	RUNTEST(SortedListFindIfTest);
	RUNTEST(SortedListForEachTest);
	RUNTEST(SortedListMergeTest);

	return PASSED; 
}

/*--------------------------------------------------------*/

static test_status_t SortedListCreateTest(void)
{

	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);
	REQUIRE(NULL !=  user_sorted_list);
	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListDestroyTest(void)
{
	REQUIRE(1 == 1);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListIsEmptyTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	REQUIRE(1 == SortedListIsEmpty(user_sorted_list));
	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListSizeTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	REQUIRE(0 == SortedListSize(user_sorted_list));
	SortedListInsert(user_sorted_list,(void*)5);
	REQUIRE(1 == SortedListSize(user_sorted_list));
	SortedListInsert(user_sorted_list,(void*)6);
	REQUIRE(2 == SortedListSize(user_sorted_list));
	SortedListRemove(user_sorted_list, SortedListBegin(user_sorted_list));
	REQUIRE(1 == SortedListSize(user_sorted_list));
	SortedListRemove(user_sorted_list, SortedListBegin(user_sorted_list));
	REQUIRE(0 == SortedListSize(user_sorted_list));



	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListInsertTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	REQUIRE((void*)5 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListInsert(user_sorted_list,(void*)4);
	REQUIRE((void*)4 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListInsert(user_sorted_list,(void*)6);
	REQUIRE((void*)4 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListInsert(user_sorted_list,(void*)4);
	REQUIRE((void*)4 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListInsert(user_sorted_list,(void*)3);
	REQUIRE((void*)3 == SortedListGetData(SortedListBegin(user_sorted_list)));
	REQUIRE(5 == SortedListSize(user_sorted_list));


	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListRemoveTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	REQUIRE(1 == SortedListSize(user_sorted_list));
	SortedListInsert(user_sorted_list,(void*)6);
	REQUIRE(2 == SortedListSize(user_sorted_list));
	SortedListRemove(user_sorted_list, SortedListBegin(user_sorted_list));
	REQUIRE(1 == SortedListSize(user_sorted_list));
	SortedListRemove(user_sorted_list, SortedListBegin(user_sorted_list));
	


	SortedListDestroy(user_sorted_list);
	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListBeginTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	REQUIRE((void*)5 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListInsert(user_sorted_list,(void*)4);
	REQUIRE((void*)4 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListInsert(user_sorted_list,(void*)6);
	REQUIRE((void*)4 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListInsert(user_sorted_list,(void*)4);
	REQUIRE((void*)4 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListInsert(user_sorted_list,(void*)3);
	REQUIRE((void*)3 == SortedListGetData(SortedListBegin(user_sorted_list)));


	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListEndTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	REQUIRE((void*)5 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListInsert(user_sorted_list,(void*)4);
	REQUIRE((void*)5 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListInsert(user_sorted_list,(void*)6);
	REQUIRE((void*)6 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListInsert(user_sorted_list,(void*)4);
	REQUIRE((void*)6 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListInsert(user_sorted_list,(void*)3);
	REQUIRE((void*)6 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));


	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListIsSameIterTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	
	SortedListInsert(user_sorted_list,(void*)4);
	 REQUIRE(1 == SortedListIsSameIter(SortedListNext(SortedListBegin(user_sorted_list)),SortedListPrev(SortedListEnd(user_sorted_list))));

	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListPopFrontTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)6);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)3);
	
	REQUIRE((void*)3 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListPopFront(user_sorted_list);
	REQUIRE((void*)4 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListPopFront(user_sorted_list);
	REQUIRE((void*)4 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListPopFront(user_sorted_list);
	REQUIRE((void*)5 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListPopFront(user_sorted_list);
	REQUIRE((void*)6 == SortedListGetData(SortedListBegin(user_sorted_list)));
	SortedListPopFront(user_sorted_list);	


	SortedListDestroy(user_sorted_list);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t SortedListPopBackTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)6);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)3);
	

	REQUIRE((void*)6 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListPopBack(user_sorted_list);
	REQUIRE((void*)5 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListPopBack(user_sorted_list);
	REQUIRE((void*)4 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListPopBack(user_sorted_list);
	REQUIRE((void*)4 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListPopBack(user_sorted_list);
	REQUIRE((void*)3 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	SortedListPopBack(user_sorted_list);	


	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListNextTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)6);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)3);
	

	REQUIRE((void*)3 == SortedListGetData(SortedListBegin(user_sorted_list)));
	REQUIRE((void*)4 == SortedListGetData(SortedListNext(SortedListBegin(user_sorted_list))));
	REQUIRE((void*)4 == SortedListGetData(SortedListNext(SortedListNext(SortedListBegin(user_sorted_list)))));
	REQUIRE((void*)5 == SortedListGetData(SortedListNext(SortedListNext(SortedListNext(SortedListBegin(user_sorted_list))))));
	REQUIRE((void*)6 == SortedListGetData(SortedListNext(SortedListNext(SortedListNext(SortedListNext(SortedListBegin(user_sorted_list)))))));
	


	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListPrevTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)6);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)3);
	

	
	REQUIRE((void*)6 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	REQUIRE((void*)5 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list)))));
	REQUIRE((void*)4 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list))))));
	REQUIRE((void*)4 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list)))))));
	REQUIRE((void*)3 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list))))))));


	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListGetDataTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)6);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)3);
	

	
	REQUIRE((void*)6 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list))));
	REQUIRE((void*)5 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list)))));
	REQUIRE((void*)4 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list))))));
	REQUIRE((void*)4 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list)))))));
	REQUIRE((void*)3 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list))))))));


	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListFindTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)6);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)3);

	REQUIRE(1 == SortedListIsSameIter(SortedListFind(user_sorted_list,
	 SortedListBegin(user_sorted_list), SortedListEnd(user_sorted_list), 
	 (void*)5), SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list)))));

	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListFindIfTest(void)
{
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);

	
	SortedListInsert(user_sorted_list,(void*)5);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)6);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)3);

	REQUIRE(1 == SortedListIsSameIter(SortedListFindIf(user_sorted_list,
	 SortedListBegin(user_sorted_list), SortedListEnd(user_sorted_list), compare_func, 
	 (void*)6), SortedListPrev(SortedListEnd(user_sorted_list))));

	SortedListDestroy(user_sorted_list);

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t SortedListForEachTest(void)
{
	int sum = 0, check = 0;
	sorted_list_t *user_sorted_list = SortedListCreate(compare_func);


	SortedListInsert(user_sorted_list,(void*)5);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)6);
	SortedListInsert(user_sorted_list,(void*)4);
	SortedListInsert(user_sorted_list,(void*)3);

	check = SortedListForEach(SortedListBegin(user_sorted_list), SortedListEnd(user_sorted_list), SumAllElements, &sum);
	REQUIRE(0 == check);
	REQUIRE(22 == sum);	


	SortedListDestroy(user_sorted_list);
	return PASSED;	

}
/*--------------------------------------------------------*/

static test_status_t SortedListMergeTest(void)
{
	
	sorted_list_t *user_sorted_list1 = SortedListCreate(compare_func);
	sorted_list_t *user_sorted_list2 = SortedListCreate(compare_func);



	SortedListInsert(user_sorted_list1,(void*)5);
	SortedListInsert(user_sorted_list1,(void*)4);
	SortedListInsert(user_sorted_list1,(void*)6);
	SortedListInsert(user_sorted_list1,(void*)4);
	SortedListInsert(user_sorted_list1,(void*)3);

	SortedListInsert(user_sorted_list1,(void*)15);
	SortedListInsert(user_sorted_list1,(void*)1);
	SortedListInsert(user_sorted_list1,(void*)2);
	SortedListInsert(user_sorted_list1,(void*)4);
	SortedListInsert(user_sorted_list1,(void*)6);
	SortedListInsert(user_sorted_list1,(void*)10);

	SortedListMerge(user_sorted_list1,user_sorted_list2);
	REQUIRE(0 == SortedListSize(user_sorted_list2));
	REQUIRE(11 == SortedListSize(user_sorted_list1));

	REQUIRE((void*)1 == SortedListGetData(SortedListBegin(user_sorted_list1)));
	REQUIRE((void*)2 == SortedListGetData(SortedListNext(SortedListBegin(user_sorted_list1))));
	REQUIRE((void*)3 == SortedListGetData(SortedListNext(SortedListNext(SortedListBegin(user_sorted_list1)))));
	REQUIRE((void*)4 == SortedListGetData(SortedListNext(SortedListNext(SortedListNext(SortedListBegin(user_sorted_list1))))));
	REQUIRE((void*)4 == SortedListGetData(SortedListNext(SortedListNext(SortedListNext(SortedListNext(SortedListBegin(user_sorted_list1)))))));

	REQUIRE((void*)15 == SortedListGetData(SortedListPrev(SortedListEnd(user_sorted_list1))));
	REQUIRE((void*)10 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list1)))));
	REQUIRE((void*)6 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list1))))));
	REQUIRE((void*)6 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list1)))))));
	REQUIRE((void*)5 == SortedListGetData(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListPrev(SortedListEnd(user_sorted_list1))))))));

	SortedListDestroy(user_sorted_list1);
	SortedListDestroy(user_sorted_list2);

	return PASSED;


}
/*--------------------------------------------------------*/

int compare_func(const void *data, const void *data_to_compare)
{
	return *(int*)&data_to_compare - *(int*)&data;
}

static int SumAllElements(void *data, void *param)
{
	*(int *)param += *(int *)&data;
	++*(int *)&data;
	return 0;
}