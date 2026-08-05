/*
 * NAME:       semaphore
 * AUTHORS:    OL94
 * VERSION:    0.1
 */
#include <sys/sem.h>

#ifndef __SSEMAPHORE_H__
#define __SSEMAPHORE_H__

/*DESCRIPTION:
* create new key
*
*	@param
*	int_key - int_key to create the key
* @return
*/
key_t KeyCreate(int int_key);

/*DESCRIPTION:
* create new system V senaohore
*
*	@param
* 	key - key to create new semaphore
*	val - init value to the semaphore
*
* @return
*/
int SemIdCreate(key_t key, int val);

/*DESCRIPTION:
* update the value of the semaphore
*
*	@param
* 	sem_id - the semaphore id to be updated 
*	val - init value to the semaphore
*
* @return
*/
void IncDecSem(int sem_id, int oper, int undo_flag);

#endif /* __SSEMAPHORE_H__ */
