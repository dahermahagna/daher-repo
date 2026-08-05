#ifndef __DLIST_H__
#define __DLIST_H__

#include <stddef.h> /* size_t */

typedef struct dlist Dlist_t;
typedef struct dlist_node *Dlist_iter_t;

/*
 * NAME:       DList
 * AUTHORS:    OL94
 * VERSION:    0.1
 */

/* DESCRIPTION: 
 * Compares user's data
 *
 *		@param
 *		data - list's element
 *		data_to_compare - search key
 *
 * @return
 * 1 if equal, 0 if not
 */
typedef int (*is_match_func_t)(const void *data, const void *data_to_compare);

/* DESCRIPTION: 
 * Does action on data
 * 		
 *		@param
 * 		data - list's element
 * 		param - user input
 *
 * @return
 * 0 on Success or any value of user's function on failure.
 */
typedef int (*action_func_t)(void *data, void *param);

/* DESCRIPTION: 
 * Creates a new doubly linked list
 * Created list should be destroyed with DlistDestroy() function
 *
 *		@param
 *  	No parameters.
 * 
 * @return
 * Pointer to the new list, NULL on error
*/
Dlist_t *DlistCreate(void);


/* DESCRIPTION: 
 * Destroys the list
 * 		
 *		@param
 *		list - pointer to a list to be destroyed
 * 
 * @return
 * No return
*/
void DlistDestroy(Dlist_t *list);


/* DESCRIPTION: 
 * Returns boolean value whether list is empty
 *
 *		@param
 *		list - list of elements
 *
 * @return
 * 1 if true, 0 if false  
*/
int DlistIsEmpty(const Dlist_t *list);


/* DESCRIPTION: 
 * Counts elements in list
 *
 * 		@param
 * 		list - pointer to list of elements
 *
 * @return
 * The amount of elements
*/
size_t DlistSize(const Dlist_t *list);


/* DESCRIPTION: 
 * Inserts element before Dlist_iter_t where
 *
 * 		@param
 *		list - 	pointer to a list to be inserted in
 * 		where - iterator to the element
 * 		data - 	data to write
 * 
 * @return
 * Iterator to the inserted value, in case of failure returns DlistEnd
*/
Dlist_iter_t DlistInsert(Dlist_t *list, Dlist_iter_t where, void *data);


/* DESCRIPTION: 
 * Removes element at iter 
 * Calling to remove with DListEnd is undefined.
 *
 * 		@param
 *		list - 	pointer to a list to be removed
 * 		iter -  iterator to element to be removed
 *
 * @return
 * Iterator to the next element after removed
 * Returns iterator to DListEnd if it’s the last element.
*/
Dlist_iter_t DlistRemove(Dlist_t *list, Dlist_iter_t iter);


/* DESCRIPTION: 
 * Inserts element at beginning of a list
 *
 * 		@param
 * 		list - pointer to list
 * 		data - data to write
 * 
 * @return
 * Iterator to new element, in case of failure returns iterator to DListEnd.
*/
Dlist_iter_t DlistPushFront(Dlist_t *list, void *data);


/* DESCRIPTION: 
 * Inserts element at end of a list
 *
 * 		@param
 * 		list - pointer to list
 * 		data - data to write
 * 
 * @return
 * Iterator to new element, in case of failure returns iterator to DListEnd.
*/
Dlist_iter_t DlistPushBack(Dlist_t *list, void *data);


/* DESCRIPTION: 
 * removes first user's element of list 
 * undefined behavior on empty list 
 *
 * 	@param
 * 		list - pointer to list
 *
 * @return
 * No return
*/
void DlistPopFront(Dlist_t *list);


/* DESCRIPTION: 
 * removes last user's element of list 
 * undefined behavior on empty list 
 *
 * 	@param
 * 		list - pointer to list
 *
 * @return
 * No return
*/
void DlistPopBack(Dlist_t *list);

/* DESCRIPTION: 
 * Returns iterator to the first element of the list
 * If the list is empty, the returned iterator will be equal to DlistEnd.
 *		
 *		@param
 *		list - list of elements
 *
 * @return
 * Iterator at the beginning of the list    
*/
Dlist_iter_t DlistBegin(const Dlist_t *list);


