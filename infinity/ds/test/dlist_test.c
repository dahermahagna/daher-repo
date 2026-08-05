#include "../utils/test.h"
#include "dlist.h"


static int IsMatchFunc(const void *data, const void *data_to_compare);
static int SumAllElements(void *data, void *param);


static test_status_t DlistCreateTest();
static test_status_t DlistDestroyTest();
static test_status_t DlistIsEmptyTest();
static test_status_t DlistSizeTest();
static test_status_t DlistInsertTest();
static test_status_t DlistRemoveTest();
static test_status_t DlistPushFrontTest();
static test_status_t DlistPushBackTest();
static test_status_t DlistPopFrontTest();
static test_status_t DlistPopBackTest();
static test_status_t DlistBeginTest();
static test_status_t DlistEndTest();
static test_status_t DlistIsSameIterTest();
static test_status_t DlistNextTest();
static test_status_t DlistPrevTest();
static test_status_t DlistGetDataTest();
static test_status_t DlistSetDataTest();
static test_status_t DlistFindTest();
static test_status_t DlistMultiFindTest();
static test_status_t DlistForEachTest();
static test_status_t DlistSpliceTest();


int main()
{
	RUNTEST(DlistCreateTest);
	RUNTEST(DlistDestroyTest);
	RUNTEST(DlistIsEmptyTest);
	RUNTEST(DlistSizeTest);
	RUNTEST(DlistInsertTest);
	RUNTEST(DlistRemoveTest);
	RUNTEST(DlistPushFrontTest);
	RUNTEST(DlistPushBackTest);
	RUNTEST(DlistPopFrontTest);
	RUNTEST(DlistPopBackTest);
	RUNTEST(DlistBeginTest);
	RUNTEST(DlistEndTest);
	RUNTEST(DlistIsSameIterTest);
	RUNTEST(DlistNextTest);
	RUNTEST(DlistPrevTest);
	RUNTEST(DlistGetDataTest);
	RUNTEST(DlistSetDataTest);
	RUNTEST(DlistFindTest);
	RUNTEST(DlistMultiFindTest);
	RUNTEST(DlistForEachTest);
	RUNTEST(DlistSpliceTest);


return 0; 
}

static test_status_t DlistCreateTest()
{
	Dlist_t *user_dlist = DlistCreate();
	REQUIRE(NULL !=  user_dlist);
	DlistDestroy(user_dlist);

	return PASSED;
}

static test_status_t DlistDestroyTest()
{
	return PASSED;
}

static test_status_t DlistIsEmptyTest()
{
	Dlist_t *user_dlist = DlistCreate();
	REQUIRE(1 == DlistIsEmpty(user_dlist));
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)14);
	REQUIRE(0 == DlistIsEmpty(user_dlist));

	DlistDestroy(user_dlist);

	return PASSED;
}

static test_status_t DlistSizeTest()
{
	Dlist_t *user_dlist = DlistCreate();
	REQUIRE(0 == DlistSize(user_dlist));
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)14);
	REQUIRE(1 == DlistSize(user_dlist));
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	REQUIRE(2 == DlistSize(user_dlist));
	DlistPopBack(user_dlist);
	REQUIRE(1 == DlistSize(user_dlist));
	DlistPopBack(user_dlist);
	REQUIRE(0 == DlistSize(user_dlist));
	
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistInsertTest()
{

	Dlist_t *user_dlist = DlistCreate();
	REQUIRE(1 == DlistIsEmpty(user_dlist));
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	REQUIRE(0 == DlistIsEmpty(user_dlist));

	DlistDestroy(user_dlist);

	return PASSED;
}

