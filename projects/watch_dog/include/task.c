#include <assert.h> /* assert */
#include <time.h> /* time */
#include <stdlib.h> /* malloc */

#include "uid.h" /* UIDCreate */
#include "task.h"


/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

/*-----------------------TYPEDEF--------------------------*/

/*-----------------------STRUCTS--------------------------*/

struct task
{
	size_t interval;
	void *data;
	task_action_func_t task_action;
	time_t time_to_execute;
	UID_t task_UID;
};

/*--------------------------------------------------------*/

task_t *TaskCreate(size_t interval, task_action_func_t action_func, void *data)
{
	task_t *new_task = malloc(sizeof(struct task));
	if(NULL == new_task)
	{	
		return NULL;
	} 
	new_task->interval = interval;
	new_task->data = data;
	new_task->task_action = action_func;
	new_task->time_to_execute = time(NULL) + interval;
	new_task->task_UID = UIDCreate();
	return new_task;
}
/*--------------------------------------------------------*/

void TaskDestroy(task_t *task)
{
	assert(NULL != task);
	free(task);
	task = NULL;

}
/*--------------------------------------------------------*/

int TaskActivateFunc(task_t *task)
{
	assert(NULL != task);
	return task->task_action(task->data);

}
/*--------------------------------------------------------*/

UID_t TaskGetUid(task_t *task)
{
	assert(NULL != task);
	return task->task_UID;
}
/*--------------------------------------------------------*/

time_t TaskGetTimeToExecute(task_t *task)
{
	assert(NULL != task);
	return task->time_to_execute;
}
/*--------------------------------------------------------*/

task_t *TaskSetTimeToExecute(task_t *task)
{
	assert(NULL != task);
	task->time_to_execute = time(NULL) + task->interval;
	return task;
}



