#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/


#include "sorted_list.h"
#include "dlist.h"

/*------------------------sorted_list_iter_t---------------------------*/

#ifdef NDEBUG


static sorted_list_iter_t CreatIter(Dlist_iter_t iter, const sorted_list_t *list)
{
    sorted_list_iter_t s_list_iter = {NULL};
    (void)list;
    s_list_iter.internal_itr = iter;
    return s_list_iter;
}

static sorted_list_iter_t UpdateInternalIter(Dlist_iter_t iter, Dlist_iter_t node)
{
	iter.internal_itr = node;
	return iter;
}


#else

static sorted_list_iter_t CreatIter(Dlist_iter_t iter, const sorted_list_t *list)
{
    sorted_list_iter_t s_list_iter = {NULL,NULL};
    s_list_iter.internal_itr = iter;
    s_list_iter.list = (sorted_list_t *)list;

    return s_list_iter;
}

#endif

static Dlist_iter_t GetInternalIterFromIter(sorted_list_iter_t iter)
{
	return iter.internal_itr;
}

static sorted_list_iter_t UpdateInternalIter(sorted_list_iter_t iter, Dlist_iter_t node)
{
	iter.internal_itr = node;
	return iter;
}






/*---------------FUNCTION DECLERATION---------------------*/

/*-----------------------TYPEDEF--------------------------*/

/*-----------------------STRUCTS--------------------------*/

struct sorted_list
{
	Dlist_t *sorted_list;
	compare_func_t sort_func;

};

/*--------------------------------------------------------*/

sorted_list_t *SortedListCreate(compare_func_t sort_func)
{
	struct sorted_list *new_sorted_list = (struct sorted_list *)(malloc(sizeof(struct sorted_list)));
	if (NULL == new_sorted_list)
	{
		return NULL;
	}

	assert(NULL != sort_func);

	new_sorted_list->sorted_list = DlistCreate();
	if (NULL == new_sorted_list->sorted_list)
	{
		free(new_sorted_list);
		new_sorted_list = NULL;
		return NULL;
	}

	new_sorted_list->sort_func = sort_func;
	return new_sorted_list;

}


void SortedListDestroy(sorted_list_t *list)
{
	assert(NULL != list);
	DlistDestroy(list->sorted_list);
	free(list);
	list = NULL;

}

int SortedListIsEmpty(const sorted_list_t *list)
{	
	assert(NULL != list);
	return DlistIsEmpty(list->sorted_list);
}


size_t SortedListSize(const sorted_list_t *list)
{
	assert(NULL != list);
	return DlistSize(list->sorted_list);
}


sorted_list_iter_t SortedListInsert(sorted_list_t *list, void *data)
{
	sorted_list_iter_t runner = SortedListBegin(list);
	sorted_list_iter_t last_node_iter = SortedListEnd(list);

	assert(NULL != list);

	while (!SortedListIsSameIter(runner, last_node_iter) && 0 <= list->sort_func(SortedListGetData(runner),data))
	{
		runner = SortedListNext(runner);
	}

	return CreatIter(DlistInsert(list->sorted_list,GetInternalIterFromIter(runner),data),list);

}


sorted_list_iter_t SortedListRemove(sorted_list_t *list, sorted_list_iter_t iter)
{
	assert(NULL != list);
	return CreatIter(DlistRemove(list->sorted_list,GetInternalIterFromIter(iter)),list);
}


sorted_list_iter_t SortedListBegin(const sorted_list_t *list)
{
	assert (NULL != list);
	return CreatIter(DlistBegin(list->sorted_list),list);
}


sorted_list_iter_t SortedListEnd(const sorted_list_t *list)
{
	assert (NULL != list);
	return CreatIter(DlistEnd(list->sorted_list),list);
}


int SortedListIsSameIter(sorted_list_iter_t one, sorted_list_iter_t two)
{
	assert(NULL != GetInternalIterFromIter(one));
	assert(NULL != GetInternalIterFromIter(two));
	return(DlistIsSameIter(GetInternalIterFromIter(one), GetInternalIterFromIter(two)));
}


void SortedListPopFront(sorted_list_t *list)
{
	assert(NULL != list);
	DlistRemove(list->sorted_list, DlistBegin(list->sorted_list));
}


void SortedListPopBack(sorted_list_t *list)
{
	assert(NULL != list);
	DlistRemove(list->sorted_list, DlistPrev(DlistEnd(list->sorted_list)));

}


sorted_list_iter_t SortedListNext(sorted_list_iter_t iter)
{
	sorted_list_iter_t next_iter = iter;
	assert(NULL != GetInternalIterFromIter(iter));

	next_iter = UpdateInternalIter(next_iter,DlistNext(GetInternalIterFromIter(iter)));

	/*next_iter.internal_itr = DlistNext(next_iter.internal_itr);*/

	return next_iter;
}


sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter)
{

	sorted_list_iter_t prev_iter = iter;
	assert(NULL != GetInternalIterFromIter(iter));

	prev_iter = UpdateInternalIter(prev_iter,DlistPrev(GetInternalIterFromIter(iter)));
	return prev_iter;
}


void *SortedListGetData(const sorted_list_iter_t iter)
{

	return DlistGetData(GetInternalIterFromIter(iter));
}


sorted_list_iter_t SortedListFind(sorted_list_t *list, sorted_list_iter_t from, sorted_list_iter_t to, const void *data_to_compare)
{
	sorted_list_iter_t runner = from;
	assert(NULL != list);
	assert(NULL != GetInternalIterFromIter(from));
	assert(NULL != GetInternalIterFromIter(to));


	while(!SortedListIsSameIter(runner,to) && 0 != list->sort_func(SortedListGetData(runner),data_to_compare))
	{
		runner = SortedListNext(runner);
	}
	return runner;


}


sorted_list_iter_t SortedListFindIf(sorted_list_t *list, sorted_list_iter_t from, sorted_list_iter_t to, compare_func_t search_func, const void *data_to_compare)
{

	sorted_list_iter_t runner = from;
	(void)list;
	assert(NULL != GetInternalIterFromIter(from));
	assert(NULL != GetInternalIterFromIter(to));

	while(!SortedListIsSameIter(runner,to) && 0 != search_func(SortedListGetData(runner),data_to_compare))
	{
		runner = SortedListNext(runner);
	}
	return runner;
}


int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, action_func_t action_func, void *param)
{
	int result = 0;
	sorted_list_iter_t runner = from;
	assert(NULL != action_func);
	assert(NULL != GetInternalIterFromIter(from));
	assert(NULL != GetInternalIterFromIter(to));

	while(!SortedListIsSameIter(runner,to) && !(result = action_func(SortedListGetData(runner),param)))
	{
		runner = SortedListNext(runner);
	}
	return result;
}


sorted_list_t *SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
	sorted_list_iter_t dest_runner;
	assert(dest->sort_func == src->sort_func);
	assert(NULL != dest );
	assert(NULL != src);

	dest_runner = SortedListBegin(dest);

	while (!SortedListIsEmpty(src))
	{
		while (!SortedListIsSameIter(dest_runner, SortedListEnd(dest)) && 0 < dest->sort_func(SortedListGetData(dest_runner),SortedListGetData(SortedListBegin(src)))) 
		{
			dest_runner = SortedListNext(dest_runner);
		}
	
		DlistSplice(GetInternalIterFromIter(dest_runner),GetInternalIterFromIter(SortedListBegin(src)),GetInternalIterFromIter(SortedListNext(SortedListBegin(src))));
	}

	return dest;
}

