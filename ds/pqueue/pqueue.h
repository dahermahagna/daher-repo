#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#include <stddef.h> /* size_t */

typedef struct pqueue PQueue_t;

/* DESCRIPTION: 
 * Compares user's data
 *
 *		@param
 *		data - list's element
 *		data_to_compare - user's data to check against.
 *
 * @return
 * 0 if data is the same.
 * Positive value if data_to_compare should be after data.
 * Negative value if data_to_compare should be before data.
 */
typedef int (*pq_compare_func_t)(const void *data, const void *data_to_compare);

/* DESCRIPTION: 
 * Compares user's data
 *
 *		@param
 *		data - pqueue's element
 *		data_to_compare - search key
 *
 * @return
 * 1 if equal, 0 if not
 */
typedef int (*pq_is_match_func_t)(const void *data, const void *data_to_compare);

/*DESCRIPTION:
* Create new pqueue
* Created queue should be destroyed with PQDestroy() function
*
*		@param
*       compare_func - compare function
*@return
* pointer to the new pqueue or NULL if failed
*/
PQueue_t *PQCreate(pq_compare_func_t compare_func);

/*DESCRIPTION:
* deletes given pqueue and frees all allocated memory
*
*	@param
* 	pqueue - pointer to the pqueue to delete
*
*@return
*/
void PQDestroy(PQueue_t *pqueue);

/*DESCRIPTION:
* gets the amounts of elements in the pqueue
*
*	@param
* 	pqueue - pointer to the pqueue
*
*@return
*number of elements in the pqueue
*/
size_t PQSize(const PQueue_t *pqueue);

/*DESCRIPTION:
* checks if there are any elements in the pqueue
*
*	@param
* 	pqueue - pointer to the pqueue
*
*@return
* 1 if empty, 0 if not
*/
int PQIsEmpty(const PQueue_t *pqueue);

/*DESCRIPTION:
* Pushes an element to the queue according to priority of the element.
* (By default it goes at the end of the queue)
*
*	@param
* 	pqueue - pointer to pqueue
* 	data - data of element
*
*@return
* 0 on success, 1 on failure.
*/
int PQEnpqueue(PQueue_t *pqueue, void *data);

/*DESCRIPTION:
* Removes the element from the beginning of the pqueue.
* Dequeue on an empty priority queue causes undefined behavior.
*
*	@param
* 	pqueue - pointer to pqueue
*
*@return
* Pointer to depqueued element's data
*/
void *PQDepqueue(PQueue_t *pqueue);

/*DESCRIPTION:
* Get the first element of the pqueue
* Peek on an empty queue causes undefined behavior.
*
*	@param
* 	pqueue - pointer to pqueue
*
*@return
* Pointer to first element's data
*/
void *PQPeek(const PQueue_t *pqueue);

/*DESCRIPTION:
* Deletes all the elements from the pqueue
* After clear the queue is empty
* if data was allocated by user then memory leaks are possible
*	@param
* 	pqueue - pointer to the pqueue to delete all of its elements
*
*@return
*/
void PQClear(PQueue_t *pqueue);

/*DESCRIPTION:
* Search and deletes first element from the priority queue which meets user’s is_match criteria.
*
*
*	@param
* 	pqueue - pointer to the pqueue to delete all of its elements
*	is_match - pointer to the compare function
	data_to_compare - the data to be compared on the pqueue to be deleted
*
*@return
* Pointer to element's data on success: if found match and erased element, or NULL on failure.
*/
void *PQErase(PQueue_t *pqueue, pq_is_match_func_t is_match, void *data_to_compare);


#endif /* __PQUEUE_H__ */
