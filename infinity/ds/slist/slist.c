#include <stdlib.h>/*malloc free*/
#include <assert.h>/*asser*/

#include "slist.h"

static Slist_iter_t GetEndByNext(Slist_iter_t node_runner);
static int CountNode(void *data, void *param);

/*reviwed by May*/

struct slist
{

	struct slist_node *head;
	struct slist_node *tail;
};

typedef struct slist_node
{
	void * data;
	Slist_iter_t next_node;
	
}slist_node;

Slist_t *SlistCreate(void)
{
	Slist_iter_t new_head = NULL;

	Slist_t *new_slist = (Slist_t*)malloc(sizeof(Slist_t));
	if (NULL == new_slist)
	{
		return NULL;
	}

	new_head = (Slist_iter_t)malloc(sizeof(slist_node));
	if (NULL == new_head)
	{
		free(new_slist);
		return NULL;
	}

	new_slist->head = new_head;
	new_slist->tail = new_head;

	new_head->data = (Slist_iter_t)new_slist;
	new_head->next_node = NULL;

	return new_slist;
}

Slist_iter_t SlistBegin(const Slist_t *list)
{
	assert(NULL != list);

	return list->head;
}

Slist_iter_t SlistEnd(const Slist_t *list)
{
	assert(NULL != list);

	return list->tail;
}

Slist_iter_t SlistNext(Slist_iter_t iter)
{
	assert(NULL != iter);

	return iter->next_node;
}

Slist_iter_t SlistInsert(Slist_iter_t where, void *data)
{
	Slist_iter_t new_node = NULL;

	assert(NULL != where);

	new_node = (Slist_iter_t) malloc(sizeof(slist_node));
	if (NULL == new_node)
	{
		return GetEndByNext(where);
	}

	new_node->next_node = where->next_node;
	SlistSetData(new_node,where->data);

	where->next_node = new_node;
	SlistSetData(where,data);
	if (NULL == new_node->next_node)
	{
		((Slist_t *)(new_node->data))->tail = new_node;
	}

	return where;
}

static Slist_iter_t GetEndByNext(Slist_iter_t node_runner)
{
	assert(NULL != node_runner);

	while (NULL != node_runner->next_node)
	{
		node_runner = node_runner->next_node;
	}
	return node_runner;
}

int SlistIsSameIter(Slist_iter_t one, Slist_iter_t two)
{
	assert(NULL != one);
	assert(NULL != two);

	return (one->next_node == two->next_node);
}

void *SlistGetData(const Slist_iter_t iter)
{
	assert(NULL != iter);

	return iter->data;
}

void SlistSetData(Slist_iter_t iter, void *data)
{
	assert(NULL != iter);

	iter->data = data;
}

Slist_iter_t SlistRemove(Slist_iter_t iter)
{
	
	Slist_iter_t buffer = NULL;

	assert(NULL != iter);

	if (NULL == SlistNext(iter))
	{
		return NULL;
	}

	

	SlistSetData(iter, SlistGetData(SlistNext(iter)));
	
	if (NULL == SlistNext(SlistNext(iter)))
	{
		((Slist_t *)(iter->data))->tail = iter;
		((Slist_t *)(iter->data))->head = iter;
	}

	buffer = SlistNext(iter);
	iter->next_node = (SlistNext(iter))->next_node;

	free(buffer);
	buffer = NULL;
	
	return iter;
}

int SlistForEach(Slist_iter_t from, Slist_iter_t to, action_func_t action_func, void * param)
{
	int result = 0;
	Slist_iter_t runner = from;
	assert(NULL != action_func);

	while (!SlistIsSameIter(runner,to) && 0 == action_func(from->data,param))
	{
		runner = SlistNext(runner);
	}
	return result;
}

Slist_iter_t SlistFind(Slist_iter_t from, Slist_iter_t to, const void *data_to_compare, is_match_func_t is_match_func)
{
	Slist_iter_t runner = from;
	assert(NULL != is_match_func);

	while (!SlistIsSameIter(runner,to) && 0 == is_match_func(SlistGetData(runner), data_to_compare))
	{
		runner = SlistNext(runner);
	}
	
	return runner;
}

static int CountNode(void *data,void *param)
{
	(void) data;
	++*(size_t *)param;
	return 0;
}


size_t SlistCount(const Slist_t *list)
{
	size_t count = 0;
	action_func_t action_func = CountNode;

	assert(NULL != list);

	SlistForEach(SlistBegin(list),SlistEnd(list), action_func, (void *)&count);
	return count;
}

void SlistDestroy(Slist_t *list)
{
	Slist_iter_t node_runner = NULL;
	Slist_iter_t buffer = NULL;

	assert(NULL != list);
	node_runner = list->head;
	
	do
	{
		buffer = node_runner->next_node;
		free(node_runner);
		node_runner = buffer;
	}while (NULL != node_runner);

	free(list);

}

void SlistAppend(Slist_t *dest, Slist_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);

	if(NULL == SlistBegin(src)->next_node)
	{
		return;
	}

	SlistSetData(SlistEnd(src), SlistGetData(SlistEnd(dest)));

	SlistSetData(SlistEnd(dest), SlistGetData(SlistBegin(src)));

	(SlistEnd(dest))->next_node = (SlistBegin(src))->next_node;

	dest->tail = src->tail;
	SlistBegin(src)->data = src;
	SlistBegin(src)->next_node = NULL; 
	src->tail = src->head;

}

