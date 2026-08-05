#include <unistd.h> /*sleep*/
#include "../utils/test.h"
#include "scheduler.h"
#include "uid.h" /* UIDCreate */




/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t CaseOneTest(void);
static test_status_t CaseTwoTest(void);
static test_status_t CaseTthreeTest(void);

static int PrintCurrentTime(void* data);
static int PrintMyName(void* data);
static int PauseTheProgram(void* data);

/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(CaseOneTest);
	RUNTEST(CaseTwoTest);
	RUNTEST(CaseTthreeTest);

	return PASSED; 
}

/*--------------------------------------------------------*/

static test_status_t CaseOneTest(void)
{
	/*Test Case #1: SchedulerCreate > SchedulerIsEmpty > SchedulerAdd > SchedulerIsEmpty > SchedulerDestroy*/

	Scheduler_t *user_scheduler = SchedulerCreate();
	FUNC_TO_TEST(SchedulerCreate);
	FUNC_TO_TEST(SchedulerAdd);
	FUNC_TO_TEST(SchedulerIsEmpty);

	REQUIRE(1 == SchedulerIsEmpty(user_scheduler));
	SchedulerAdd(user_scheduler, PrintCurrentTime, NULL, 10);

	REQUIRE(0 == SchedulerIsEmpty(user_scheduler));

	SchedulerDestroy(user_scheduler);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTwoTest(void)
{
	/*Test Case #2: SchedulerCreate > SchedulerAdd > SchedulerRun > SchedulerDestroy*/
	int return_status = 0;
	Scheduler_t *user_scheduler = SchedulerCreate();
	FUNC_TO_TEST(SchedulerRun);
	

	SchedulerAdd(user_scheduler, PrintCurrentTime, (void*)(time(NULL) + 5), 1);
	
 
	return_status = SchedulerRun(user_scheduler);

	REQUIRE(FINISH_ALL_WORK == return_status);

	REQUIRE(1 == SchedulerIsEmpty(user_scheduler));

	SchedulerDestroy(user_scheduler);

	return PASSED;
}

/*--------------------------------------------------------*/

static test_status_t CaseTthreeTest(void)
{
	/*Test Case #3: SchedulerCreate > SchedulerAdd X 3 > SchedulerSize >
	 SchedulerRun (with BadApple) > SchedulerRemove > SchedulerAdd X 5 >
	 SchedulerClear*/
	
	int return_status = 0;
	UID_t uid = {0};
	Scheduler_t *user_scheduler = SchedulerCreate();

	FUNC_TO_TEST(SchedulerDestroy);
	FUNC_TO_TEST(SchedulerSize);
	FUNC_TO_TEST(SchedulerPause);
	FUNC_TO_TEST(SchedulerRemove);
	FUNC_TO_TEST(SchedulerClear);
	

	SchedulerAdd(user_scheduler, PrintCurrentTime, (void*)(time(NULL)+15), 3);
	uid = SchedulerAdd(user_scheduler, PrintMyName, (void*)(time(NULL)+15), 1);

	SchedulerAdd(user_scheduler, PauseTheProgram, user_scheduler, 6);
	REQUIRE(3 == SchedulerSize(user_scheduler));

	return_status = SchedulerRun(user_scheduler);
	REQUIRE(STOPPED == return_status);
	printf("continue after 2 seconds\n");
	sleep(2);
	SchedulerRemove(user_scheduler ,uid);
	REQUIRE(1 == SchedulerSize(user_scheduler));
	SchedulerAdd(user_scheduler, PauseTheProgram, user_scheduler, 6);
	SchedulerAdd(user_scheduler, PrintMyName, (void*)(time(NULL)+15), 2);
	REQUIRE(3 == SchedulerSize(user_scheduler));

	return_status = SchedulerRun(user_scheduler);
	REQUIRE(STOPPED == return_status);
	REQUIRE(2 == SchedulerSize(user_scheduler));

	return_status = SchedulerRun(user_scheduler);
	REQUIRE(FINISH_ALL_WORK == return_status);

	SchedulerAdd(user_scheduler, PrintMyName, (void*)(time(NULL)+4), 2);
	REQUIRE(1 == SchedulerSize(user_scheduler));
	SchedulerAdd(user_scheduler, PrintMyName, (void*)(time(NULL)+4), 2);
	REQUIRE(2 == SchedulerSize(user_scheduler));
	SchedulerAdd(user_scheduler, PrintMyName, (void*)(time(NULL)+4), 2);
	REQUIRE(3 == SchedulerSize(user_scheduler));
	SchedulerAdd(user_scheduler, PrintMyName, (void*)(time(NULL)+4), 2);
	REQUIRE(4 == SchedulerSize(user_scheduler));
	SchedulerAdd(user_scheduler, PrintMyName, (void*)(time(NULL)+4), 2);
	REQUIRE(5 == SchedulerSize(user_scheduler));

	SchedulerClear(user_scheduler);

	REQUIRE(0 == SchedulerSize(user_scheduler));

	REQUIRE(1 == SchedulerIsEmpty(user_scheduler));

	SchedulerDestroy(user_scheduler);

	return PASSED;
}

static int PrintCurrentTime(void* data)
{
	time_t now = time(NULL);
	time_t param_time = (time_t)data;
	struct tm *ptm_now = localtime(&now);


	if(time(NULL) < *(int*)&data)
	{
		printf("The time now is: %02d:%02d:%02d < ", ptm_now->tm_hour, ptm_now->tm_min, ptm_now->tm_sec);
		ptm_now = localtime(&param_time);
		printf("%02d:%02d:%02d\n", ptm_now->tm_hour, ptm_now->tm_min, ptm_now->tm_sec);	
		return  0;
	}
	return 1;
}

static int PrintMyName(void* data)
{
	if(time(NULL) < *(int*)&data)
	{
		printf("Daher\n");
		return  0;
	}
	return 1;
}

static int PauseTheProgram(void* data)
{
	SchedulerPause((Scheduler_t*)data);
	return 1;
}