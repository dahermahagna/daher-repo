#include <stdlib.h>/*malloc*/
#include <assert.h>/*assert*/

#include "pqueue.h"
#include "heap.h"

/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/


/*-----------------------TYPEDEF--------------------------*/

/*-----------------------STRUCTS--------------------------*/

/* reviewed by Melinda*/

struct pqueue
{
	Heap_ty *queue;
};

/*--------------------------------------------------------*/

PQueue_t *PQCreate(pq_compare_func_t compare_func)
{
	PQueue_t *new_pqueue = NULL;
	assert(NULL != compare_func);

	new_pqueue = (PQueue_t*)malloc(sizeof(PQueue_t));
	if (NULL == new_pqueue)
	{
		return NULL;
	}

	new_pqueue->queue = HeapCreate(compare_func);
	if (NULL == new_pqueue->queue)
	{
		free(new_pqueue);
		new_pqueue = NULL;
	}

	return new_pqueue;

}
/*--------------------------------------------------------*/

void PQDestroy(PQueue_t *pqueue)
{
	assert(NULL != pqueue);
	HeapDestroy(pqueue->queue);
	free(pqueue);
	pqueue = NULL;
}
/*--------------------------------------------------------*/

size_t PQSize(const PQueue_t *pqueue)
{
	assert(NULL != pqueue);
	return HeapSize(pqueue->queue);
}
/*--------------------------------------------------------*/

int PQIsEmpty(const PQueue_t *pqueue)
{
	assert(NULL != pqueue);
	return HeapIsEmpty(pqueue->queue);

}
/*--------------------------------------------------------*/

int PQEnpqueue(PQueue_t *pqueue, void *data)
{
	assert(NULL != pqueue);
	return (HeapPush(pqueue->queue,data));
}
/*--------------------------------------------------------*/

void *PQDepqueue(PQueue_t *pqueue)
{
	void *data = NULL;
	assert(NULL != pqueue);
	data = HeapPeek(pqueue->queue);
	HeapPop(pqueue->queue);
	return data;
}
/*--------------------------------------------------------*/

void *PQPeek(const PQueue_t *pqueue)
{
	assert(NULL != pqueue);
	return (HeapPeek(pqueue->queue));
}
/*--------------------------------------------------------*/

void PQClear(PQueue_t *pqueue)
{
	assert(NULL != pqueue);
	while(!PQIsEmpty(pqueue))
	{
		PQDepqueue(pqueue);
	}
}
/*--------------------------------------------------------*/

void *PQErase(PQueue_t *pqueue, pq_is_match_func_t is_match, void *data_to_compare)
{
	assert(NULL != pqueue);
	assert(NULL != is_match);
	
	return HeapRemove(pqueue->queue,is_match,data_to_compare);
}
/*--------------------------------------------------------*/
