#include <stdio.h>/*printf*/	

#include "stack.h"

int main()
{
	stack_t *user_stack = StackCreate(5);
	if(1 == StackIsEmpty(user_stack))
	{
		printf("the stack is created and StackIsEmpty is working \033[1;32mcorrectly!\033[0m;\n");
	}
	else
	{
		printf("the StackIsEmpty \033[1;31mnot working correctly :(\033[0m\n");
	}

	StackPush(user_stack,(void *)15);
	StackPush(user_stack,(void *)17);
	StackPush(user_stack,(void *)18);
	StackPush(user_stack,(void *)19);
	StackPush(user_stack,(void *)20);

	StackPop(user_stack);

	if(4 == StackSize(user_stack))
	{
		printf("the StackSize is working \033[1;32mcorrectly!\033[0m\n");
	}
	else
	{
		printf("the StackSize is \033[1;31mnot working correctly :(\033[0m\n");
	}

	if((void*)19 == StackPeek(user_stack))
	{
		printf("the stack is StackPush and StackPeek is working \033[1;32mcorrectly!\033[0m\n");
	}
	else
	{
		printf("eather StackPush or StackPeek \033[1;31mnot working correctly :(\033[0m\n");
	}

	if(0 == StackIsEmpty(user_stack))
	{
		printf("the stack is created and StackIsEmpty is working \033[1;32mcorrectly!\033[0m\n");
	}
	else
	{
		printf("the StackIsEmpty \033[1;31mnot working correctly :(\033[0m\n");
	}
	if(5 == StackCapacity(user_stack))
	{
		printf("the StackCapacity is working \033[1;32mcorrectly!\033[0m\n");
	}
	else
	{
		printf("the StackCapacity \033[1;31mnot working correctly :(\033[0m\n");
	}
	StackDestroy(user_stack);


	

	return 0;
}