/*
 * NAME:       scheduler
 * AUTHORS:    OL94
 * VERSION:    0.1
 */

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h> /* size_t */
#include "uid.h"	/* uid_t */


typedef struct scheduler Scheduler_t;

enum SCHED_RUN_RETURN_STATUS
{
	FINISH_ALL_WORK = 0, 
	STOPPED = 1,
	ERROR = 2
};

/* DESCRIPTION: 
 * User's function to be executed in a scheduler
 *
 *        @param
 *        void* - user's argument for the function 
 *
 * @return
 * Returns 1 if task is completed and 0 if task requires rescheduling.
 */
typedef int (*sched_action_func_t)(void *);


/*DESCRIPTION:
* Create new scheduler
* Created scheduler should be destroyed with SchedulerDestroy() function
*
*		@param
*       no param
* @return
* Pointer to the new scheduler or NULL if failed
*/
Scheduler_t *SchedulerCreate(void);

/*DESCRIPTION:
* deletes given scheduler and frees all allocated memory
*
*	@param
* 	scheduler - pointer to the scheduler to delete
*
* @return
*/
void SchedulerDestroy(Scheduler_t *scheduler);

/*DESCRIPTION:
* gets the amounts of functions in the scheduler
*
*	@param
* 	scheduler - pointer to the scheduler
*
* @return
* number of functions in the scheduler
*/
size_t SchedulerSize(const Scheduler_t *scheduler);

/*DESCRIPTION:
* Checks if there are any functions in the scheduler
*
*	@param
* 	scheduler - pointer to the scheduler
*
*@return
* 1 if scheduler is empty, 0 if not
*/
int SchedulerIsEmpty(const Scheduler_t *scheduler);

/*DESCRIPTION:
* Add the function to the scheduler.
*
*	@param
* 	scheduler - pointer to scheduler
*	action_func - pointer to operation function 
*	param - user's argument for action_func_ptr_t function 
*	interval_in_sec - time between each action_func execution
*
*@return
* A valid UID of successfully scheduled function, 
* UIDGetBadUID() in case of of failure.
*/
UID_t SchedulerAdd(Scheduler_t *scheduler, sched_action_func_t action_func,
										void *param, size_t interval_in_sec);

/*DESCRIPTION:
* Runs the scheduler.
*
*	@param
* 	scheduler - pointer to scheduler
*
* @return
* FINISH_ALL_WORK - on execution of all functions
* STOPPED - on SchedulerPause()
* ERROR - on error
*/
int SchedulerRun(Scheduler_t *scheduler);

/*DESCRIPTION:
* Pauses the scheduler. Scheduler can be run again.
* 
*	@param
* 	Scheduler - pointer to scheduler
*
*@return
*/
void SchedulerPause(Scheduler_t *scheduler);

/*DESCRIPTION:
* Remove one function from the scheduler.
*
*	@param
* 	Scheduler - pointer to the scheduler.
*	Uid - uid of the function.
*
*@return
* 0 on successful remove, 1 if function was not found
*/
int SchedulerRemove(Scheduler_t *scheduler, UID_t uid);

/*DESCRIPTION:
* Clear all functions from the scheduler. 
*
*	@param
* 	Scheduler - pointer to the scheduler.
*
*@return
*/
void SchedulerClear(Scheduler_t *scheduler);


#endif /* __SCHEDULER_H__ */
