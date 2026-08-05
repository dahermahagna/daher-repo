/*
 * NAME:       SortedList
 * AUTHORS:    OL94
 * VERSION:    0.2
 */

#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__

#include <stddef.h> /* size_t */
#include "../include/dlist.h"  /* dlist_iter_t */

typedef struct sorted_list sorted_list_t;

#ifdef NDEBUG

typedef struct 
{
    Dlist_iter_t internal_itr;
} sorted_list_iter_t;

#else

typedef struct 
{
    Dlist_iter_t internal_itr;
    sorted_list_t *list;
} sorted_list_iter_t;
    
#endif

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
typedef int (*compare_func_t)(const void *data, const void *data_to_compare);

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
typedef int (*sort_action_func_t)(void *data, void *param);

/* DESCRIPTION: 
 * Creates a new sorted list
 * Created list should be destroyed with SortedListDestroy() function
 *
 *		@param
 *  	sort_func - function to sort the list by.
 * 
 * @return
 * Pointer to the new list, NULL on error
*/
sorted_list_t *SortedListCreate(compare_func_t sort_func);


/* DESCRIPTION: 
 * Destroys the list
 * 		
 *		@param
 *		list - pointer to a list to be destroyed
 * 
 * @return
 * No return
*/
void SortedListDestroy(sorted_list_t *list);


/* DESCRIPTION: 
 * Returns boolean value whether list is empty
 *
 *		@param
 *		list - list of elements
 *
 * @return
 * 1 if true, 0 if false  
*/
int SortedListIsEmpty(const sorted_list_t *list);

/* DESCRIPTION: 
 * Counts elements in list
 *
 * 		@param
 * 		list - pointer to list of elements
 *
 * @return
 * The amount of elements
*/
size_t SortedListSize(const sorted_list_t *list);


/* DESCRIPTION: 
 * Inserts element in order with other elements
 *
 * 		@param
 *		list - 	pointer to a list to be inserted in
 * 		data - 	data to write
 * 
 * @return
 * Iterator to the inserted element, in case of failure returns SortedListEnd
*/
sorted_list_iter_t SortedListInsert(sorted_list_t *list, void *data);


/* DESCRIPTION: 
 * Removes element at iter 
 * Calling to remove with SortedListEnd is undefined.
 *
 * 		@param
 *		list - 	pointer to a list to be removed
 * 		iter -  iterator to element to be removed
 *
 * @return
 * Iterator to the next element after removed
 * Returns iterator to SortedListEnd if it’s the last element.
*/
sorted_list_iter_t SortedListRemove(sorted_list_t *list, sorted_list_iter_t iter);


/* DESCRIPTION: 
 * Returns iterator to the first element of the list
 * If the list is empty, the returned iterator will be equal to SortedListEnd.
 *		
 *		@param
 *		list - list of elements
 *
 * @return
 * Iterator at the beginning of the list    
*/
sorted_list_iter_t SortedListBegin(const sorted_list_t *list);


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
sorted_list_iter_t SortedListEnd(const sorted_list_t *list);


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
int SortedListIsSameIter(sorted_list_iter_t one, sorted_list_iter_t two);


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
void SortedListPopFront(sorted_list_t *list);


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
void SortedListPopBack(sorted_list_t *list);


/* DESCRIPTION: 
 * Returns iterator to the next element of the list
 * Next on SortedListEnd results in undefined behavior
 *
 *		@param
 *		iter - iterator to an element of a list
 *
 * @return
 * Iterator at the next element of the list    
*/
sorted_list_iter_t SortedListNext(sorted_list_iter_t iter);


/* DESCRIPTION: 
 * Returns iterator to the previous element of the list
 * Previous on SortedListBegin results in undefined behavior
 *
 * 		@param
 * 		iter - iterator to an element of a list
 *
 * @return
 * Iterator at the prev element of the list    
*/
sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter);


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
void *SortedListGetData(const sorted_list_iter_t iter);




/* DESCRIPTION: 
 * Returns the first element with searched data with range [from, to)
 * 
 *		@param
 * 		list - pointer to list
 * 		from - iter to beginning of range
 * 		to - iter to the first element out of range
 * 		data_to_compare - key for search
 *
 * @return
 * Iterator to the found element, if not found returns sorted_list_iter_t to
*/
sorted_list_iter_t SortedListFind(sorted_list_t *list, sorted_list_iter_t from, 
							sorted_list_iter_t to, const void *data_to_compare);


/* DESCRIPTION: 
 * Returns the first element with searched data with range [from, to)
 * 
 *		@param
 * 		from - iter to beginning of range
 * 		to - iter to the first element out of range
 * 		search_func - function pointer to function that compares element's data
 * 		data_to_compare - key for search
 *
 * @return
 * Iterator to the found element, if not found returns sorted_list_iter_t to
*/
sorted_list_iter_t SortedListFindIf(sorted_list_t *list, sorted_list_iter_t from, sorted_list_iter_t to,
					compare_func_t search_func, const void *data_to_compare);



/* DESCRIPTION: 
 * Call action_funct_t for each element in the range [from, to)
 * SortedListForEach will be stopped after the first failure of the user's function
 *
 * 		@param
 * 		from - iter to beginning of range
 * 		to - iter to the first element out of range
 * 		action_func - function pointer to function to execute
 * 		param - user input
 *
 * @return
 * 0 is case of successful execution of the action_func_t function. In case
 * user’s action function is not successful, return any value of the user's function.
*/
int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, 
									action_func_t action_func, void *param);


/* DESCRIPTION:
 * Merge all elements from both lists to dest. Source list will be 
 * empty after merge.
 * Undefined behavior if sort_func of lists are different. 
 *
 *		@param
 * 		dest - pointer to list where all element will be after merge 
 *		src - list to take elements for merge
 *
 * @return
 * Returns pointer to a merged list. 
*/
sorted_list_t *SortedListMerge(sorted_list_t *dest, sorted_list_t *src);

 
#endif /*__SORTED_LIST_H__*/

