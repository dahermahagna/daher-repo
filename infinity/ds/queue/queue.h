#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /* size_t */

typedef struct queue Queue_t;

/*DESCRIPTION:
* create new queue
*
*@return
* pointer to the new queue or NULL if failed
*/
Queue_t *QCreate(void);

/*DESCRIPTION:
* deletes given queue and frees all allocated memory
*
*@param
* queue - pointer to the queue to delete
*
*@return
*/
void QDestroy(Queue_t *queue);

/*DESCRIPTION:
* gets the amounts of elements in the queue
*
*@param
* queue - pointer to the queue
*
*@return
*number of elements in the queue
*/
size_t QSize(const Queue_t *queue);

/*DESCRIPTION:
* checks if there are any elements in the queue
*
*@param
* queue - pointer to the queue
*
*@return
* 1 if empty, 0 if not
*/
int QueueIsEmpty(const Queue_t *queue);

/*DESCRIPTION:
* pushes an element to the end of the queue.
*
*@param
* queue - pointer to Queue
* data - element to insert
*
*@return
* returns 0 in case of success, and non-zero in case of failure.
*/
int QEnqueue(Queue_t *queue, void *data);

/*DESCRIPTION:
* pops the element from the beginning of the queue.
*
*@param
* queue - pointer to Queue
*
*@return
*/
void QDequeue(Queue_t *queue);

/*DESCRIPTION:
* get the first element of the queue
*
*@param
* queue - pointer to Queue
*
*@return
* the first element of the queue.
*/
void *QPeek(const Queue_t *queue);

/*DESCRIPTION:
* takes all elements from src queue, in order, 
* and appends them to the end of dest queue.
* the src queue is left empty.
*
*@param
* dest - pointer to the queue to append to
* src - pointer to queue to be appended
*
*@return
* a pointer to the new joined queue.
*/
void QAppend(Queue_t *dest, Queue_t *src);


#endif /* __QUEUE_H__ */
