#ifndef __RECURSION_H__
#define __RECURSION_H__

#include <stddef.h> /* size_t */
#include "../../ds/include/stack.h"

typedef struct node
{
	void *data;
	struct node *next;
} node_t;

/*DESCRIPTION: 
*The strlen() function calculates the length of the string pointed to by s,excluding the terminating null byte ('\0')
*@param
*s: This is the pointer to the array where the content exist.
*
*@return
*the length of the array
*/

size_t Strlen(const char *s);

/*DESCRIPTION:
*function compares the two strings s1 and s2.
*	@param
*	s1:the first string to compare
*	s2: the second sring to compare
*@return
*0 if equal, the first different character between s1 and s2, s1 minus s2.
*/

int Strcmp(const char *s1, const char *s2);

/*DESCRIPTION: 
*Copies the string pointed to by src, including the terminating null byte ('\0'), to the buffer  pointed  to  by  dest.
* dest need to be in a size equal or bigger then the src string
* the function add a termination sign at the end of the copied string
*@param
*dest: This is the pointer to the destination array where the content is to be copied.
*src: This is the string to be copied.
*
*@return
*Pointer to the destination string dest
*/
char *Strcpy(char *dest, const char *src);


/*DESCRIPTION:
* Appends the src string to the dest string,
* overwriting the terminating null byte ('\0') at the end of dest, and then adds a terminating null byte.
* The strings may not overlap, and the  dest  string  must have  enough  space for the result.
* If dest is not large enough, program behavior is unpredictable; buffer overruns are a favorite avenue for attacking secure programs.
*
*@param
* dest: pointer to string to append to
* src: pointer to constant string to be appended
*@return
* Pointer to the resulting string dest.
*/
char *Strcat(char *dest, const char *src);

/*DESCRIPTION:
* Finds the first occurrence of the substring needle in the string haystack.
* The terminating null bytes ('\0') are not compared.
*
* @param
*	haystack: const pointer to string haystach.
*   needle: const pointer to string needle.
*@return
* Pointer to the beginning of the located substring, and  NULL  if  the  substring  is  not found. 
*/
char *Strstr(const char *haystack, const char *needle);

/*DESCRIPTION:
*return the n elemnt in fibonacci Iteratively
*	@param
*	element_index: the n element in fibonacci to be returned
*/
int FibonacciIteratively(int element_index);

/*DESCRIPTION:
*return the n elemnt in fibonacci recursively
*	@param
*	element_index: the n element in fibonacci to be returned
*/
int FibonacciRecursively(int element_index);

/*DESCRIPTION:
*flips Singly list recursively
*	@param
*	node: head of the list.
*/
node_t *FlipList(node_t *node);
void SortStack(stack_t *stack);

#endif  /* __RECURSION_H__ */ 
