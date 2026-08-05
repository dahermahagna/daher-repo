#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/


#include "../slist/slist.h"
#include "queue.h"

struct queue 
{
	Slist_t *list;
};

Queue_t *QCreate(void)
{
	Slist_t * new_slist = NULL;
	Queue_t *new_queue = malloc(sizeof(Queue_t));
	if (NULL == new_queue)
	{
		return NULL;
	}

	new_slist = SlistCreate();
	if (NULL == new_slist)
	{
		free(new_queue);
		new_queue = NULL;

		return NULL;
	}
	new_queue->list = new_slist;

	return new_queue;
}

void QDestroy(Queue_t *queue)
{
	assert(NULL != queue);
	SlistDestroy(queue->list);
	free(queue);
	queue = NULL;
}

size_t QSize(const Queue_t *queue)
{
	assert(NULL != queue);
	return SlistCount(queue->list);

}

int QueueIsEmpty(const Queue_t *queue)
{
	assert(NULL != queue);
	return (0 == SlistCount(queue->list));
}

int QEnqueue(Queue_t *queue, void *data)
{
	Slist_iter_t tail_before_insert = NULL;
	assert(NULL != queue);
	tail_before_insert = SlistEnd(queue->list);
	SlistInsert(SlistEnd(queue->list),data);
	return (tail_before_insert == SlistEnd(queue->list));
}

void QDequeue(Queue_t *queue)
{
	assert(NULL != queue);
	SlistRemove(SlistBegin(queue->list));
}

void *QPeek(const Queue_t *queue)
{
	assert(NULL != queue);
	return SlistGetData(SlistBegin(queue->list));
}

void QAppend(Queue_t *dest, Queue_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	SlistAppend(dest->list, src->list);
}
