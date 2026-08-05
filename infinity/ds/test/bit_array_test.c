#include <stdio.h>/*printf*/
#include <string.h>/*strcmp*/

#include "bit_array.h"

int strcmp (const char* str1, const char* str2);

#define SIZE_OF_BIT_ARRAY_WITH_TREMINAL (65)
#define ALL_BITS_ON  (0xFFFFFFFFFFFFFFFF)
#define ALL_BITS_OFF  (0x0)
#define ALL_NIBBLE_ON (0xf)


size_t BitsArrCountOnLut(bit_array_t x);
bit_array_t BitsArrMirrorLut(bit_array_t arr);

void BitsArrResetAllTest();
void BitsArrSetAllTest();
void BitsArrSetOnTest();
void BitsArrSetOffTest();
void BitsArrGetValTest();
void BitsArrRotRTest();
void BitsArrRotLTest();
void BitsArrCountOnTest();
void BitsArrCountOffTest();
void BitsArrCountOnLutTest();
void BitsArrSetBitTest();
void BitsArrMirrorTest();
void BitsArrMirrorLutTest();
void BitsArrFlipTest();

int main()
{		
	
	BitsArrSetAllTest();
	BitsArrResetAllTest();
	BitsArrSetOnTest();
	BitsArrSetOffTest();
	BitsArrGetValTest();
	BitsArrRotRTest();
	BitsArrRotLTest();
	BitsArrCountOnTest();
	BitsArrCountOnLutTest();
	BitsArrCountOffTest();
	BitsArrSetBitTest();
	BitsArrMirrorTest();
	BitsArrMirrorLutTest();
	BitsArrFlipTest();
	return 0;

}

void BitsArrSetAllTest()
{
	
	if(ALL_BITS_ON == BitsArrSetAll(ALL_BITS_OFF))
	{
		printf("BitsArrSetAll     \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrSetAll \033[1;31mis not working correctly :(\033[0m\n");
	}
}
void BitsArrResetAllTest()
{
	if(ALL_BITS_OFF == BitsArrResetAll(0x53535356))
	{
		printf("BitsArrResetAll   \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrResetAll \033[1;31mis not working correctly :(\033[0m\n");
	}
}
void BitsArrSetOnTest()
{
	char str_test1[SIZE_OF_BIT_ARRAY_WITH_TREMINAL] = {0};
	char str_test2[SIZE_OF_BIT_ARRAY_WITH_TREMINAL] = {0};
	char str_test3[SIZE_OF_BIT_ARRAY_WITH_TREMINAL] = {0};
	char str_test4[SIZE_OF_BIT_ARRAY_WITH_TREMINAL] = {0};

	if (0 == strcmp((BitsArrToString(BitsArrSetOn(0x5A5F,6),str_test1))  ,  BitsArrToString(0x5A7F,str_test2)  ) &&
		0 == strcmp((BitsArrToString(BitsArrSetOn(0,18),str_test3))  ,   BitsArrToString(131072,str_test4) ))
	{
		printf("BitsArrSetOn      \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrSetOn \033[1;31mis not working correctly :(\033[0m\n");
	}


}

void BitsArrSetOffTest()
{
	char str_test1[SIZE_OF_BIT_ARRAY_WITH_TREMINAL] = {0};
	char str_test2[SIZE_OF_BIT_ARRAY_WITH_TREMINAL] = {0};
	char str_test3[SIZE_OF_BIT_ARRAY_WITH_TREMINAL] = {0};
	char str_test4[SIZE_OF_BIT_ARRAY_WITH_TREMINAL] = {0};

	if (0 == strcmp((BitsArrToString(BitsArrSetOff(0x5A5F,7),str_test1))  ,  BitsArrToString(0x5A1F,str_test2)) &&
		0 == strcmp(( BitsArrToString(BitsArrSetOff(ALL_BITS_ON,18),str_test3)) ,BitsArrToString(0xFFFFFFFFFFFDFFFF,str_test4)))
	{
		printf("BitsArrSetOff     \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrSetOff \033[1;31mis not working correctly :(\033[0m\n");
	}


}

void BitsArrGetValTest()
{
	
	if (1 == BitsArrGetVal(ALL_BITS_ON,64) && 0 == BitsArrGetVal(0xFFFFFFFFFFFDFFFF,18))
	
	{
		printf("BitsArrGetVal     \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrGetVal \033[1;31mis not working correctly :(\033[0m\n");
	}


}
void BitsArrRotRTest()
{
	
	if (0xD4B2000000004CF6 == BitsArrRotR(0x267B6A59,15) && 0x400000000000 == BitsArrRotR(1,18))
	
	{
		printf("BitsArrRotR       \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrRotR \033[1;31mis not working correctly :(\033[0m\n");
	}


}
void BitsArrRotLTest()
{
	
	if (0x267B6A59 == BitsArrRotL(0xD4B2000000004CF6,15) && 0x40000 == BitsArrRotL(1,18))
	
	{
		printf("BitsArrRotL       \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrRotL \033[1;31mis not working correctly :(\033[0m\n");
	}


}


void BitsArrCountOnTest()
{
	
	if (17 == BitsArrCountOn(0xD4B2000000004CF6) && 1 == BitsArrCountOn(0x40000))
	
	{
		printf("BitsArrCountOn    \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrCountOn \033[1;31mis not working correctly :(\033[0m\n");
	}


}
void BitsArrCountOnLutTest()
{
	
	if (17 == BitsArrCountOnLut(0xD4B2000000004CF6) && 1 == BitsArrCountOnLut(0x40000))
	
	{
		printf("BitsArrCountOnLut \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrCountOnLut \033[1;31mis not working correctly :(\033[0m\n");
	}


}
void BitsArrCountOffTest()
{
	
	if (47 == BitsArrCountOff(0xD4B2000000004CF6) && 63 == BitsArrCountOff(0x40000))
	
	{
		printf("BitsArrCountOff   \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrCountOff \033[1;31mis not working correctly :(\033[0m\n");
	}


}
void BitsArrSetBitTest()
{
	
	if (0 == BitsArrGetVal(BitsArrSetBit(ALL_BITS_ON,64,0),64) && 1 == BitsArrGetVal(BitsArrSetBit(ALL_BITS_OFF,64,1),64))
	
	{
		printf("BitsArrSetBit     \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrSetBit \033[1;31mis not working correctly :(\033[0m\n");
	}


}


void BitsArrMirrorTest()
{
	
	if (ALL_BITS_ON == BitsArrMirror(ALL_BITS_ON) && 0x25 == BitsArrMirror(0xA400000000000000))
	
	{
		printf("BitsArrMirror     \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrMirror \033[1;31mis not working correctly :(\033[0m\n");
	}


}

void BitsArrMirrorLutTest()
{
	
	if (ALL_BITS_ON == BitsArrMirrorLut(ALL_BITS_ON) && 0x25 == BitsArrMirrorLut(0xA400000000000000))
	
	{
		printf("BitsArrMirrorLut  \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrMirrorLut \033[1;31mis not working correctly :(\033[0m\n");
	}


}



void BitsArrFlipTest()
{
	
	if (0x0 == BitsArrFlip(ALL_BITS_ON) && 0x5BFFFFFFFFFFFFFF == BitsArrFlip(0xA400000000000000))
	
	{
		printf("BitsArrFlip       \033[1;32mis working correctly !\033[0m\n");
	}
	else
	{
		printf("BitsArrFlip \033[1;31mis not working correctly :(\033[0m\n");
	}


}
