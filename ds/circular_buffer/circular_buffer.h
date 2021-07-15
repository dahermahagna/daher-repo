#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <sys/types.h> /* ssize_t , size_t */
typedef struct CBuffer CBuffer_t;


/* DESCRIPTION: 
 * Create a circular buffer for specified size.
 * Created buffer should be destroyed with CBufferDestroy() function
 *
 * 		@param
 * 		capacity - size of buffer in bytes
 *
 * @return
 * Pointer to allocated buffer on SUCCESS, on FAIL returns NULL.
*/
CBuffer_t *CBufferCreate(size_t capacity);


/* DESCRIPTION: 
 * Destroys the circular cbuffer.
 * Must be called on each CBuffer created.
 *
 * 		@param
 *		cbuffer - pointer to the cbuffer
 *
 * @return
 * No return.
*/
void CBufferDestroy(CBuffer_t *cbuffer);


/* DESCRIPTION:
 * Gives the amount of bytes written in the cbuffer
 *
 *		@param
 * 		cbuffer - pointer to the cbuffer
 *
 * @return
 * Number of bytes in the cbuffer
*/
size_t CBufferSize(const CBuffer_t *cbuffer);


/* DESCRIPTION:
 * Gives the amount of bytes available to insert.
 *
 *		param
 * 		cbuffer - pointer to the cbuffer
 *
 * @return
 * Amount of free bytes in the cbuffer. 
*/
size_t CBufferFreeSpace(const CBuffer_t *cbuffer);


/* DESCRIPTION:
 * Checks if there are any bytes written in the cbuffer
 *
 *		@param
 * 		cbuffer - pointer to the cbuffer
 *
 * @return
 * 1 if cbuffer empty, 0 if not.
*/
int CBufferIsEmpty(const CBuffer_t *cbuffer);


/* DESCRIPTION:
 * Read given number of bytes from cbuffer to output.
 * Output must have enough memory to hold count_bytes. 
 * Undefined behavior if output size is smaller than count_bytes.
 * If buffer size is less than count_bytes not all bytes are read.
 *
 *		@param
 * 		cbuffer - pointer to the cbuffer
 * 		output - container to the bytes read
 * 		count_bytes - number of bytes to read from buffer
 *
 * @return
 * Returns number of bytes read. 
 * Returns 0 if buffer is empty.
 * Returns -1 in case of error.
*/
ssize_t CBufferRead(CBuffer_t *cbuffer, void *output, size_t count_bytes);


/* DESCRIPTION:
 * Write count_bytes number of bytes from output to cbuffer.
 * If buffer is full not all bytes are written.
 *
 *		@param
 * 		cbuffer - pointer to the cbuffer
 * 		input - container of bytes to read from 
 * 		count_bytes - number of bytes to write to cbuffer
 *
 * @return
 * Returns number of bytes written.
 * Returns 0 if buffer is full. 
 * Returns -1 in case of error.
*/
ssize_t CBufferWrite(CBuffer_t *cbuffer, const void *input, size_t count_bytes);

#endif /* __CIRCULAR_BUFFER_H__ */
