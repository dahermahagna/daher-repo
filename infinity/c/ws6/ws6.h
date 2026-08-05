#ifndef __WS6_H__
#define __WS6_H__

#include <stddef.h> /* size_t */

/*DESCRIPTION: 
*the function receives two unsigned numbers and return the result of x*(2^y)
*@param
*x: number1 unsigned int variable
*y: number2 unsigned int variable
*
*@return
* number1 multiply by 2 powered number2 -  x*(2^y)
*/
long Pow2(unsigned int x ,unsigned int y);

/*DESCRIPTION:
*the function checks if n it is a power of 2
*@param
*n: unsigned int
@return
*1 if n is power of 2, 0 otherwise .  
*/
int IsPow(unsigned int n);

int IsPowLoop(unsigned int n);

/*DESCRIPTION:
*the function adds 1 to an int without using arithmetic operators
*@param
*int- the number you want to increase by 1
*@return
*return the int number after increasing him by 1
*/
int AddOne(int num);
int AddOneLoop(int num);

/*DESCRIPTION:
* The function receives an array of unsigned ints and prints only the ints that have exactly 3 *bits which are set to 1 (‘on’).
*@param
*arr - an array of ints
*size - the number of elements in arr
*/
void Print3BitsOn(unsigned int *arr, size_t size);


/*DESCRIPTION:
* returns 'byte mirror' of unsigned char 
*@param
*byte - unsigned char to be mirrored
@return
*the byte mirror of the parameter byte in unsigned char
*/
unsigned char ByteMirror(unsigned char byte);
unsigned char ByteMirrorLoop(unsigned char byte);

/*DESCRIPTION:
*checks if bits 2 and 6 is on (without using if)
*@param
*byte - unsigned char
*@return
* 1 - for True, 0 - for False
*/
int Is2And6BitsOn(unsigned char byte);

/*DESCRIPTION:
*checks if bits 2 or 6 is on (without using if)
*@param
*byte - unsigned char
*@return
* 1 - for True, 0 - for False
*/
int Is2Or6BitsOn(unsigned char byte);

/*DESCRIPTION:
*swap the 3 and 5 bits (without using if)
*@param
*byte - unsigned char
*@return
* the new number after the swap
*/
unsigned char Swap3And5Bits(unsigned char byte);

/*DESCRIPTION:
*The function receives unsigned int and return the closest (smaller) number that is divisible by 16 without a remainder
*
*@param
*num: unsigned int variable received from main
*
@return
*return unsigned int the closest (smaller) number that is divisible by 16 without a remainder..
*/
unsigned int FindClosestDivisibleBy16(unsigned int num);

/*DESCRIPTION:
*swap two variables without third variable
*@param
*num1: the first element to be swapped 
*num2: the second element to be swapped
@return
*void.
*/
void SwapWithOnly2Var(int *num1, int *num2);

/*DESCRIPTION:
*count the number of set bits (bits that are set to 1) in an integer .
*@param 
*num - parameter to count set bits in 
@return
*the amount of bits set to 1 in num
*/
int CountSetBits(int num);
int CountSetBitsLoop(int num);

/*DESCRIPTION:
*prints the bits in a float
*
*@param 
*num -  parameter to print bits in 
@return
*void
*/
void FloatAnalize(float num);


#endif /*__WS6_H__*/

