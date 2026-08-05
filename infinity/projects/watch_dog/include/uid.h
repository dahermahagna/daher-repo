#ifndef __UID_H__
#define __UID_H__

#include <time.h>       /* time_t */
#include <sys/types.h>  /* pid_t, size_t */

/*
 * NAME:       UID
 * AUTHORS:    OL94
 * VERSION:    0.1
 */


/* WARNING!
 * Never access the fields of the UID directly.
 * The names, types, and number of fields can change in future. 
 * Only use provided functions.
 */

typedef struct uid
{
	pid_t pid;
	time_t time_stamp;
	size_t counter;
} UID_t;

/*DESCRIPTION: 
* creates Uid instance.
*
*		@param
*
* @return
* return uid
*/
UID_t UIDCreate(void);

/*DESCRIPTION: 
* compare if two Uids are the same;
*
*		@param
*		one - uid to compare
*		two - uid to compare
*
* @return
* return 1 if the uids are the same, 0 if not.
*/
int UIDIsSame(UID_t one, UID_t two);

/*DESCRIPTION: 
* create an instance of Bad Uid;
*
*		@param
*		
*		
* @return
* return BadUid;
*/
UID_t UIDGetBadUid(void);


#endif /* __UID_H__ */