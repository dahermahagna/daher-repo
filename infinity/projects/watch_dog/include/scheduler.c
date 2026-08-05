#include <assert.h> /* assert */
#include <time.h> /* time */
#include <stdlib.h> /* malloc */
#include <unistd.h> /*sleep*/

#include "uid.h" /* UIDCreate */
#include "pqueue.h"
#include "task.h"
#include "scheduler.h"


/*------------------------MACRO---------------------------*/

#define ADJUSTMENT_TO_INSERT_AFTER_EQUAL (1)
#define RETURN_TASK_TO_CONTAINER (0)
#define CONTINUE_TO_EXECUTE (0)
#define STOP_EXECUTING (1)


/*---------------FUNCTION DECLERATION---------------------*/

int ComparFunc(const void *data, const void *data_to_compare);
int ComparUID(const void *data, const void *data_to_compare);



/*-----------------------STRUCTS--------------------------*/

struct scheduler
{
	PQueue_t *container;
	int stop_flag;
};

/*--------------------------------------------------------*/

/*-----------          REVIEWED BY OR        -------------*/


Scheduler_t *SchedulerCreate(void)
{
	Scheduler_t *new_scheduler = malloc(sizeof(Scheduler_t));
	if (NULL == new_scheduler)
	{
		return NULL;
	}

	new_scheduler->container = PQCreate(ComparFunc);
	if(NULL == new_scheduler->container)
	{
		free(new_scheduler);
		new_scheduler = NULL;
	}
	return new_scheduler;
}
/*--------------------------------------------------------*/

void SchedulerDestroy(Scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	SchedulerClear(scheduler);

	PQDestroy(scheduler->container);
	free(scheduler);
	scheduler = NULL;
}
/*--------------------------------------------------------*/

size_t SchedulerSize(const Scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return PQSize(scheduler->container);

}
/*--------------------------------------------------------*/

int SchedulerIsEmpty(const Scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return PQIsEmpty(scheduler->container);
}
/*--------------------------------------------------------*/

UID_t SchedulerAdd(Scheduler_t *scheduler, sched_action_func_t action_func, void *param, size_t interval_in_sec)
{
	task_t *new_task = NULL;
	UID_t task_uid = {0};

	assert(NULL != scheduler);
	assert(NULL != action_func);

	new_task = TaskCreate(interval_in_sec, action_func, param);
	if(NULL == new_task)
	{
		task_uid = UIDGetBadUid();
		return task_uid;
	}

	task_uid = TaskGetUid(new_task);

	if (PQEnpqueue(scheduler->container, new_task))
	{
		TaskDestroy(new_task);
		new_task = NULL;
		task_uid = UIDGetBadUid();
	}

	return task_uid;

}
/*--------------------------------------------------------*/

int SchedulerRun(Scheduler_t *scheduler)
{
	int return_status;
	task_t *exe_task = NULL;

	assert(NULL != scheduler);

	scheduler->stop_flag = CONTINUE_TO_EXECUTE;

	while (!SchedulerIsEmpty(scheduler) && CONTINUE_TO_EXECUTE == scheduler->stop_flag)
	{
		exe_task = PQDepqueue(scheduler->container);

		if (0 < TaskGetTimeToExecute(exe_task) - time(NULL))
		{
			sleep(TaskGetTimeToExecute(exe_task) - time(NULL));
		}

		return_status = TaskActivateFunc(exe_task);
		if (RETURN_TASK_TO_CONTAINER == return_status)
		{
			if (PQEnpqueue(scheduler->container, TaskSetTimeToExecute(exe_task)))
			{

				return ERROR;
			}
		}
		else
		{
			TaskDestroy(exe_task);
		}	
		
	}

	if (SchedulerIsEmpty(scheduler))
	{
		return FINISH_ALL_WORK;
	}

	return STOPPED;

}
/*--------------------------------------------------------*/

void SchedulerPause(Scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	scheduler->stop_flag = STOP_EXECUTING;
}
/*--------------------------------------------------------*/

int SchedulerRemove(Scheduler_t *scheduler, UID_t uid)
{
	task_t *return_value = NULL;
	assert(NULL != scheduler);

	return_value = PQErase(scheduler->container, ComparUID, (void*)&uid);

	if(NULL != return_value)
	{
		TaskDestroy(return_value);
		return 0;
	}
	
	return 1;

}
/*--------------------------------------------------------*/

void SchedulerClear(Scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	while(!PQIsEmpty(scheduler->container))
	{
		TaskDestroy(PQDepqueue(scheduler->container));
	}

}
/*--------------------------------------------------------*/
int ComparFunc(const void *data, const void *data_to_compare)
{
	task_t *task_on_container = (task_t*)data;
	task_t *task_to_add = (task_t*)data_to_compare;

	return (TaskGetTimeToExecute(task_to_add) - TaskGetTimeToExecute(task_on_container) + ADJUSTMENT_TO_INSERT_AFTER_EQUAL);
}

int ComparUID(const void *data, const void *data_to_compare)
{
	task_t *task_on_container = (task_t*)data;
	UID_t *uid_to_compare = (UID_t*)data_to_compare;

	return UIDIsSame(TaskGetUid(task_on_container), *uid_to_compare);
}