#include <stdlib.h> /*abs*/
#include <assert.h> /*assert*/

#include "vsa.h"

/*------------------------MACRO---------------------------*/

#define SIZE_OF_WORD (sizeof(size_t))
#define SIZE_OF_VSA_STRUCT sizeof(struct variable_sized_allocator)
#define MAX(x,y) ((x) < (y) ? (y) : (x))
#define DOUBLE (2)
#define END_OF_VSA (0)
#define SPECIAL_MAGIC_NUM (6897104101114)


/*---------------FUNCTION DECLERATION---------------------*/

static size_t MergeFreeBlocks(size_t first_memory_size, size_t second_memory_size); 
static void *AllocateBlock(VSA_t *allocator, long size_in_bytes);
static VSA_t *GetNextBlock(VSA_t *allocator);
static VSA_t *FindEnoughBLockSize(VSA_t *runner, size_t size_in_bytes);

/*-----------------------TYPEDEF--------------------------*/

/*-----------------------STRUCTS--------------------------*/

#ifdef NDEBUG

struct variable_sized_allocator
{
	long block_size;

};

void VsaSetBlockSize(VSA_t *allocator, long memory_size)
{
	allocator->block_size memory_size - DOUBLE * SIZE_OF_VSA_STRUCT;
}

#else
struct variable_sized_allocator
{
	long block_size;
	long magic_num;
};

/*reviewed by Yoni*/

void VsaSetBlockSize(VSA_t *allocator, long memory_size)
{
	allocator->block_size = memory_size;
	allocator->magic_num = SPECIAL_MAGIC_NUM;
}
static int CheckMagicNum(VSA_t *allocator)
{
	return allocator->magic_num == SPECIAL_MAGIC_NUM;
}
#endif

/*--------------------------------------------------------*/

VSA_t *VSAInit(void *memory, size_t memory_size)
{
	VSA_t *new_vsa = NULL;

	assert(memory != NULL);
	if (memory_size < DOUBLE * SIZE_OF_VSA_STRUCT)
	{
		return NULL;
	}

	new_vsa = memory;	
	VsaSetBlockSize(new_vsa, memory_size - DOUBLE * SIZE_OF_VSA_STRUCT);

	VsaSetBlockSize(GetNextBlock(new_vsa), END_OF_VSA);

	return new_vsa;
}
/*--------------------------------------------------------*/


void *VSAAlloc(VSA_t *allocator, size_t size_in_bytes)
{
	void *return_ptr = NULL;
	VSA_t *runner = NULL;

	assert(NULL != allocator);
	assert(CheckMagicNum(allocator));

	runner = allocator;
	
	runner = FindEnoughBLockSize(runner, size_in_bytes);
	if (END_OF_VSA == runner->block_size)
	{
		return NULL;
	}

	return_ptr = AllocateBlock(runner, (long)size_in_bytes);

	return return_ptr;
}
/*--------------------------------------------------------*/

void VSAFree(void *block)
{
	VSA_t *block_to_free = NULL;
	assert(NULL != block);
	/*assert() for magic number*/

	block_to_free = (VSA_t *)((char*)block - SIZE_OF_VSA_STRUCT);
	
	block_to_free->block_size = labs(block_to_free->block_size);	
}
/*--------------------------------------------------------*/

size_t VSALargestChunkAvailable(VSA_t *allocator)
{
	size_t max = 0;
	VSA_t *runner = NULL;
	VSA_t *next_block = NULL;

	assert(NULL != allocator);
	assert(CheckMagicNum(allocator));
	
	runner = allocator;

	while (0 != runner->block_size)
	{
		next_block = GetNextBlock(runner);

		while (0 < runner->block_size && 0 < next_block->block_size)
		{
			VsaSetBlockSize(runner, MergeFreeBlocks(runner->block_size, next_block->block_size));

			next_block = GetNextBlock(runner);		
		}

		max = MAX((long)max,runner->block_size);
		runner = next_block;
	}
	return max;

}


/*-------------     static functions    -----------------*/

static VSA_t *FindEnoughBLockSize(VSA_t *runner, size_t size_in_bytes)
{
	VSA_t *next_block = NULL;

	while ((long)size_in_bytes > runner->block_size && END_OF_VSA != runner->block_size)
	{
		if (runner->block_size < 0)
		{
			runner = GetNextBlock(runner);			
		}
		else
		{
			next_block = GetNextBlock(runner);;

			if (next_block->block_size > 0)
			{
				
				runner->block_size = MergeFreeBlocks(runner->block_size, next_block->block_size);
				next_block = 0;
			}
			else
			{
				runner = (VSA_t *)((char*)runner + runner->block_size);
			}
		}
	}

	return runner;

}

static VSA_t * GetNextBlock(VSA_t *allocator)
{
	assert(NULL != allocator);
	return (VSA_t*)((char*)allocator + labs(allocator->block_size) + SIZE_OF_VSA_STRUCT);
}


static void* AllocateBlock(VSA_t *runner, long size_in_bytes)
{
	VSA_t *next_block = NULL;

	assert(NULL != runner);

	if (size_in_bytes + (long)SIZE_OF_VSA_STRUCT >= runner->block_size)
	{
		VsaSetBlockSize(runner, -runner->block_size);
	}
	else
	{
		next_block = (VSA_t*)((char*)runner + size_in_bytes + SIZE_OF_VSA_STRUCT);
		next_block->block_size = runner->block_size - size_in_bytes - SIZE_OF_VSA_STRUCT;
		
		runner->block_size = -size_in_bytes;
	}
	
	return (void*)((char*)runner + SIZE_OF_VSA_STRUCT);		
}


static size_t MergeFreeBlocks(size_t first_memory_size, size_t second_memory_size)
{
	return first_memory_size + second_memory_size + SIZE_OF_VSA_STRUCT;
}

