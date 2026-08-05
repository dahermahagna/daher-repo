#include <stdio.h>/*printf*/
#include "ws6.h"

int IsPowLoop2(unsigned int n);
int IsThreeOn(int num);


#define UNSIGNED_INT_RANGE  (4294967295)
#define UNSIGNED_CHAR_RANGE  (255)

long Pow2(unsigned int x ,unsigned int y)
{
	return (x * 1 << y);
}

int IsPow(unsigned int n)
{
	/*the power 2 numbers are one 1 and the rest are 0, if i do -1 it will be 0 and the rest 1(1000 -1-> 0111)*/
	if (n < 1)
	{
		return 0;
	}

	if ( 0 == (n & (n-1)))
	{
		return 1; 
	}
	return 0;
}

int IsPowLoop(unsigned int n)
{
	unsigned int power2 = 1;

	while (power2 < n)
	{
		power2 = power2 << 1;
	}

	return (power2 == n)
	
}

int IsPowLoop2(unsigned int n)
{
	if (1 == CountSetBits(n))
	{
		return 1;
	}
	return 0;

}

int AddOne(int num)
{
	return (-(~num));
}

int AddOneLoop(int num)
{
	int power2 = 1;
	while (power2 == (num & power2))
	{
		num = num ^ power2;
		power2 = (power2 << 1);
	}

	return num^power2;
}

int IsThreeOn(int num)
{
	int count = 0;
	while (num != 0)
	{
		if (1 == (1 & num))
		{
			++count;
		}

		num = num >> 1;
	}
	if (3 == count)
	{
		return 1;
	}
	return 0;
}

void Print3BitsOn(unsigned int *arr, size_t size)
{
	size_t i = 0; 
	while (i < size)
	{
		if (1 == IsThreeOn(arr[i]))
			printf("%d, ", arr[i]);
		
		++i;	
	}
	
}

unsigned char ByteMirror(unsigned char byte)/*it needs to be corrected*/
{


    byte = (byte >> 4) | (byte << 4); 
    byte = ((byte & 204) >> 2) | ((byte & 51) << 2); /*204 -> 1100 1100, 51 -> 0011 0011*/
    byte = ((byte & 170) >> 1) | ((byte & 85) << 1); /*170 -> 1010 1010, 85 -> 0101 0101*/

    return byte;
}


unsigned char ByteMirrorLoop(unsigned char byte)
{
	unsigned int result = 0; 
	int i = 0;
	while (i < 8)
	{
		result = result <<1;
		result = result | (byte & 1);
		byte = byte >> 1; 
		++i;
	}
	return result;
}

int Is2And6BitsOn(unsigned char byte)
{
	unsigned char result6 = (byte & 32);
	unsigned char result2 = (byte & 2);

	return (result6 >> 5 & result2 >> 1);
}

int Is2Or6BitsOn(unsigned char byte)
{
	unsigned int result6 = (byte & 32);
	unsigned int result2 = (byte & 2);

	return (result6 >> 5|result2 >> 1);
}
unsigned char Swap3And5Bits(unsigned char byte)
{
	unsigned char bit3,bit5; 
	bit3 = byte & 4;
	bit5 = byte & 16;
	bit3 = bit3 << 2;
	bit5 = bit5 >> 2;

	byte = (byte & (UNSIGNED_CHAR_RANGE - 20));/*11101011*/
	byte = byte | bit5;
	byte = byte | bit3;
	return byte;

}

unsigned int FindClosestDivisibleBy16(unsigned int num)
{
	return num&(UNSIGNED_INT_RANGE - 15);
}

void SwapWithOnly2Var(int *num1, int *num2)
{
	*num1 = *num1 ^ *num2;
	*num2 = *num1 ^ *num2;
	*num1 = *num1 ^ *num2;
}

int CountSetBits(int num)
{
	return (num & 1)+(num >> 1 & 1)+(num >> 2 & 1)+(num >> 3 & 1)+
	(num >> 4 & 1)+(num >> 5 & 1)+(num >> 6 & 1)+(num >> 7 & 1)+(num >> 8 & 1)+
	(num >> 9 & 1)+(num >> 10 & 1)+(num >> 11 & 1)+(num >> 12 & 1)+
	(num >> 13 & 1)+(num >> 14 & 1)+(num >> 15 & 1)+(num >> 16 & 1)+
	(num >> 17 & 1)+(num >> 18 & 1)+(num >> 19 & 1)+(num >> 20 & 1)+
	(num >> 21 & 1)+(num >> 22 & 1)+(num >> 23 & 1)+(num >> 24 & 1)+
	(num >> 25 & 1)+(num >> 26 & 1)+(num >> 27 & 1)+(num >> 28 & 1)+
	(num >> 29 & 1)+(num >> 30 & 1)+(num >> 31 & 1);
}



int CountSetBitsLoop(int num)
{
	int count = 0, power2 = 1; 
	while(0 != power2)
	{

		if(power2 == (num & power2))
		{
			++count;
		}
		power2 = power2 << 1;

	}

	return count;
}

void FloatAnalize(float num)
{
	unsigned int *int_ptr_to_float= (unsigned int *)&num;
	int i = 0;
	printf("\n");
	for (i = 32; i > 0; --i)
	{
		
		printf("%d", ((*int_ptr_to_float >> (i - 1)) & 1));

		if(32 == i || 25 == i)
		{
			printf(" "); /*to print every part separatly*/
		}
	}
	printf("\n");
}

