/*
 * NAME:       FSA
 * AUTHORS:    OL94
 * VERSION:    0.1
 */

#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h> /* size_t */

typedef struct fixed_size_allocator FSA_t;

/*DESCRIPTION:
* Calculate the memory needed for fixed size allocator. 
* Both parameters need to give a value greater than 0.
*
*	@param
*	block_size - size of each block in bytes
* 	num_of_blocks - number of blocks
*
*@return
* Amount of bytes needed for fixed size allocator.
*/
size_t FSASuggestSize(size_t block_size, size_t num_of_blocks);


/*DESCRIPTION:
* Creates fixed size allocator that holds blocks of memory.
* Size of memory must be enough to allocate at least one block of memory. 
* It can be calculated using FSASuggestSize()
*
*	@param
* 	memory - pointer to a sufficient memory for the Allocator.
* 	memory_size - size of memory in bytes.
*	block_size - size of each block in bytes.
*
*@return
* Pointer to initialized Allocator if successful, NULL if provided memory is 
* not enough. 
*/
FSA_t *FSAInit(void *memory, size_t memory_size, size_t block_size);

/*DESCRIPTION:
* Allocates one block of memory and returns pointer to it. 
*
*	@param
*	allocator - pointer to fixed size allocator	
*
*@return
* Pointer to allocated block of memory. 
* NULL in case there is no more memory available to allocate. 
*/
void *FSAAlloc(FSA_t *allocator);

/*DESCRIPTION:
* Frees block of memory from Allocator. 
* If provided block pointer points outside of memory managed by allocator,
* behavior is undefined.  
*
*	@param
*	allocator - pointer to fixed size allocator	
*	block - pointer to a block to be free	
*
*@return
* No return.
*/
void FSAFree(FSA_t *allocator, void *block);

/*DESCRIPTION:
* Counts the number of free blocks
*
*	@param
*	allocator - pointer to fixed size allocator
*
*@return
* The amount of free blocks
*/
size_t FSACountFree(const FSA_t *allocator);

#endif /* __FSA_H__ */
