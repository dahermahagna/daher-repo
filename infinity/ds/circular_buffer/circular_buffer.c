#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /*memcpy*/

#include "circular_buffer.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *) 0)->ELEMENT))

#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define FREE_BYTE (1)
#define EMPTY (0)

/*reviewd by tomer*/

struct CBuffer
{
	size_t capacity;
	size_t read_index;
	size_t write_index;
	char buffer_array[1];
};

CBuffer_t *CBufferCreate(size_t capacity)
{
	CBuffer_t *new_cbuffer = (CBuffer_t *)malloc(OFFSETOF(struct CBuffer,buffer_array) + capacity + FREE_BYTE);
	if (NULL == new_cbuffer)
	{
		return NULL;
	}

	new_cbuffer->capacity = capacity + FREE_BYTE;
	new_cbuffer->read_index = 0;
	new_cbuffer->write_index = 0;

	return new_cbuffer;
}

void CBufferDestroy(CBuffer_t *cbuffer)
{
	assert(NULL != cbuffer);

	free(cbuffer);
	cbuffer = NULL;
}

size_t CBufferSize(const CBuffer_t *cbuffer)
{

	assert(NULL != cbuffer);

	return (cbuffer->write_index - cbuffer->read_index + cbuffer->capacity) % cbuffer->capacity;
}


int CBufferIsEmpty(const CBuffer_t *cbuffer)
{
	assert(NULL != cbuffer);

	return (EMPTY == CBufferSize(cbuffer));
}

size_t CBufferFreeSpace(const CBuffer_t *cbuffer)
{
	assert(NULL != cbuffer);

	return (cbuffer->capacity - FREE_BYTE - CBufferSize(cbuffer));
}

ssize_t CBufferRead(CBuffer_t *cbuffer, void *output, size_t count_bytes)
{
	size_t first_memcpy_Steps = 0;
	size_t second_memcpy_steps = 0;

	assert(NULL != cbuffer);
	assert(NULL != output);

	count_bytes = MIN(CBufferSize(cbuffer), count_bytes);

	first_memcpy_Steps = MIN(count_bytes, (cbuffer->capacity - cbuffer->read_index));
	memcpy(output, (void*)&cbuffer->buffer_array[cbuffer->read_index],first_memcpy_Steps);
	cbuffer->read_index = cbuffer->read_index + first_memcpy_Steps;

	second_memcpy_steps = count_bytes - first_memcpy_Steps;
	memcpy(((char *)output) + first_memcpy_Steps, (void *)&cbuffer->buffer_array[0],second_memcpy_steps);
	cbuffer->read_index = (cbuffer->read_index + second_memcpy_steps) % cbuffer->capacity;

	return count_bytes;

}

ssize_t CBufferWrite(CBuffer_t *cbuffer, const void *input, size_t count_bytes)
{

	size_t first_memcpy_Steps = 0;
	size_t second_memcpy_steps = 0;

	assert(NULL != cbuffer);
	assert(NULL != input);

	count_bytes = MIN(CBufferFreeSpace(cbuffer), count_bytes);

	first_memcpy_Steps = MIN(count_bytes, (cbuffer->capacity - cbuffer->write_index));
	memcpy((void *)&cbuffer->buffer_array[cbuffer->write_index], input ,first_memcpy_Steps);
	cbuffer->write_index = cbuffer->write_index + first_memcpy_Steps;

	second_memcpy_steps = (count_bytes - first_memcpy_Steps);
	memcpy((void *)&cbuffer->buffer_array[0],((char *)input) + first_memcpy_Steps ,second_memcpy_steps);
	cbuffer->write_index = (cbuffer->write_index + second_memcpy_steps) % cbuffer->capacity ;
	
	return (ssize_t)count_bytes;
}