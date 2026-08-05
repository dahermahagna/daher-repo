#include <stdio.h>/*printf*/
#include "ws6.h"

int IsPowLoop2(unsigned int n);

int main()
{	
	/*variables for the array exercise*/
	unsigned int arr[5] = {3,67,50,45,97};


	/*variables for swaping exercise*/

	int swap1 = 0, swap2 = 0;
	int swap3 = 0, swap4 = 0;
	int swap5 = 0, swap6 = 0;


	/*variables for float exercise*/
	float f = 0;

	

	if (2 * 16== Pow2(2,4) && 4 * 8 == Pow2(4,3) && 5 == Pow2(5,0))
		printf("pwo is workink correcty !\n");
	else 
		printf("pow is not working :(\n");


	if(1 == IsPowLoop(256) && 0 == IsPowLoop(73) && 0 == IsPowLoop(0) && 1 == IsPowLoop(1))
		printf("IsPowLoop is workink correcty !\n");
	else 
		printf("IsPowLoop is not working :(\n");

	if(1 == IsPowLoop2(256) && 0 == IsPowLoop2(73) && 0 == IsPowLoop2(0) && 1 == IsPowLoop2(1))
		printf("IsPowLoop2 is workink correcty !\n");
	else 
		printf("IsPowLoop2 is not working :(\n");


	if(1 == IsPow(512) && 0 == IsPow(73) && 0 == IsPow(0) && 1 == IsPow(64) && 1 == IsPow(1))
		printf("IsPow is workink correcty !\n");
	else 
		printf("IsPow is not working :(\n");


	if(513 == AddOne(512) && 79 == AddOne(78) && 1 == AddOne(0))
		printf("AddOne is workink correcty !\n");
	else 
		printf("AddOne is not working :(\n");

	if(16 == AddOneLoop(15) && 79 == AddOneLoop(78) && 1 == AddOneLoop(0))
		printf("AddOneLoop is workink correcty !\n");
	else 
		printf("AddOneLoop is not working :(\n");


	printf("Print3BitsOn is workink correcty if it prints 67, 50, 97!\n");
	Print3BitsOn(arr,(sizeof(arr)/sizeof(arr[0])));

	printf("\nby ByteMirrorLoop:if its 160 its correct, the mirror of 5 is ->%d\n", ByteMirrorLoop(5));
	printf("by ByteMirrorLoop:if its 172 its correct, the mirror of 53 is ->%d\n", ByteMirrorLoop(53));
	printf("by ByteMirrorLoop:if its 24 its correct, the mirror of 24 is ->%d\n", ByteMirrorLoop(24));
	printf("by ByteMirrorLoop:if its 29 its correct, the mirror of 184 is ->%d\n", ByteMirrorLoop(184));
	
	printf("\nby ByteMirror:if its 160 its correct, the mirror of 5 is ->%d\n", ByteMirror(5));
	printf("by ByteMirror:if its 172 its correct, the mirror of 53 is ->%d\n", ByteMirror(53));
	printf("by ByteMirror:if its 24 its correct, the mirror of 24 is ->%d\n", ByteMirror(24));
	printf("by ByteMirror:if its 29 its correct, the mirror of 184 is ->%d\n", ByteMirror(184));


	if(1 == Is2And6BitsOn('2') && 1 == Is2And6BitsOn('3') && 1 == Is2And6BitsOn('b'))
		printf("Is2And6BitsOn is workink correcty !\n");
	else 
		printf("Is2And6BitsOn is not working :(\n");

	if(1 == Is2Or6BitsOn('d') && 1 == Is2Or6BitsOn('0') && 0 == Is2Or6BitsOn(0))
		printf("Is2Or6BitsOn is workink correcty !\n");
	else 
		printf("Is2Or6BitsOn is not working :(\n");


	if(57 == Swap3And5Bits(45) && 45 == Swap3And5Bits(57) && 31 == Swap3And5Bits(31) && 15 == Swap3And5Bits(27))
		printf("Swap3And5Bits is workink correcty !\n");
	else 
		printf("Swap3And5Bits is not working :(\n");

	if( 80 == FindClosestDivisibleBy16(89) && 96 == FindClosestDivisibleBy16(100) && 3680 == FindClosestDivisibleBy16(3692))
		printf("FindClosestDivisibleBy16 is workink correcty !\n");
	else 
		printf("FindClosestDivisibleBy16 is not working :(\n");


	swap1 = 10;
	swap2 = 20;
	SwapWithOnly2Var(&swap1,&swap2);
	swap3 = 0;
	swap4 = 20;
	SwapWithOnly2Var(&swap3,&swap4);
	swap5 = -10;
	swap6 = -35;
	SwapWithOnly2Var(&swap5,&swap6);

	if(swap1 == 20 && swap2 == 10 && swap3 == 20 && swap4 == 0 && swap5 == -35 && swap6 == -10)
		printf("SwapWithOnly2Var is workink correcty !\n");
	else 
		printf("SwapWithOnly2Var is not working :(\n");

	if(4 == CountSetBits(15) && 31 == CountSetBits(2147483647) && 1 == CountSetBits(-2147483648) && 32 == CountSetBits(-1))
		printf("CountSetBits is workink correcty !\n");
	else 
		printf("CountSetBits is not working :(\n");

	if(4 == CountSetBitsLoop(15) && 4 == CountSetBitsLoop(101) && 1 == CountSetBitsLoop(-2147483648) && 32 == CountSetBitsLoop(-1))
		printf("CountSetBitsLoop is workink correcty !\n");
	else 
		printf("CountSetBitsLoop is not working :(\n");


	f = 1;
	FloatAnalize(f);
	




	 
	return 0;
}