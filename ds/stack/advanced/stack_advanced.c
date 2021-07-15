#include <stdio.h>
#include <stdlib.h>/*malloc*/
#include <assert.h>/*asser*/

#include "stack.h"

#define STACK_CAPACITY *(size_t *)&stack[0]
#define STACK_POINTER *(size_t *)&stack[1]


struct stack
{
	size_t stack_capacity;
};


stack_t *StackCreate(size_t capacity)
{

	stack_t *my_stack = (void *) malloc(sizeof(struct stack) * (capacity + 2));
	if(NULL == my_stack)
	{
		return NULL;
	}
	
	*(size_t *)&my_stack->stack_capacity = capacity;
	*(size_t *)&my_stack[1] = 1;

	return my_stack;
}

void StackDestroy(stack_t *stack)
{
	free(stack);
	stack = NULL;
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);
	assert(1 != STACK_POINTER);
	--STACK_POINTER;
}

void StackPush(stack_t *stack, void *element)
{
	assert( NULL != stack);
	assert( STACK_POINTER != STACK_CAPACITY +1);
	++STACK_POINTER;
	*(void**)&stack[STACK_POINTER] = element;

}

void *StackPeek(const stack_t *stack)
{
	assert( NULL != stack);
	assert(0 != STACK_POINTER);
	return *(void **)&stack[STACK_POINTER];
}

size_t StackSize(const stack_t *stack)
{
	assert( NULL != stack);
	return STACK_POINTER-1;
}

int StackIsEmpty(const stack_t *stack)
{
	assert( NULL != stack);
	return STACK_POINTER == 1;
}

size_t StackCapacity(const stack_t *stack)
{
	assert( NULL != stack);
	return STACK_CAPACITY ;
}
