#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h> /* size_t */

typedef struct slist Slist_t;
typedef struct slist_node *Slist_iter_t;

/*DESCRIPTION: 
* compares user's data
*@param
*data - list's element
*data_to_compare - search key
*@return
*1 if equal, 0 if not
*/
typedef int (*is_match_func_t)(const void *data, const void *data_to_compare);

/*DESCRIPTION: 
* does action on data
*@param
*data - list's element
*param - user input
*@return
* return 0 for success or any value of user's function for fail.
*/
typedef int (*action_func_t)(void *data, void *param);

/*DESCRIPTION: 
*Returns iterator to the first element of the list
*If the list is empty, the returned iterator will be equal to SlistEnd.
*@param
*list - list of elements
*@return
*iterator at the beginning of the list    
*/
Slist_iter_t SlistBegin(const Slist_t *list);

/*DESCRIPTION: 
*Returns an iterator to the element following the last element of the list.
*This element acts as a placeholder; 
*Attempting to access it results in undefined behavior.
*@param
*list - list of elements
*@return
*pointer to the iterator at the end of the list    
*/
Slist_iter_t SlistEnd(const Slist_t *list);

/*DESCRIPTION: 
*returns iterator to the next element of the list
*next on SlistEnd results in undefined behavior
*@param
*iter - iterator to an element of a list
*@return
* pointer to the iterator at the next element of the list    
*/
Slist_iter_t SlistNext(Slist_iter_t iter);


/*DESCRIPTION: 
*checks if two iterators point to same element
*@param
*one - iterator to an element of a list
*two - iterator to an element of a list
*@return
* 1 if equal, 0 if not  
*/
int SlistIsSameIter(Slist_iter_t one, Slist_iter_t two);

/*DESCRIPTION: 
*creates a new list
*@param
*
*@return
* pointer to the new list, NULL on error
*/
Slist_t *SlistCreate(void);

/*DESCRIPTION: 
*destroys the list
*@param
*list - a list of elements
*@return
* 
*/
void SlistDestroy(Slist_t *list);

/*DESCRIPTION: 
*gets data from iterator
*@param
*iter - pointer to iterator
*@return
*data from iterator
*/
void *SlistGetData(const Slist_iter_t iter);

/*DESCRIPTION: 
*sets data at iterator
*@param
*iter - pointer to iterator
*data - data to write
*@return
*
*/
void SlistSetData(Slist_iter_t iter, void *data);

/*DESCRIPTION: 
*inserts element before Slist_iter_t where
*O(1), except when malloc fails and we need to return End in O(n)
*@param
*where - pointer to iterator
*data - data to write
*@return
*Iterator to the inserted value, in case of failure returns SlistEnd
*/
Slist_iter_t SlistInsert(Slist_iter_t where, void *data);

/*DESCRIPTION: 
*removes element at iter 
*@param
*iter - pointer to iterator
*@return
*Iterator to the next element after removed
*/
Slist_iter_t SlistRemove(Slist_iter_t iter);


/*DESCRIPTION: 
*counts elements in list
*@param
*list - pointer to list of elements
*@return
*the amount of elements
*/
size_t SlistCount(const Slist_t *list);

/*DESCRIPTION: 
*returns the first element with data with range [from, to)
*@param
*from - iter to beginning of range
*to - iter to the first element out of range
*is_match_func - function pointer to function that compares element's data
*data_to_compare - key for search
*@return
*iterator to the found element, if not found Slist_iter_t to
*/
Slist_iter_t SlistFind(Slist_iter_t from, Slist_iter_t to, 
						const void *data_to_compare, is_match_func_t is_match_func);

/*DESCRIPTION: 
*call action_funct_t for each element in the range [from, to)
*@param
*from - iter to beginning of range
*to - iter to the first element out of range
*action_func - function pointer to function to execute
*param - user input
*@return
*0 is case of successful execution of the action_func_t function. In case
*user’s action function is not successful return  any value of user's function.
*/
int SlistForEach(Slist_iter_t from, Slist_iter_t to, 
									action_func_t action_func, void * param);

/*DESCRIPTION:
* takes all elements from src list, in order,
* and appends them to the end of dest list.
*
*@param
* dest - pointer to the list to append to
* src - pointer to list to be appended
*
*@return
*/
void SlistAppend(Slist_t *dest, Slist_t *src);

#endif /*__SLIST_H__*/