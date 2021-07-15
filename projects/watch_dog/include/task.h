#ifndef __TASK_H__
#define __TASK_H__



#include <stddef.h> /* size_t */
#include "uid.h"	/* uid_t */

typedef struct task task_t;

/* DESCRIPTION:
 * User's function to be executed in a task
 *
 *        @param
 *        void* - user's argument for the function
 *
 * @return
 * Returns 1 if task is completed and 0 if task requires rescheduling.
 */
typedef int(*task_action_func_t)(void *);

/*DESCRIPTION:
* Create new task
* Created task should be destroyed with taskDestroy() function
*
*		@param
*       no param
*
* @return
* Pointer to the new task or NULL if failed
*/
task_t *TaskCreate(size_t interval, task_action_func_t action_func, void *data);

/*DESCRIPTION:
* deletes given task and frees all allocated memory
*
*	@param
* 	task - pointer to the task to delete
*
* @return
*/
void TaskDestroy(task_t *task);

/*DESCRIPTION:
* activate the function of the task with the param of task.
*
*	@param
* 	task - pointer to the task which function to be activate
*
* @return
* int- the return value of the action function
*/
int TaskActivateFunc(task_t *task);

/*DESCRIPTION:
* return the UID of the task.
*
*	@param
* 	task - pointer to the task which function to be activate
*
* @return
* UID_t- the UID of the Task
*/
UID_t TaskGetUid(task_t *task);

/*DESCRIPTION:
* return the time_to_execute of the task.
*
*	@param
* 	task - pointer to the task 
*
* @return
* time_t- the time_to_execute of the Task
*/
time_t TaskGetTimeToExecute(task_t *task);


/*DESCRIPTION:
* return the task with updated time to execute.
*
*	@param
* 	task - pointer to the task 
*
* @return
* task_t- task with updated time to execute
*/
task_t *TaskSetTimeToExecute(task_t *task);




#endif /* __TASK_H__ */