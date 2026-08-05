#include <stdlib.h>/*malloc*/
#include <assert.h>/*asser*/

#include "stack.h"

struct stack
{
	void **stack_elements;
	int stack_pointer;
	size_t stack_capacity;
};


stack_t *StackCreate(size_t capacity)
{
	stack_t *my_stack = (struct stack*) malloc(sizeof(struct stack));
	if (NULL == my_stack)
    {
        return NULL;
    }
	my_stack->stack_capacity = capacity;
	my_stack->stack_pointer = -1;
	my_stack->stack_elements = malloc(capacity * sizeof(void*));
	if (NULL == my_stack->stack_elements)
    {
        free(my_stack);
        return NULL;
    }

	return my_stack;
}

void StackDestroy(stack_t *stack)
{	
	free(stack->stack_elements);
	stack->stack_elements = NULL;
	free(stack);
	stack = NULL;
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);
	assert( -1 != stack->stack_pointer);
	--stack->stack_pointer;
}

void StackPush(stack_t *stack , void *element)
{
	assert( NULL != stack);
	assert( stack->stack_capacity != (size_t)stack->stack_pointer + 1);
	++stack->stack_pointer;
	stack->stack_elements[stack->stack_pointer] = element;
}

void *StackPeek(const stack_t *stack)
{
	assert( NULL != stack);
	return stack->stack_elements[stack->stack_pointer];
}

size_t StackSize(const stack_t *stack)
{
	assert( NULL != stack);
	return stack->stack_pointer + 1;
}

int StackIsEmpty(const stack_t *stack)
{
	assert( NULL != stack);
	return stack->stack_pointer == -1;
}

size_t StackCapacity(const stack_t *stack)
{
	assert( NULL != stack);
	return stack->stack_capacity;
}
