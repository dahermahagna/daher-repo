
#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h> /* size_t */


typedef unsigned long bit_array_t;


/*DESCRIPTION: 
*sets all bits in array to 1
*@param
*arr - array of bits to set
*@return
*arr after setting to 1
*/
bit_array_t BitsArrSetAll(bit_array_t arr);

/*DESCRIPTION: 
*sets all bits in array to 0
*@param
*arr - array of bits to set
*@return
*arr after resetting to 0
*/
bit_array_t BitsArrResetAll(bit_array_t arr);

/*DESCRIPTION: 
*sets bit in arr at idx to 1
*@param
*arr - array of bit to set
*idx - index of bit to set
*@return
*arr after set
*/
bit_array_t BitsArrSetOn(bit_array_t arr, size_t idx);


/*DESCRIPTION: 
*sets bit in arr at idx to 0 
*@param
*arr - array of bit to set
*idx - index of bit to set
*@return
* arr after set
*/
bit_array_t BitsArrSetOff(bit_array_t arr, size_t idx);

/*DESCRIPTION: 
*get the value of the index bit in arr
*@param
*arr - array of bit to get
*idx - index of bit to get
*@return
* boolean value of arr[index]
*/
int BitsArrGetVal(bit_array_t arr, size_t idx);

/*DESCRIPTION: 
*rotate array steps times to right
*@param
*bit_array - array of bit to get
*steps - number of steps to do
*@return
* rotated array
*/
bit_array_t BitsArrRotR(bit_array_t arr, size_t steps);

/*DESCRIPTION: 
*rotate arr steps times to left
*@param
*arr - array of bit to get
*steps - number of steps to do
*@return
*rotated array
*/
bit_array_t BitsArrRotL(bit_array_t arr, size_t steps);

/*DESCRIPTION: 
*count all set bit in given array
*@param
*bit_array - array of bit to count
*@return
*number of set bits in array
*/
size_t BitsArrCountOn(bit_array_t arr);

/*DESCRIPTION: 
*count all un-set bits in arr
*@param
*arr - array of bits to count
*@return
*number of un-set bits in arr
*/
size_t BitsArrCountOff(bit_array_t arr);

/*DESCRIPTION: 
*returns string representation of arr
*@param
*arr - array of bit to count
*dest- destination string
*@return
*string that represent the array
*/
char *BitsArrToString(bit_array_t arr, char *dest);

/*DESCRIPTION: 
*set bit in arr at idx to boolean_val
*@param
*arr - array of bit to set
*idx - index of bit to set
*val - value to place in bit should be 0 or 1
*@return
*array with setted bit 
*/
bit_array_t BitsArrSetBit(bit_array_t arr, size_t idx, int boolean_val);

/*DESCRIPTION: 
*swap the array so it would be miror of original arrray
*@param
*bit_array - array of bit to get
*@return
*mirrored array
*/
bit_array_t BitsArrMirror(bit_array_t arr);

/*DESCRIPTION: 
*flips the value of all bits in arr
*@param
*bit_array - array of bit to get
size- size of array
*@return
*flipped array              
*/
bit_array_t BitsArrFlip(bit_array_t arr);

#endif /* __BIT_ARRAY_H__ */
