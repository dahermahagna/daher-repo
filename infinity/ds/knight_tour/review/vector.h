#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h> /* size_t */

typedef struct vector Vector_t;

/*DESCRIPTION: 
*create new vector with given capacity
*@param
*capacity - max number of elements
*@return
* pointer to the new vector or NULL if failed         
*/
Vector_t *VectorCreate(size_t capacity);

/*DESCRIPTION: 
*delete given vector -free all alocated memory
*@param
*vector - pointer to the vector to delete
*@return          
*/
void VectorDestroy(Vector_t *vector);

/*DESCRIPTION: 
*pop out the last element of the vector 
*undefined behavior if called on an empty vector 
*@param
*vector - pointer to vector
*@return
*/
void VectorPopBack(Vector_t *vector);

/*DESCRIPTION: 
* insert element to the end of the vector 
* if called on a full vector - increase capacity before insertion.
*@param
*vector - pointer to vector
*element - pointer to element to insert
*@return
* 0 in case of successfull push, non-zero in failure
*/
int VectorPushBack(Vector_t *vector, void *element);

/*DESCRIPTION: 
*returns the element of a given index.
*undefined behavior if index is bigger than current size. 
*@param
*vector - pointer to vector
*index - location of requiered element. 
*@return
*pointer to the element in the requiered index. 
*/
void *VectorGetElement(const Vector_t *vector, size_t index); 

/*DESCRIPTION: 
*modify the element at given index 
*undefined behavior if index is greater than current size. 
*@param
*vector - pointer to vector
*index - location of requiered element. 
*@return
*/
void VectorSetElement(Vector_t *Vector, void *element, size_t index);

/*DESCRIPTION: 
*gives the current size of the vector.
*@param
*vector - pointer to vector
*@return
* number of current elements in the vector.
*/
size_t VectorSize(const Vector_t *vector);

/*DESCRIPTION: 
*gives the max number of elements vector can hold at the moment.
*@param
*vector - pointer to vector
*@return
* maximum number of elements vector can hold.
*/
size_t VectorCapacity(const Vector_t *vector);

/*DESCRIPTION: 
*increase capacity to new_capacity.  
*undifined behaviour if new_capacity is less than current capacity.
*@param
*vector - pointer to vector.
*new_capacity - the requested capacity. 
*@return
* 0 in case of successfull reserve, non-zero in case of failure
*/
int VectorReserve(Vector_t *Vector, size_t new_capacity); 

/*DESCRIPTION: 
*Decrease capacity to current size.
*@param
*vector - pointer to vector.
*@return
* 0 in case of successfull shrink, non-zero in case of failure 
*/
int VectorShrinkToFit(Vector_t *vector);

#endif /*__VECTOR_H__*/
