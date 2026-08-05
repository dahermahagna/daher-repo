#include <stdlib.h>/*malloc*/
#include <assert.h>/*assert*/

#include "pqueue.h"
#include "sorted_list.h"

/*------------------------MACRO---------------------------*/

/*---------------FUNCTION DECLERATION---------------------*/

static int WrapIsMatch(const void *data, const void *data_to_compare);

/*-----------------------TYPEDEF--------------------------*/

/*-----------------------STRUCTS--------------------------*/

/* reviewed by Melinda*/

struct pqueue
{
	sorted_list_t *queue;
};


typedef struct wrap_sruct
{

	pq_is_match_func_t is_match_to_wrap;
	void *data_to_wrap;

}Wrapper_t;

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

	new_pqueue->queue = SortedListCreate(compare_func);
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
	SortedListDestroy(pqueue->queue);
	free(pqueue);
	pqueue = NULL;
}
/*--------------------------------------------------------*/

size_t PQSize(const PQueue_t *pqueue)
{
	assert(NULL != pqueue);
	return SortedListSize(pqueue->queue);
}
/*--------------------------------------------------------*/

int PQIsEmpty(const PQueue_t *pqueue)
{
	assert(NULL != pqueue);
	return SortedListIsEmpty(pqueue->queue);

}
/*--------------------------------------------------------*/

int PQEnpqueue(PQueue_t *pqueue, void *data)
{
	assert(NULL != pqueue);
	return (SortedListIsSameIter(SortedListEnd(pqueue->queue), SortedListInsert(pqueue->queue, data)));
}
/*--------------------------------------------------------*/

void *PQDepqueue(PQueue_t *pqueue)
{
	void *data = NULL;
	assert(NULL != pqueue);
	data = PQPeek(pqueue);
	SortedListPopFront(pqueue->queue);
	return data;
}
/*--------------------------------------------------------*/

void *PQPeek(const PQueue_t *pqueue)
{
	assert(NULL != pqueue);
	return (SortedListGetData(SortedListBegin(pqueue->queue)));
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
	sorted_list_iter_t remove_iter = {NULL};
	void *data = NULL;
	Wrapper_t new_wrapper;

	assert(NULL != pqueue);
	assert(NULL != is_match);
	
	new_wrapper.is_match_to_wrap = is_match;
	new_wrapper.data_to_wrap = data_to_compare;


	remove_iter = SortedListFindIf(pqueue->queue, SortedListBegin(pqueue->queue), SortedListEnd(pqueue->queue), WrapIsMatch, (void*)&new_wrapper);

	if (!SortedListIsSameIter(SortedListEnd(pqueue->queue),remove_iter))
	{
		data = SortedListGetData(remove_iter);
		SortedListRemove(pqueue->queue,remove_iter);
	}

	
	return data;

}
/*--------------------------------------------------------*/


static int WrapIsMatch(const void *data, const void *data_to_compare)
{
	Wrapper_t *new_wrapper = (Wrapper_t*)data_to_compare;
	return !new_wrapper->is_match_to_wrap(data, new_wrapper->data_to_wrap); 
}