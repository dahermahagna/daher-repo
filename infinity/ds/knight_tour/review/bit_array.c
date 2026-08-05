#include <stdio.h>/*printf*/
#include <assert.h>/*assert*/
#include <limits.h>/*CHAR_BIT*/

#include "bit_array.h"

#define SIZE_OF_BIT_ARRAY (sizeof(bit_array_t)*CHAR_BIT)
#define ALL_BITS_ON  (0xFFFFFFFFFFFFFFFF)
#define ALL_BITS_OFF  (0x0)
#define ALL_NIBBLE_ON (0xf)
#define OFFSET 

void BitsArrPrintByNibble(char *str);

size_t BitsArrCountOnLut(bit_array_t arr);
bit_array_t BitsArrMirrorLut(bit_array_t arr);


bit_array_t BitsArrSetAll(bit_array_t arr)
{
	arr = ALL_BITS_ON; 
	return arr;
}
bit_array_t BitsArrResetAll(bit_array_t arr)
{
	arr = ALL_BITS_OFF;
	return arr;
}

bit_array_t BitsArrSetOn(bit_array_t arr, size_t idx)
{
	assert(SIZE_OF_BIT_ARRAY >= idx);

	arr |= ((size_t)1 << (idx-1));
	return (arr);

}

bit_array_t BitsArrSetOff(bit_array_t arr, size_t idx)
{
	assert(SIZE_OF_BIT_ARRAY >= idx);

	arr &= ~((size_t)1 << (idx-1));
	return arr;
}
int BitsArrGetVal(bit_array_t arr, size_t idx)
{
	assert(SIZE_OF_BIT_ARRAY >= idx);

	arr &= ((size_t)1 << (idx-1));
	return (arr != 0);
}

bit_array_t BitsArrRotR(bit_array_t arr, size_t steps)
{
	steps %= SIZE_OF_BIT_ARRAY;
	
	arr = (arr << (SIZE_OF_BIT_ARRAY-steps)) | (arr >> steps);
	return arr;
}

bit_array_t BitsArrRotL(bit_array_t arr, size_t steps)
{
	steps %= SIZE_OF_BIT_ARRAY;
	
	arr = arr << steps  | arr >> (SIZE_OF_BIT_ARRAY-steps);
	return arr;
}


size_t BitsArrCountOn(bit_array_t arr)
{
	size_t count = 0;
	while(0 != arr)
	{
		arr &= (arr-1);
		++count;
	}
	return count;
}

size_t BitsArrCountOnLut(bit_array_t arr)
{
	static const int arr_lut_count[16]={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	int i = 0 ;
	size_t count = 0;
	while(16 > i)
	{
		count += arr_lut_count [arr & ALL_NIBBLE_ON];
		arr >>= 4;
		++i;
	}
	return count;
}

size_t BitsArrCountOff(bit_array_t arr)
{
	return (SIZE_OF_BIT_ARRAY - BitsArrCountOn(arr));
}

bit_array_t BitsArrSetBit(bit_array_t arr, size_t idx, int boolean_val)
{
	assert(SIZE_OF_BIT_ARRAY >= idx);
	if(boolean_val == 1)
	{
		arr = BitsArrSetOn(arr,idx);
		return arr;
	}
	arr = BitsArrSetOff(arr,idx);
	return arr;
}

bit_array_t BitsArrMirror(bit_array_t arr)
{
	arr = (arr                        >> SIZE_OF_BIT_ARRAY/2)   |  (arr                       << SIZE_OF_BIT_ARRAY/2); 
    arr = ((arr & 0xFFFF0000FFFF0000) >> SIZE_OF_BIT_ARRAY/4)   | ((arr & 0x0000FFFF0000FFFF) << SIZE_OF_BIT_ARRAY/4); 
    arr = ((arr & 0xFF00FF00FF00FF00) >> SIZE_OF_BIT_ARRAY/8)   | ((arr & 0x00FF00FF00FF00FF) << SIZE_OF_BIT_ARRAY/8);
    arr = ((arr & 0xF0F0F0F0F0F0F0F0) >> SIZE_OF_BIT_ARRAY/16)  | ((arr & 0x0F0F0F0F0F0F0F0F) << SIZE_OF_BIT_ARRAY/16); 
    arr = ((arr & 0xCCCCCCCCCCCCCCCC) >> SIZE_OF_BIT_ARRAY/32)  | ((arr & 0x3333333333333333) << SIZE_OF_BIT_ARRAY/32); 
    arr = ((arr & 0xAAAAAAAAAAAAAAAA) >> SIZE_OF_BIT_ARRAY/64)  | ((arr & 0x5555555555555555) << SIZE_OF_BIT_ARRAY/64);
    return arr;
}

static const int arr_lut_mirror[16]={0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};

bit_array_t BitsArrMirrorLut(bit_array_t arr)
{
	int i = 0 ;
	bit_array_t mirror_arr = 0;
	while(16 > i)
	{
		mirror_arr <<= 4;
		mirror_arr |= arr_lut_mirror[arr & ALL_NIBBLE_ON];
		arr >>= 4;
		++i;
	}
    return mirror_arr;
}



bit_array_t BitsArrFlip(bit_array_t arr)
{
	return (~arr);
}

char *BitsArrToString(bit_array_t arr, char *dest)
{
	int i = SIZE_OF_BIT_ARRAY;
	dest[i--] = '\0';
	while(i >= 0)
	{	
		dest[i] = (arr & 1) + '0';
		--i;
		arr >>= 1;
	}

	return dest;
}