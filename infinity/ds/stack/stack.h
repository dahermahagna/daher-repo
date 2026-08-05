#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h> /* size_t */

typedef struct stack stack_t;

/*DESCRIPTION: 
*create new stack with given capacity
*@param
*capacity - max number of elements
*@return
* pointer to the new stack or NULL if failed         
*/
stack_t *StackCreate(size_t capacity);

/*DESCRIPTION: 
*delete given stack -free all alocated memory
*@param
*stack - pointer to stack to delete
*@return          
*/
void StackDestroy(stack_t *stack);

/*DESCRIPTION: 
*pop out the first element of the stack 
*undefined behavior if called on an empty stack 
*@param
*stack - pointer to stack
*@return
*/
void StackPop(stack_t *stack);

/*DESCRIPTION: 
* insert element to the top of the stack 
* undefined behavior if called on a full stack
*@param
*stack - pointer to stack
*element - pointer to element to insert
*@return
*/
void StackPush(stack_t *stack , void *element);

/*DESCRIPTION: 
*checks what is the last element in the stack.
*undefined behavior if called on an empty stack
*@param
*stack - an array of void pointers (stack)
*@return
* returns last elemnet in the stack
*/
void *StackPeek(const stack_t *stack);

/*DESCRIPTION: 
*checks the size of stack.
*@param
*stack - an array of void pointers (stack)
*@return
* number of elements in the stack.
*/
size_t StackSize(const stack_t *stack);

/*DESCRIPTION: 
*checks if stack is empty.
*@param
*stack - an array of void pointers (stack)
*@return
* boolean of whether stack is empty (1) or not (0)
*/
int StackIsEmpty(const stack_t *stack);


/*DESCRIPTION: 
*checks the maximum size stack can hold.
*@param
*stack - an array of void pointers (stack)
*@return
* maximum size stack can hold.
*/
size_t StackCapacity(const stack_t *stack);


#endif /*__STACK__*/
