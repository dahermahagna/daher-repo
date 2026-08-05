#include "../utils/test.h"
#include "slist.h"


test_status_t SlistBeginTest();
test_status_t SlistEndTest();
test_status_t SlistNextTest();
test_status_t SlistIsSameIterTest();
test_status_t SlistCreateTest();
test_status_t SlistDestroyTest();
test_status_t SlistGetDataTest();
test_status_t SlistSetDataTest();
test_status_t SlistInsertTest();
test_status_t SlistRemoveTest();
test_status_t SlistCountTest();
test_status_t SlistFindTest();
test_status_t SlistForEachTest();

int IsMatchFunc(const void *data, const void *data_to_compare);
int AddOne(void *data, void *param);

static Slist_t *user_slist;

int main()
{
	RUNTEST(SlistCreateTest);
	RUNTEST(SlistInsertTest);
	RUNTEST(SlistBeginTest);
	RUNTEST(SlistEndTest);
	RUNTEST(SlistNextTest);
	RUNTEST(SlistIsSameIterTest);
	RUNTEST(SlistGetDataTest);
	RUNTEST(SlistSetDataTest);

	RUNTEST(SlistRemoveTest);
	RUNTEST(SlistCountTest);
	RUNTEST(SlistFindTest);
	RUNTEST(SlistForEachTest);
	RUNTEST(SlistDestroyTest);


return 0; 
}

test_status_t SlistCreateTest()
{
	user_slist =  SlistCreate();
	REQUIRE(NULL != user_slist);
	return PASSED;

} 

test_status_t SlistInsertTest()
{

	SlistInsert( SlistEnd(user_slist), ((void *)15) );
	SlistInsert( SlistEnd(user_slist), ((void *)16) );
	SlistInsert( SlistEnd(user_slist), ((void *)17) );
	SlistInsert( SlistBegin(user_slist), ((void *)14) );
	REQUIRE(4 == SlistCount(user_slist));

	return PASSED;
}

test_status_t SlistBeginTest()
{
	REQUIRE((void*)14 ==  SlistGetData(SlistBegin(user_slist)));
	return PASSED;
}

test_status_t SlistEndTest()
{
	REQUIRE((void*)0 ==  SlistNext(SlistEnd(user_slist)));
	return PASSED;
}

test_status_t SlistNextTest()
{
	REQUIRE((void*)15 ==  SlistGetData(SlistNext(SlistBegin(user_slist))));
	return PASSED;
}

test_status_t SlistIsSameIterTest()
{
	REQUIRE(1 ==  SlistIsSameIter(SlistNext(SlistBegin(user_slist)), SlistNext(SlistBegin(user_slist))));
	return PASSED;
}

test_status_t SlistGetDataTest()
{
	REQUIRE((void*)16 ==  SlistGetData(SlistNext(SlistNext(SlistBegin(user_slist)))));
	return PASSED;
}

test_status_t SlistSetDataTest()
{
	SlistSetData(SlistNext(SlistNext(SlistBegin(user_slist))), (void *)116);
	REQUIRE((void*)116 ==  SlistGetData(SlistNext(SlistNext(SlistBegin(user_slist)))));


	return PASSED;
}

test_status_t SlistRemoveTest()
{

	REQUIRE((void*)15 == SlistGetData(SlistRemove(SlistBegin(user_slist))));
	return PASSED;
}

test_status_t SlistCountTest()
{
	REQUIRE(3 == SlistCount(user_slist));
	return PASSED;
}

test_status_t SlistFindTest()
{

	is_match_func_t func_ptr = IsMatchFunc;
	Slist_iter_t check_iter = NULL;
	check_iter = SlistFind(SlistBegin(user_slist), SlistEnd(user_slist), (void*)17, func_ptr);
	REQUIRE(1 == SlistIsSameIter(check_iter,SlistNext(SlistNext(SlistBegin(user_slist)))));
	return PASSED;
}

test_status_t SlistForEachTest()
{
	action_func_t func_ptr = AddOne;
	int check = 0;
	check = SlistForEach(SlistBegin(user_slist), SlistEnd(user_slist), func_ptr, NULL);
	REQUIRE(0 == check);
	return PASSED;
}

test_status_t SlistDestroyTest()
{

	SlistDestroy(user_slist);
	return PASSED;
}

int IsMatchFunc(const void *data, const void *data_to_compare)
{
	return (data == data_to_compare);
}

int AddOne(void *data, void *param)
{
	(void)param;
	++*(int *)&data;
	return 0;
}