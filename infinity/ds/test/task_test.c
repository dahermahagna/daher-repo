#include <time.h> /* time() */
#include "../utils/test.h"
#include "task.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t StageOneTest(void);

static int PrintCurrentTime(void* data);

/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(StageOneTest);

	return PASSED; 
}

/*--------------------------------------------------------*/



static test_status_t StageOneTest(void)
{
	task_t *user_task = TaskCreate(10, PrintCurrentTime, (void*)5); 

	FUNC_TO_TEST(TaskCreate);
	FUNC_TO_TEST(TaskDestroy);
	FUNC_TO_TEST(TaskActivateFunc);

	REQUIRE(time(NULL) + 5 == TaskActivateFunc(user_task));
	TaskDestroy(user_task);
	return PASSED;
}

static int PrintCurrentTime(void* data)
{
	return time(NULL) + *(int*)&data;
}
