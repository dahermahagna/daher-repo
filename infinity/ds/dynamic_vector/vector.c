#include <stdlib.h>/*malloc, realloc, free*/
#include <assert.h>/*assert*/


#include "vector.h"

#define GROWTH_FACTOR (2) 
int VectorIsFull(Vector_t *vector);

struct vector
{
	void **elements;  
	size_t capacity; 
	int pointer; 
};

Vector_t *VectorCreate(size_t capacity)
{
	Vector_t  *vector = malloc(sizeof(Vector_t));
	if (NULL == vector)
	{
		return NULL;
	}

	vector->elements = malloc(sizeof(void *) * capacity);
	if (NULL == vector->elements)
	{
		free(vector);
		return NULL;
	}
	vector->capacity = capacity;
	
	vector->pointer = -1;

	return vector;
	
}

void VectorDestroy(Vector_t *vector) 
{
	free(vector->elements);
	vector->elements = NULL;
	free(vector);
	vector = NULL;
}

void VectorPopBack(Vector_t *vector)
{
	--vector->pointer;
}

int VectorPushBack(Vector_t *vector, void *element)
{
	assert(NULL != vector);
	if (1 == VectorIsFull(vector))
	{
		if(1 == VectorReserve(vector,vector->capacity * GROWTH_FACTOR))
			return 1;
	}

	++vector->pointer;
	vector->elements[vector->pointer] = element;
	return 0;
}

void *VectorGetElement(const Vector_t *vector, size_t index)
{
	assert(NULL != vector);
	assert(index <= (size_t)vector->pointer+1);
	return vector->elements[index-1];/*if the index is starts with 1*/
}

void VectorSetElement(Vector_t *vector, void *element, size_t index)
{
	assert(NULL != vector);
	assert(index <= (size_t)vector->pointer + 1);
	vector->elements[index-1] = element;
}

size_t VectorSize(const Vector_t *vector)
{
	assert(NULL != vector);
	return vector->pointer + 1;
}

size_t VectorCapacity(const Vector_t *vector)
{
	assert(NULL != vector);
	return vector->capacity;
}

int VectorReserve(Vector_t *vector, size_t new_capacity)
{
	void **new_vector_element = NULL;
	assert(NULL != vector);
	
	new_vector_element = realloc(vector->elements,(sizeof(void*) * (new_capacity)));

	if (NULL == new_vector_element)
	{
		return 1;
	}
	vector->elements = new_vector_element;
	vector->capacity = new_capacity;

	return 0;

}

int VectorShrinkToFit(Vector_t *vector)
{
	assert(NULL != vector);
	if(1 == VectorReserve(vector, vector->pointer + 1))
	{
		return 1;
	}
	return 0 ;
}

int VectorIsFull(Vector_t *vector)
{
	return ((size_t)vector->pointer == vector->capacity - 1);
}



