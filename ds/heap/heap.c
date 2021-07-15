#include <assert.h> /* assert*/
#include <stdlib.h> /* malloc */

#include "../include/vector.h"
#include "heap.h"


/*------------------------MACRO---------------------------*/

#define VECTOR heap->vector
#define GET_CHILD1(index) (2 * index + 1)
#define GET_CHILD2(index) (2 * index + 2)
#define FIRST (0)
#define ADJUST_TO_INDEX (1)

/*---------------FUNCTION DECLERATION---------------------*/

static void HeapIfyUp(Heap_ty *heap, size_t index, void *data);
static size_t GetParent(size_t index);
static void HeapIfyDown(Heap_ty *heap, size_t index, void *data);
static size_t GetBigChild(Heap_ty *heap, size_t child1, size_t child2);
static void SwapByIndex(Heap_ty *heap, size_t index, size_t big_child);

/*-----------------------TYPEDEF--------------------------*/

/*-----------------------STRUCTS--------------------------*/

struct heap
{
    Vector_t *vector;
    cmp_func_ty compare_func;
};

/*--------------------------------------------------------*/

Heap_ty *HeapCreate(cmp_func_ty cmp)
{
    Heap_ty *new_heap = NULL;

    assert(NULL != cmp);

    new_heap = (Heap_ty *)malloc(sizeof(Heap_ty));
    if (NULL == new_heap)
    {
        return NULL;
    }
    new_heap->compare_func = cmp;
    new_heap->vector = VectorCreate(7);
    if (NULL == new_heap->vector)
    {
        free(new_heap);
        new_heap = NULL;
    }
    return new_heap;
}
/*--------------------------------------------------------*/

void HeapDestroy(Heap_ty *heap)
{
    assert(NULL != heap);
    VectorDestroy(VECTOR);
    free(heap);
    heap = NULL;
}
/*--------------------------------------------------------*/

void HeapPop(Heap_ty *heap)
{
    void *last_element = NULL;

    assert(NULL != heap);

    last_element = VectorGetElement(VECTOR, VectorSize(VECTOR));
    VectorSetElement(VECTOR, last_element, 1);
    VectorPopBack(VECTOR);
    HeapIfyDown(heap, FIRST, last_element);
}
/*--------------------------------------------------------*/

int HeapPush(Heap_ty *heap, void *data)
{
    int res = 0;
    size_t size = 0;
    assert(NULL != heap);
    res = VectorPushBack(VECTOR, data);
    if(res != 0)
    {
        return res;
    }
    size = VectorSize(VECTOR);
    HeapIfyUp(heap, size - ADJUST_TO_INDEX, data);
    return 0;
}
/*--------------------------------------------------------*/

void *HeapPeek(const Heap_ty *heap)
{
    assert(NULL != heap);
    return VectorGetElement(VECTOR, 1);
}
/*--------------------------------------------------------*/

void *HeapRemove(Heap_ty *heap ,is_match_func_ty match_func ,void *data)
{
    size_t runner = 0, vector_size = 0;
    void *data_of_runner = NULL;
    void *last_element = NULL;

    assert(NULL != heap);
    assert(NULL != match_func);

    vector_size = VectorSize(VECTOR);
    
    do
    {
        ++runner;
        data_of_runner = VectorGetElement(VECTOR, runner);
    }while (runner < vector_size && 0 != match_func(data_of_runner,data));

    if (runner == vector_size)
    {
        return NULL;
    }

    last_element = VectorGetElement(VECTOR, vector_size);
    VectorSetElement(VECTOR, last_element, runner);

    VectorPopBack(VECTOR);
    HeapIfyDown(heap, runner - ADJUST_TO_INDEX, last_element);
    return data_of_runner;
}
/*--------------------------------------------------------*/

int HeapIsEmpty(const Heap_ty *heap)
{
    assert(NULL != heap);
    return 0 == VectorSize(VECTOR);
}
/*--------------------------------------------------------*/

size_t HeapSize(const Heap_ty *heap)
{
    assert(NULL != heap);
    return VectorSize(VECTOR);
}
/*--------------------------------------------------------*/

static void HeapIfyUp(Heap_ty *heap, size_t index, void *data)
{
    size_t  parent = 0;
    assert(NULL != heap);
    parent = GetParent(index);

    while (index != 0 && 0 > heap->compare_func(VectorGetElement(VECTOR, parent + ADJUST_TO_INDEX), data))
    {
        SwapByIndex(heap,parent,index);
        index = parent;
        parent = GetParent(parent);
    }

}
static size_t GetParent(size_t index)
{
    return index == 0 ? 0 : (index - 1) / 2;
}

static void HeapIfyDown(Heap_ty *heap, size_t index, void *data)
{
    size_t child1 = GET_CHILD1(index);
    size_t child2 = GET_CHILD2(index);
    size_t big_child = 0;
    assert(NULL != heap);
    
    big_child = GetBigChild(heap, child1, child2);
    while (index + 1 < VectorSize(VECTOR) && 0 < heap->compare_func(VectorGetElement(VECTOR, big_child + ADJUST_TO_INDEX), data))
    {

        SwapByIndex(heap,index,big_child);
        child1 = GET_CHILD1(big_child);
        child2 = GET_CHILD2(big_child);
        index = big_child;
        big_child = GetBigChild(heap,child1,child2);
    }

    
}

static void SwapByIndex(Heap_ty *heap, size_t index, size_t big_child)
{
    void *container = NULL;
    void *container2 = NULL;
    container = VectorGetElement(VECTOR, index + ADJUST_TO_INDEX);
    container2 = VectorGetElement(VECTOR,big_child +ADJUST_TO_INDEX);
    VectorSetElement(VECTOR, container2, index + ADJUST_TO_INDEX);
    VectorSetElement(VECTOR, container, big_child + ADJUST_TO_INDEX);
}


static size_t GetBigChild(Heap_ty *heap, size_t child1, size_t child2)
{
    if(child2 >= VectorSize(VECTOR))
    {
        if (child1 >= VectorSize(VECTOR))
        {
            return (child1 - 1) / 2; 
        }
        return child1;
    }

    if (0 < heap->compare_func(VectorGetElement(VECTOR, child1 + ADJUST_TO_INDEX), VectorGetElement(VECTOR, child2 + ADJUST_TO_INDEX)))
        return child1;
    return child2;
}

