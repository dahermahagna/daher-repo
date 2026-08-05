/*
 * NAME:       Hash
 * AUTHORS:    OL94
 * VERSION:    0.1
 */

#ifndef __HASH_H__
#define __HASH_H__

#include <stddef.h> /* size_t */

typedef struct hash_table hash_table_ty;

/* DESCRIPTION: 
 * Maps user's data into index
 *
 *		@param
 *		data - user's data
 *
 * @return
 * generated index starting from 0 
 */
typedef size_t (*hash_func_ty)(const void *data);

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

/* DESCRIPTION: 
 * Does action on data
 * 		
 *		@param
 * 		data - element from hash table
 * 		param - user input
 *
 * @return
 * 0 on Success or any value of user's function on failure.
 */
typedef int (*action_func_ty)(void *data, void *param);

/* DESCRIPTION: 
 * Creates a new hash table
 * Created hash table should be destroyed with HashDestroy() function
 *
 *		@param
 *  	hash_func - hashing function to generate keys for user's value
 *      is_match_func - function pointer to function that compares element's data
 *      capacity - size of the hash array 
 * 
 * @return
 * Pointer to the new hash table, NULL on failure
*/
hash_table_ty *HashCreate(hash_func_ty hash_func, is_match_func_ty is_match_func, size_t capacity);

/* DESCRIPTION: 
 * Destroys the hash table
 * 		
 *		@param
 *		hash_table - pointer to a hash table to be destroyed
 * 
 * @return
 * No return
*/
void HashDestroy(hash_table_ty *hash_table);

/* DESCRIPTION: 
 * Returns boolean value whether hash table is empty
 *
 *		@param
 *		hash_table - hash table of elements
 *
 * @return
 * 1 if empty, 0 if not  
*/
int HashIsEmpty(const hash_table_ty *hash_table);

/* DESCRIPTION: 
 * Counts elements in hash table
 *
 * 		@param
 * 		hash_table - pointer to hash table of elements
 *
 * @return
 * The amount of elements in the hash table
*/
size_t HashSize(const hash_table_ty *hash_table);

/* DESCRIPTION: 
 * Inserts element to hash table 
 * if data already exist, it will be overwritten
 * 		@param
 *		hash_table - pointer to a hash table to be inserted in
 * 		data - data to insert
 * 
 * @return
 * 0 on success, 1 on failure
*/
int HashInsert(hash_table_ty *hash_table, void *data);

/* DESCRIPTION: 
 * Removes element with data 
 *
 * 		@param
 *		hash_table - pointer to a hash table to be removed from
 * 		data - data to remove
 *
 * @return
 * No Return.
*/
void HashRemove(hash_table_ty *hash_table, const void *data);

/* DESCRIPTION: 
 * Returns the data of the first element with searched data
 * 
 *		@param
 *		hash_table - pointer to a hash table to search in
 * 		is_match_func - function pointer to function that compares element's data
 * 		data - data to search
 *
 * @return
 * the data matches to the data, if not found returns NULL
*/
void *HashFind(hash_table_ty *hash_table, const void *data);

/* DESCRIPTION: 
 * Call action funct for each element in the hash table
 * HashForEach will be stopped after the first failure of the user's function
 *
 * 		@param
 *		hash_table - pointer to a hash table 
 * 		action_func - function pointer to function to execute
 * 		param - user input
 *
 * @return
 * 0 is case of successful execution of the action function on all elements
 * In case user’s action function is not successful, return any value of the user's function.
*/
int HashForEach(hash_table_ty *hash_table, action_func_ty action_func, void *param);

#endif /*__HASH_H__*/