/* DESCRIPTION: 
 * Returns an iterator to the element following the last element of the list.
 * This element acts as a placeholder; 
 * Attempting to access it results in undefined behavior.
 * 
 *		@param
 * 		list - list of elements
 *
 * @return
 * Pointer to the iterator at the end of the list    
*/
Dlist_iter_t DlistEnd(const Dlist_t *list);


/* DESCRIPTION: 
 * Checks if two iterators point to same element
 * 
 *		@param
 *		one - iterator to an element of a list
 *		two - iterator to an element of a list
 *
 * @return
 * 1 if equal, 0 if not
 */
int DlistIsSameIter(Dlist_iter_t one, Dlist_iter_t two);

/* DESCRIPTION: 
 * Returns iterator to the next element of the list
 * Next on DlistEnd results in undefined behavior
 *
 *		@param
 *		iter - iterator to an element of a list
 *
 * @return
 * Pointer to the iterator at the next element of the list    
*/
Dlist_iter_t DlistNext(Dlist_iter_t iter);


/* DESCRIPTION: 
 * Returns iterator to the previous element of the list
 * Previous on DlistBegin results in undefined behavior
 *
 * 		@param
 * 		iter - iterator to an element of a list
 *
 * @return
 * Pointer to the iterator at the next element of the list    
*/
Dlist_iter_t DlistPrev(Dlist_iter_t iter);


/* DESCRIPTION: 
 * Gets data from iterator
 * Iterator to a non-existing element causes undefined behavior.
 * 
 *		@param
 * 		iter - pointer to iterator
 *
 * @return
 * Data from iterator
*/
void *DlistGetData(const Dlist_iter_t iter);


/* DESCRIPTION: 
 * Sets data at iterator
 *
 *		@param
 *		iter - pointer to iterator
 *		data - data to write
 *
 * @return
 * No return
*/
void DlistSetData(Dlist_iter_t iter, void *data);


/* DESCRIPTION: 
 * Returns the first element with searched data with range [from, to)
 * 
 *		@param
 * 		from - iter to beginning of range
 * 		to - iter to the first element out of range
 * 		is_match_func - function pointer to function that compares element's data
 * 		data_to_compare - key for search
 *
 * @return
 * Iterator to the found element, if not found returns Dlist_iter_t to
*/
Dlist_iter_t DlistFind(Dlist_iter_t from, Dlist_iter_t to,
					is_match_func_t is_match_func, const void *data_to_compare);


/* DESCRIPTION: 
 * Returns the elements with data with range [from, to)
 * 
 *		@param
 * 		dest - list for search result
 * 		from - iter to beginning of range
 * 		to - iter to the first element out of range
 * 		is_match_func - function pointer to function that compares element's data
 * 		data_to_compare - key for search
 *
 * @return
 * 0 on Success and non-zero on fail
*/
int DlistMultiFind(Dlist_t *dest, Dlist_iter_t from, Dlist_iter_t to,
					is_match_func_t is_match_func, const void *data_to_compare);


/* DESCRIPTION: 
 * Call action_funct_t for each element in the range [from, to)
 * DListForEach will be stopped after the first failure of the user's function
 *
 * 		@param
 * 		from - iter to beginning of range
 * 		to - iter to the first element out of range
 * 		action_func - function pointer to function to execute
 * 		param - user input
 *
 * @return
 * 0 is case of successful execution of the action_func_t function. In case
 * User’s action function is not successful, return any value of the user's function.
*/
int DlistForEach(Dlist_iter_t from, Dlist_iter_t to, 
									action_func_t action_func, void *param);


/* DESCRIPTION:
 * Takes all elements in range [src_from, src_to)
 * and moves them to the before iter where.
 *
 *		@param
 * 		where - iterator to the iter to move to
 * 		src_from - iterator to start of range
 *		src_to - iterator to end of range
 *
 * @return
 * Returns iterator to the last spliced element.
*/
Dlist_iter_t DlistSplice(Dlist_iter_t where, Dlist_iter_t src_from, 
														Dlist_iter_t src_to);

 
#endif /*__DLIST_H__*/