static test_status_t DlistRemoveTest()
{
	Dlist_t *user_dlist = DlistCreate();
	Dlist_iter_t test_runner = NULL; 
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)14);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)17);
	REQUIRE((void*)14 == DlistGetData(DlistBegin(user_dlist)));
	test_runner = DlistRemove(user_dlist, DlistBegin(user_dlist));
	REQUIRE((void*)15 == DlistGetData(test_runner));
	test_runner = DlistRemove(user_dlist, DlistNext(test_runner));
	REQUIRE((void*)17 == DlistGetData(test_runner));
	test_runner = DlistRemove(user_dlist, test_runner);
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistPushFrontTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)15 == DlistGetData(DlistBegin(user_dlist)));
	DlistPushFront(user_dlist,(void*)14);
	REQUIRE((void*)14 == DlistGetData(DlistBegin(user_dlist)));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistPushBackTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)16 == DlistGetData(DlistPrev(DlistEnd(user_dlist))));
	DlistPushBack(user_dlist,(void*)17);
	REQUIRE((void*)17 == DlistGetData(DlistPrev(DlistEnd(user_dlist))));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistPopFrontTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)15 == DlistGetData(DlistBegin(user_dlist)));
	DlistPopFront(user_dlist);
	REQUIRE((void*)16 == DlistGetData(DlistBegin(user_dlist)));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistPopBackTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)16 == DlistGetData(DlistPrev(DlistEnd(user_dlist))));
	DlistPopBack(user_dlist);
	REQUIRE((void*)15 == DlistGetData(DlistPrev(DlistEnd(user_dlist))));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistBeginTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)15 == DlistGetData(DlistBegin(user_dlist)));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistEndTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)16 == DlistGetData(DlistPrev(DlistEnd(user_dlist))));
	REQUIRE((void*)15 == DlistGetData(DlistPrev(DlistPrev(DlistEnd(user_dlist)))));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistIsSameIterTest()
{

	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE(1 == DlistIsSameIter(DlistBegin(user_dlist), DlistPrev(DlistPrev(DlistEnd(user_dlist)))));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistNextTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)15 == DlistGetData(DlistBegin(user_dlist)));
	REQUIRE((void*)16 == DlistGetData(DlistNext(DlistBegin(user_dlist))));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistPrevTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)16 == DlistGetData(DlistPrev(DlistEnd(user_dlist))));
	REQUIRE((void*)15 == DlistGetData(DlistPrev(DlistPrev(DlistEnd(user_dlist)))));
	
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistGetDataTest()
{

	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)15 == DlistGetData(DlistBegin(user_dlist)));
	DlistDestroy(user_dlist);

	return PASSED;
}

static test_status_t DlistSetDataTest()
{
	Dlist_t *user_dlist = DlistCreate();
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	REQUIRE((void*)15 == DlistGetData(DlistBegin(user_dlist)));
	DlistSetData(DlistBegin(user_dlist),(void*)150);
	REQUIRE((void*)150 == DlistGetData(DlistBegin(user_dlist)));
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistFindTest()
{
	is_match_func_t func_ptr = IsMatchFunc;
	Dlist_t *user_dlist = DlistCreate();
	Dlist_iter_t check_iter = NULL; 
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)14);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)17);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)18);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)19);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)20);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)21);
	

	check_iter = DlistFind(DlistBegin(user_dlist), DlistEnd(user_dlist), func_ptr, (void*) 20);
	REQUIRE(1 == DlistIsSameIter(check_iter, DlistPrev(DlistPrev(DlistEnd(user_dlist)))));

	
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistMultiFindTest()
{
	is_match_func_t func_ptr = IsMatchFunc;
	Dlist_t *user_dlist = DlistCreate();
	Dlist_t *dest = DlistCreate();
 
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)20);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)20);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)17);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)20);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)19);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)20);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)21);

	DlistMultiFind(dest, DlistBegin(user_dlist), DlistEnd(user_dlist),func_ptr,  (void*) 20);
	REQUIRE(4 == DlistSize(dest));

	DlistDestroy(dest);
	dest = DlistCreate();
	DlistMultiFind(dest, DlistBegin(user_dlist), DlistPrev(DlistPrev(DlistEnd(user_dlist))),func_ptr,  (void*) 20);
	REQUIRE(3 == DlistSize(dest));

	DlistDestroy(dest);
	dest = DlistCreate();
	DlistPopBack(user_dlist);
	DlistPopBack(user_dlist);
	DlistMultiFind(dest, DlistBegin(user_dlist), DlistEnd(user_dlist),func_ptr,  (void*) 20);
	REQUIRE(3 == DlistSize(dest));



	DlistDestroy(dest);
	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistForEachTest()
{
	int check = 0, count = 0;
	action_func_t func_ptr = SumAllElements;
	Dlist_t *user_dlist = DlistCreate();

	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)14);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)15);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)16);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)17);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)18);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)19);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)20);
	DlistInsert(user_dlist,DlistEnd(user_dlist),(void*)21);

	check = DlistForEach(DlistBegin(user_dlist), DlistEnd(user_dlist), func_ptr, &count);
	REQUIRE(0 == check);
	REQUIRE(140 == count);


	DlistDestroy(user_dlist);
	return PASSED;
}

