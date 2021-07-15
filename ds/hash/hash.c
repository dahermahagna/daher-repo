#include <assert.h> /*assert*/ 
#include <stdlib.h> /*malloc*/
#include "hash.h"
#include "../include/dlist.h"


/*------------------------MACRO---------------------------*/

#define EMPTY_TABLE (0)

/*---------------FUNCTION DECLERATION---------------------*/

static hash_table_ty *InitHashArray(hash_table_ty *hash_table);
static Dlist_iter_t FindData(hash_table_ty *hash_table, const void *data);
static void DestroyBySize(hash_table_ty *hash_table, size_t i);

/*-----------------------TYPEDEF--------------------------*/

/*-----------------------STRUCTS--------------------------*/

struct hash_table
{
    hash_func_ty hash_func;
    is_match_func_ty match_func;
    size_t capacity;
    Dlist_t **hash_arr;
};

/*--------------------------------------------------------*/

hash_table_ty *HashCreate(hash_func_ty hash_func, is_match_func_ty is_match_func, size_t capacity)
{
    hash_table_ty *new_htable = NULL;

    assert(NULL != hash_func);
    assert(NULL != is_match_func);


    new_htable = (hash_table_ty *) malloc(sizeof(hash_table_ty));
    if(NULL == new_htable)
    {
        return NULL;
    }
    new_htable->capacity = capacity;
    new_htable->hash_func = hash_func;
    new_htable->match_func = is_match_func;
    InitHashArray(new_htable);
    return new_htable;
}
static hash_table_ty *InitHashArray(hash_table_ty *hash_table)
{
    size_t i = 0;
    assert(hash_table);
    hash_table->hash_arr = malloc(hash_table->capacity * (sizeof(size_t)));
    if (NULL == hash_table->hash_arr)
    {
        return NULL;
    }

    while (hash_table->capacity > i)
    {
        hash_table->hash_arr[i] = DlistCreate();
        if(NULL == hash_table->hash_arr[i])
        {
            DestroyBySize(hash_table, i);
            return NULL;
        }
        ++i;
    }
    return hash_table;
}

static void DestroyBySize(hash_table_ty *hash_table, size_t i)
{
    size_t j = 0;

    assert(NULL != hash_table);
    
    while(i > j)
    {
        DlistDestroy(hash_table->hash_arr[j]);
        ++j;
    }

    free(hash_table->hash_arr);
    hash_table->hash_arr = NULL;
    free(hash_table);
    hash_table = NULL;

}
/*--------------------------------------------------------*/

void HashDestroy(hash_table_ty *hash_table)
{

    assert(hash_table);

    DestroyBySize(hash_table,hash_table->capacity);
  
}
/*--------------------------------------------------------*/

int HashIsEmpty(const hash_table_ty *hash_table)
{
    size_t i = 0;

    assert(hash_table);

    return EMPTY_TABLE == HashSize(hash_table);
}
/*--------------------------------------------------------*/

size_t HashSize(const hash_table_ty *hash_table)
{
    size_t i = 0,count = 0;

    assert(hash_table);

    while (hash_table->capacity > i)
    {
        count += DlistSize(hash_table->hash_arr[i]);
        ++i;
    }
    return count;
}
/*--------------------------------------------------------*/

int HashInsert(hash_table_ty *hash_table, void *data)
{
    size_t index = 0;
    Dlist_iter_t iter = NULL;
    assert(NULL != hash_table);
    index = hash_table->hash_func(data);
    iter = DlistFind(DlistBegin(hash_table->hash_arr[index]), 
                        DlistEnd(hash_table->hash_arr[index]), 
                                hash_table->match_func, data);
    if (!DlistIsSameIter(DlistEnd(hash_table->hash_arr[index]), iter))
    {
        DlistSetData(iter, data);
    }
    else
    {
        iter = DlistPushBack(hash_table->hash_arr[index], data);
        if (DlistIsSameIter(DlistEnd(hash_table->hash_arr[index]), iter))
        {
            return 0;
        }
    }

    return 1;
}
/*--------------------------------------------------------*/

void HashRemove(hash_table_ty *hash_table, const void *data)
{
    size_t index = 0;
    Dlist_iter_t iter = NULL;

    assert(hash_table);

    index = hash_table->hash_func(data);
    iter = FindData(hash_table, data);
    if(NULL != iter)
    {
        DlistRemove(hash_table->hash_arr[index], iter);
    }
}
/*--------------------------------------------------------*/

void *HashFind(hash_table_ty *hash_table, const void *data)
{
    Dlist_iter_t iter = NULL;
    assert(NULL != hash_table);

    iter = FindData(hash_table,data);
    return NULL == iter ? NULL : DlistGetData(iter);
}

static Dlist_iter_t FindData(hash_table_ty *hash_table, const void *data)
{
    size_t index = 0;
    Dlist_iter_t iter = NULL;
    assert(hash_table);
    index = hash_table->hash_func(data);
    iter = DlistFind(DlistBegin(hash_table->hash_arr[index]),
                     DlistEnd(hash_table->hash_arr[index]), hash_table->match_func, data);
    if(DlistIsSameIter(iter,DlistEnd(hash_table->hash_arr[index])));
    {
        return NULL;
    }
    return iter;
}
/*--------------------------------------------------------*/

int HashForEach(hash_table_ty *hash_table, action_func_ty action_func, void *param)
{
    size_t i = 0;
    int res = 0;

    assert(hash_table);
    assert(action_func);
    while (hash_table->capacity > i && 0 == res)
    {
        res = DlistForEach(DlistBegin(hash_table->hash_arr[i]), DlistEnd(hash_table->hash_arr[i]), action_func,param);
        ++i;
    }
    return res;
}
/*--------------------------------------------------------*/
