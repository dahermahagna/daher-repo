#include "../utils/test.h"
#include "uid.h"



/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static test_status_t UIDCreateTest(void);
static test_status_t UIDIsSameTest(void);
static test_status_t UIDGetBadUidTest(void);

/*-------------------------MAIN---------------------------*/


int main()
{
	RUNTEST(UIDCreateTest);
	RUNTEST(UIDIsSameTest);
	RUNTEST(UIDGetBadUidTest);

	return PASSED; 
}

/*--------------------------------------------------------*/

static test_status_t UIDCreateTest(void)
{
	UID_t uid_1 = UIDCreate();
	UID_t uid_2 = UIDCreate();

	REQUIRE(0 == UIDIsSame(uid_1,uid_2));


	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t UIDIsSameTest(void)
{
	UID_t uid_1 = UIDCreate();
	UID_t uid_2 = UIDCreate();

	REQUIRE(0 == UIDIsSame(uid_1,uid_2));

	uid_2 = uid_1;

	REQUIRE(1 == UIDIsSame(uid_1,uid_2));

	return PASSED;
}
/*--------------------------------------------------------*/

static test_status_t UIDGetBadUidTest(void)
{

	UID_t uid_1 = UIDGetBadUid();

	REQUIRE(uid_1.pid == 0 && uid_1.time_stamp == 0 && uid_1.time_stamp == 0);

	return PASSED;
}
/*--------------------------------------------------------*/