static test_status_t DlistSpliceTest()
{
	Dlist_t *dlist_src = DlistCreate();
	Dlist_t *dlist_dest = DlistCreate();
	REQUIRE(NULL != dlist_src);
	REQUIRE(NULL != dlist_dest);
	  
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)9);
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)8);
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)7);
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)6);
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)5);
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)4);
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)3);
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)2);
	DlistInsert(dlist_src, DlistBegin(dlist_src), (void *)1);

	DlistInsert(dlist_dest, DlistBegin(dlist_dest), (void *)5);
	DlistInsert(dlist_dest, DlistBegin(dlist_dest), (void *)4);
	DlistInsert(dlist_dest, DlistBegin(dlist_dest), (void *)3);
	DlistInsert(dlist_dest, DlistBegin(dlist_dest), (void *)2);
	DlistInsert(dlist_dest, DlistBegin(dlist_dest), (void *)1);

	REQUIRE(9 == DlistSize(dlist_src));
	REQUIRE(5 == DlistSize(dlist_dest));

	REQUIRE(DlistPrev(DlistFind(DlistBegin(dlist_src), DlistEnd(dlist_src), IsMatchFunc, (void *)7)) ==
				DlistSplice(DlistNext(DlistNext(DlistBegin(dlist_dest))),
				DlistFind(DlistBegin(dlist_src), DlistEnd(dlist_src), IsMatchFunc, (void *)4),
				DlistFind(DlistBegin(dlist_src), DlistEnd(dlist_src), IsMatchFunc, (void *)7)));

	REQUIRE(6 == DlistSize(dlist_src));
	REQUIRE(8 == DlistSize(dlist_dest));

	REQUIRE((void *)1 == DlistGetData(DlistBegin(dlist_src)));
	REQUIRE((void *)2 == DlistGetData(DlistNext(DlistBegin(dlist_src))));
	REQUIRE((void *)3 == DlistGetData(DlistNext(DlistNext(DlistBegin(dlist_src)))));
	REQUIRE((void *)7 == DlistGetData(DlistNext(DlistNext(DlistNext(DlistBegin(dlist_src))))));
	REQUIRE((void *)8 == DlistGetData(DlistNext(DlistNext(DlistNext(DlistNext(DlistBegin(dlist_src)))))));
	REQUIRE((void *)9 == DlistGetData(DlistNext(DlistNext(DlistNext(DlistNext(DlistNext(DlistBegin(dlist_src))))))));

	REQUIRE((void *)1 == DlistGetData(DlistBegin(dlist_dest)));
	REQUIRE((void *)2 == DlistGetData(DlistNext(DlistBegin(dlist_dest))));
	REQUIRE((void *)4 == DlistGetData(DlistNext(DlistNext(DlistBegin(dlist_dest)))));
	REQUIRE((void *)5 == DlistGetData(DlistNext(DlistNext(DlistNext(DlistBegin(dlist_dest))))));
	REQUIRE((void *)6 == DlistGetData(DlistNext(DlistNext(DlistNext(DlistNext(DlistBegin(dlist_dest)))))));
	REQUIRE((void *)3 == DlistGetData(DlistNext(DlistNext(DlistNext(DlistNext(DlistNext(DlistBegin(dlist_dest))))))));
	REQUIRE((void *)4 == DlistGetData(DlistNext(DlistNext(DlistNext(DlistNext(DlistNext(DlistNext(DlistBegin(dlist_dest)))))))));
	REQUIRE((void *)5 == DlistGetData(DlistNext(DlistNext(DlistNext(DlistNext(DlistNext(DlistNext(DlistNext(DlistBegin(dlist_dest))))))))));

	DlistDestroy(dlist_src);
	DlistDestroy(dlist_dest);


    return PASSED;

}

static int IsMatchFunc(const void *data, const void *data_to_compare)
{
	return (data == data_to_compare);
}

static int SumAllElements(void *data, void *param)
{
	*(int *)param += *(int *)&data;
	++*(int *)&data;
	return 0;
}

