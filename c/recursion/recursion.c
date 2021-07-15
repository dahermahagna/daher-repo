#include <assert.h> /*assert*/
#include "recursion.h"
#include "../../ds/include/stack.h"

static void SortElemnt(stack_t *stack, void *element);

	size_t Strlen(const char *s)
{
	assert(NULL != s);
	
	if('\0' == *s)
	{
		return 0;
	}
	else
	{
		return 1 + Strlen(s+1); 
	}
}

int Strcmp(const char *s1, const char *s2)
{
	assert(NULL != s1);
	assert(NULL != s2);

	if ('\0' == *s1 || *s1 != *s2)
	{
		return *s1 - *s2;
	}
	
	return Strcmp(s1 + 1, s2 + 1);
}



char *Strcpy(char *dest, const char *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	if ('\0'  == *src)
	{
		*dest = *src;
		return dest;
	}
	else
	{
		*dest = *src;
		Strcpy(dest + 1, src + 1);
		return dest;
	}
	
}

char *Strcat(char *dest, const char *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	if ('\0' == *dest)
	{
		*dest = *src;
		*(dest + 1) = '\0';
		if ('\0' == *src)
		{
			*dest = *src;
			return dest;
		}
		else
		{
			Strcat(dest + 1, src + 1);
		}		
	}
	else
	{
		Strcat(dest + 1, src);
		
	}
	return dest;
}

static int IsNeedleIn(const char *haystack, const char *needle)
{
	assert(NULL != haystack);
	assert(NULL != needle);

	if ('\0' == *needle)
	{
		return 1;
	}
	else if (*needle != *haystack)
	{
		return 0;
	}
	return IsNeedleIn(haystack + 1, needle + 1);
}
char *Strstr(const char *haystack, const char *needle)
{
	assert(NULL != haystack);
	assert(NULL != needle);
	if (1 == IsNeedleIn(haystack, needle))
	{
		return (char*)haystack;
	}
	else if ('\0' == *haystack)
	{
		return NULL;
	}
	return Strstr(haystack + 1, needle);
	
}


int FibonacciIteratively(int element_index)
{
	int first = 1, second = 1, i = 2;

	while (i < element_index)
	{
		second +=first;
		first = second - first;
		++i;
	}
	return second;

}

int FibonacciRecursively(int element_index)
{
	if (0 == element_index)
	{
		return 0;
	}
	else if (1 == element_index)
	{
		return 1;
	}
	else
	{
		return (FibonacciRecursively(element_index - 2) + FibonacciRecursively(element_index - 1));
	}
	
	
}

node_t *FlipList(node_t *node)
{
	node_t *next = NULL;
	node_t *new_head = NULL;
	assert(NULL != node);
	if (NULL == node->next)
	{
		return node;
	}
	else
	{
		next = node->next;
		new_head = FlipList(next);
		node->next->next = node;
		node->next = NULL;
		return new_head;
	}
}

void SortStack(stack_t *stack)
{
	void *element = NULL;

	assert(NULL != stack);

	if (0 == StackIsEmpty(stack))
	{
		element = StackPeek(stack);
		StackPop(stack);
		SortStack(stack);
		SortElemnt(stack, element);
	}
}
static void SortElemnt (stack_t *stack, void* element)
{
	void *element_container = NULL;

	assert(NULL != stack);

	if (1 == StackIsEmpty(stack) || element >= StackPeek(stack))
	{
		StackPush(stack, element);
		return;
	}

	element_container = StackPeek(stack);
	StackPop(stack);
	SortElemnt(stack, element);
	StackPush(stack,element_container);
}
