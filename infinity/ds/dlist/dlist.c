#include <stdlib.h>/*malloc*/
#include <assert.h>/*assert*/


#include "dlist.h"

/*reviwed by or*/

typedef struct dlist_node
{
	void * data;
	struct dlist_node *next_node;
	struct dlist_node *prev_node;
}dlist_node_t;

typedef struct dlist
{
	struct dlist_node begin_dummy;
	struct dlist_node end_dummy;
}dlist_t;

static Dlist_iter_t CreatNewNode();

Dlist_t *DlistCreate(void)
{
	Dlist_t *new_dlist = (dlist_t *)malloc(sizeof(dlist_t));
	if(NULL == new_dlist)
	{
		return NULL;
	}

	new_dlist->begin_dummy.next_node = &new_dlist->end_dummy;
	new_dlist->begin_dummy.prev_node = NULL;
	new_dlist->begin_dummy.data = new_dlist;

	new_dlist->end_dummy.prev_node = &new_dlist->begin_dummy;
	new_dlist->end_dummy.next_node = NULL;
	new_dlist->end_dummy.data = new_dlist;

	return new_dlist;

}

void DlistDestroy(Dlist_t *list)
{
	dlist_node_t *node_runner = NULL;
	dlist_node_t *buffer = NULL;

	assert(NULL != list);
	node_runner = list->begin_dummy.next_node;
	
	while (NULL != DlistNext(node_runner))
	{
		buffer = node_runner->next_node;
		free(node_runner);
		node_runner = NULL;
		node_runner = buffer;
	}

	free(list);
	list = NULL;
}

int DlistIsEmpty(const Dlist_t *list)
{
	assert(NULL != list);
	return (DlistBegin(list) == DlistEnd(list));
}

size_t DlistSize(const Dlist_t *list)
{
	dlist_node_t *node_runner = NULL;
	dlist_node_t *end_of_list = NULL;
	size_t nodes_counter = 0;

	assert(NULL != list);

	end_of_list = DlistEnd(list);
	node_runner = DlistBegin(list);

	while (end_of_list != node_runner)
	{
		++nodes_counter;
		node_runner = DlistNext(node_runner);
		
	} 

	return nodes_counter;

}

Dlist_iter_t DlistInsert(Dlist_t *list, Dlist_iter_t where, void *data)
{
	dlist_node_t *new_node = NULL;

	assert(NULL != list);
	assert(NULL != where);

	new_node = CreatNewNode();
	if (NULL == new_node)
	{
		return DlistEnd(list);
	}

	new_node->next_node = where;
	new_node->prev_node = where->prev_node;
	DlistSetData(new_node,data);

	where->prev_node->next_node = new_node;
	where->prev_node = new_node;

	return new_node;
}

static Dlist_iter_t CreatNewNode()
{
	dlist_node_t *new_node = (dlist_node_t*)malloc(sizeof(dlist_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}

	new_node->data = NULL;
	new_node-> next_node = NULL;
	new_node->prev_node = NULL;

	return new_node;
}


Dlist_iter_t DlistRemove(Dlist_t *list, Dlist_iter_t iter)
{
	dlist_node_t *buffer = NULL;

	(void)list;
	assert(NULL != iter);
	assert(&list->end_dummy != iter);
	
	buffer = DlistNext(iter);
	iter->next_node->prev_node = DlistPrev(iter);
	iter->prev_node->next_node = DlistNext(iter);

	free(iter);
	iter = NULL;

	return buffer;
}

Dlist_iter_t DlistPushFront(Dlist_t *list, void *data)
{
	assert(NULL != list);
	return DlistInsert(list, DlistBegin(list), data);
}

Dlist_iter_t DlistPushBack(Dlist_t *list, void *data)
{
	assert(NULL != list);
	return DlistInsert(list, DlistEnd(list), data);
}

void DlistPopFront(Dlist_t *list)
{
	assert(NULL != list);
	assert(!DlistIsEmpty(list));

	DlistRemove(list, DlistBegin(list));
}

void DlistPopBack(Dlist_t *list)
{
	assert(NULL != list);
	assert(!DlistIsEmpty(list));

	DlistRemove(list, DlistPrev(DlistEnd(list)));
}

Dlist_iter_t DlistBegin(const Dlist_t *list)
{
	assert(NULL != list);
	return list->begin_dummy.next_node;	
}

Dlist_iter_t DlistEnd(const Dlist_t *list)
{
	assert(NULL != list);
	return (dlist_node_t*)&list->end_dummy;
}

int DlistIsSameIter(Dlist_iter_t one, Dlist_iter_t two)
{
	assert(NULL != one);
	assert(NULL != two);
	return (one == two);
}

Dlist_iter_t DlistNext(Dlist_iter_t iter)
{
	assert(NULL != iter);
	return iter->next_node;
}

Dlist_iter_t DlistPrev(Dlist_iter_t iter)
{
	assert(NULL != iter);
	return iter->prev_node;

}

void *DlistGetData(const Dlist_iter_t iter)
{
	assert(NULL != iter);
	assert(NULL != DlistNext(iter));
	assert(NULL != DlistPrev(iter));

	return iter->data;
}

void DlistSetData(Dlist_iter_t iter, void *data)
{
	assert(NULL != iter);
	assert(NULL != DlistNext(iter));
	assert(NULL != DlistPrev(iter));

	iter->data = data;
}

Dlist_iter_t DlistFind(Dlist_iter_t from, Dlist_iter_t to,is_match_func_t is_match_func, const void *data_to_compare)
{
	Dlist_iter_t runner = NULL; 
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match_func);
	runner = from;


	while (!DlistIsSameIter(runner,to) && 0 == is_match_func(DlistGetData(runner), data_to_compare))
	{
		runner = DlistNext(runner);
	}
	
	return runner;
}

int DlistMultiFind(Dlist_t *dest, Dlist_iter_t from, Dlist_iter_t to,is_match_func_t is_match_func, const void *data_to_compare)
{
	Dlist_iter_t runner = NULL; 
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_match_func);
	assert(NULL != dest);
	runner = from;


	while (!DlistIsSameIter(runner,to))
	{
		if(1 == is_match_func(DlistGetData(runner), data_to_compare))
		{
			if(NULL  == DlistNext(DlistPushBack(dest,DlistGetData(runner))))
			{
				return (1);
			}
		}

		runner = DlistNext(runner);
	}
	
	return (0);
}

int DlistForEach(Dlist_iter_t from, Dlist_iter_t to,action_func_t action_func, void *param)
{
	int result = 0;
	Dlist_iter_t runner = NULL; 
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action_func);
	runner = from;

	while (!DlistIsSameIter(runner,to) && !(result = action_func(runner->data,param)))
	{
		runner = DlistNext(runner);
	}
	return result;
}

Dlist_iter_t DlistSplice(Dlist_iter_t where, Dlist_iter_t src_from,Dlist_iter_t src_to)
{

	dlist_node_t *src_from_prev = DlistPrev(src_from);

	assert(NULL != where);
	assert(NULL != src_from);
	assert(NULL != src_to);

	DlistPrev(src_to)->next_node = where;
    DlistPrev(where)->next_node = src_from;
    DlistPrev(src_from)->next_node = src_to;


    src_from->prev_node = DlistPrev(where);
    where->prev_node = DlistPrev(src_to);
    src_to->prev_node = src_from_prev;


	return DlistPrev(where);
}