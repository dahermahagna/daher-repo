/*
 * NAME:       VSA
 * AUTHORS:    OL94
 * VERSION:    0.1
 */

#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h> /* size_t */

typedef struct variable_sized_allocator VSA_t;

/*DESCRIPTION:
* Creates variable size allocator of memory_size.
* 
*    @param
*     memory - pointer to a sufficient memory for the Allocator.
*     memory_size - size of memory in bytes.
*
*@return
* Pointer to initialized Allocator if successful. 
* NULL if insufficient memory. 
*/
VSA_t *VSAInit(void *memory, size_t memory_size);

/*DESCRIPTION:
* Allocates a block of size_in_bytes memory and returns pointer to it.
* Undefined behavior if size_in_bytes is zero.
*
*    @param
*    allocator - pointer to variable size allocator.
*    size_in_bytes - size of memory to be allocated.
*
*@return
* Pointer to allocated block of memory. 
* NULL in case size_in_bytes is larger than VSALargestChunkAvailable()
*/
void *VSAAlloc(VSA_t *allocator, size_t size_in_bytes);

/*DESCRIPTION:
* Frees block of memory from Allocator. 
* If provided block pointer points outside of memory managed by allocator,
* or if already freed, behavior is undefined.  
*
*    @param
*    block - pointer to a block to be free    
*
*@return
* No return.
*/
void VSAFree(void *block);

/*DESCRIPTION:
* Returns the largest chunk available in allocator's memory.
*
*    @param
*    allocator - pointer to variable size allocator
*
*@return
* The size of the largest chunk available
*/
size_t VSALargestChunkAvailable(VSA_t *allocator);

#endif /* __VSA_H__ */