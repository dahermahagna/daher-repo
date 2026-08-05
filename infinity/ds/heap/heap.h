#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /* size_t */

typedef struct heap Heap_ty;

/*for c file */

/* DESCRIPTION: 
 *compare two elements data;
 * 0 -if same
 * positive if data > data_to_compare
 * negative if data < data_to_compare
 */
typedef int(*cmp_func_ty)(const void *data, const void *data_to_compare);
/* DESCRIPTION: 
 * Compares user's data
 *
 *		@param
 *		data - element
 *		data_to_compare - element to compare
 *
 * @return
 * 1 if elements is equal, 0 if not
 */
typedef int (*is_match_func_ty)(const void *data, const void *data_to_compare);

/*DESCRIPTION: 
*create new heap
*@param
* cmp - compare function
*@return
* pointer to the new heap or NULL if failed         
*/
Heap_ty *HeapCreate(cmp_func_ty cmp);

/*DESCRIPTION: 
*delete given vector -free all alocated memory
*@param
*vector - pointer to the vector to delete
*@return          
*/
void HeapDestroy(Heap_ty *heap);

/*DESCRIPTION: 
*pop out the root of the heap 
*undefined behavior if called on an empty heap 
*@param
*heap - pointer to heap
*@return
*/
void HeapPop(Heap_ty *heap);

/*DESCRIPTION: 
    *insert element to the heap 
*@param
    *heap - pointer to heap
    *element - pointer to element to insert
*@return
    * 0 in case of successfull push, non-zero in failure
*/
int HeapPush(Heap_ty *heap, void *data);

/*DESCRIPTION: 
    *returns the element at the root of the heap 
    *undefined behavior if called on an empty heap 
*@param
    *heap - pointer to heap
*@return
    * the data of the element at the root of the heap
*/
void *HeapPeek(const Heap_ty *heap);

/*DESCRIPTION: 
    * findes and removes first element that match from the heap 
*@param
    *heap - pointer to heap
    *data - data to look in the heap.
    *match func - function that check 
*@return
    *in case of successfull , non-zero in failure
*/
void *HeapRemove(Heap_ty *heap ,is_match_func_ty match_func ,void *data);

/*DESCRIPTION: 
* check whether the heap is empty
*@param
*heap - pointer to heap
*@return
* 1 in case heap is empty, non-zero if not
*/
int HeapIsEmpty(const Heap_ty *heap);

/*DESCRIPTION: 
*gives the current size of the heap.
*@param
*heap - pointer to heap
*@return
* number of current elements in the heap.
*/
size_t HeapSize(const Heap_ty *heap);





#endif /*__HEAP_H__*/