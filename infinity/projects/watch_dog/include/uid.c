#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/
#include <unistd.h> /* getpid */
#include <stdatomic.h> /* atomic */
#include <pthread.h> /* pthread */

/*reviewed by Or*/

#include "uid.h"

/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

/*-----------------------TYPEDEF--------------------------*/


/*--------------------------------------------------------*/

UID_t UIDCreate(void)
{
	static atomic_size_t inner_counter = 1;

	UID_t new_uid = {0};
	new_uid.pid = getpid();
	new_uid.time_stamp = time(NULL);
	new_uid.counter = atomic_fetch_add(&inner_counter, 1);

	return new_uid;
}
/*--------------------------------------------------------*/

int UIDIsSame(UID_t one, UID_t two)
{
	return (one.pid == two.pid && one.time_stamp == two.time_stamp && one.counter == two.counter);
}
/*--------------------------------------------------------*/

UID_t UIDGetBadUid(void)
{
	UID_t bad_uid = {0};
	return bad_uid;
}
/*--------------------------------------------------------*/
