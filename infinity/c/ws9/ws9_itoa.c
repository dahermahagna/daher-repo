#include <stdio.h>/*printf*/
#include <stdlib.h>/*malloc*/
#include <assert.h> /*assert*/
#include <string.h>/*strcmp*/
/*include digit*/


int strcmp (const char* str1, const char* str2);

#define WHICH_ENDIAN (*(unsigned short*)"\0\x00FF" == 0xFF00) ? "LITTLE_ENDIAN" : "BIG_ENDIAN" /*if "value" in string quatation it will oreder it from left to right*/

/*The functions of first exercise*/
char *Itoa(int num, char *string_num);
int DigitCounter(int num, int base);
int Atoi(char * str);
int AtoiAnyBase(char * str, unsigned int base);
char* ItoaAnyBase(int num, char* str, size_t base);

/*The tests of the first exercise*/
void ItoaTest();
void AtoiTest(char * str);
void MultipleItoaTest();
void AtoiAnyBaseTest();
void ItoaAnyBaseTest();

/*The second exercise function and test*/
void CallThreeArrays();
void ThreeArrays(char str1[], char str2[],char str3[]);

/*the third and four exercises*/
void CheckIfLittleEndian();


int main()
{
	ItoaTest();
		
	MultipleItoaTest();	

	AtoiAnyBaseTest();

	ItoaAnyBaseTest();

	CallThreeArrays();

	CheckIfLittleEndian();

	return 0;
}

char *Itoa(int num, char *string_num)
{
	int count = 0;

	count = DigitCounter(num,10);

	if(NULL == string_num)
	{
		return NULL;
	}

	if(num < 0)
	{
		num *= -1;
		*string_num = '-';
		++count;
		*(string_num + count) = '\0';

	}

	while(num != 0)
	{
		*(string_num + count -1) = num%10 + 48;
		num /= 10;
		--count;
	}

	return string_num;
	
}
int DigitCounter(int num , int base)
{
	int count = 0;
	while(num != 0)
	{
		++count;
		num /= base;
	}
	return count;
}

int Atoi(char * str)
{
	int num = 0, sign = 1;
	if(*str == '-')
	{
		sign = -1;
		++str;
	}
	while(isddigit(*str))
	{
		num = num * 10 + (*str - 48); 
		++str;
	}
	return num * sign;

}
int AtoiAnyBase(char * str, unsigned int base)
{
	int num = 0, sign = 1, digit_to_add = 0;
	assert(36 >=base && 0 != base && 1 != base);
	if(*str == '-')
	{
		sign = -1;
		++str;

	}
	while('\0' != *str)
	{
		digit_to_add = (*str - 48);
		if(isupper(*str))
		{
			digit_to_add -= 7;
		}
		
		num = num * base + digit_to_add; 
		++str;
	}
	return num * sign;
}

char* ItoaAnyBase(int num, char* str, size_t base)

{
	int count = 0;
	assert(base <= 36);
	
	count = DigitCounter(num,base);

	if(NULL == str)
	{
		return NULL;
	}
	if(num <0)
	{
		num *= -1;
		*str = '-';
		++count;
		*(str + count) = '\0';
	}
	
	while(num != 0)
	{
		if(num % base < 10)
		{
			*(str + count -1) = (num % base) + 48;
		}
		else
		{
			*(str + count - 1) = (num % base) + 55;
		}
		--count;
		num /= base;
	}
	return str;

}

void ItoaTest()
{
	char itoa_test[12] = {0};




	if(0 == strcmp(Itoa(6734632, itoa_test),"6734632") && 0 == strcmp(Itoa(-25, itoa_test),"-25"))
	{
		printf("Itoa is working correctly !\n");
	}
	else
	{
		printf("Itoa is not working correctly :(\n");
	}


}


void AtoiTest(char * str)
{
	
	if(0 == atoi(str)-Atoi(str))
	{
		printf("Atoi is working correctly !\n");
	}
	else
	{
		printf("Atoi is not working correctly :(\n");
	}
}
void MultipleItoaTest()
{
	AtoiTest("0");
	AtoiTest("-20");
	AtoiTest("16");
}

void AtoiAnyBaseTest()
{
	char * str2 = "121";

	if(9 == AtoiAnyBase(str2,2))
	{
		printf("AtoiAnyBase is working correctly !\n");
	}
	else
	{
		printf("AtoiAnyBase is not working correctly :(\n");
	}
}

void ItoaAnyBaseTest()
{
	char str1[12] = {0};
	char str2[12] = {0};

	if(0 == strcmp(ItoaAnyBase (-2147483647,str1,36),"-ZIK0ZJ") && 0 == strcmp(ItoaAnyBase (1010,str2,35),"SU"))
	{
		printf("ItoaAnyBase is working correctly !\n");
	}
	else
	{
		printf("ItoaAnyBase is not working correctly :(\n");
	}

}

void CallThreeArrays()
{
	char str1[] = "asdrf7frfddfddf";
	char str2[] = "asdrf7ujgryunffffff";
	char str3[11] = "dz38owel-f";

	ThreeArrays(str1,str2,str3);
}

void ThreeArrays(char str1[], char str2[],char str3[])
{
	int i = 0, length = 0;
	int arr[255] = {0};

	length = strlen(str1);
	while(i < length )
	{
		arr[(int)str1[i]] = 1;
		++i;
	}

	
	length = strlen(str2);
	i = 0;
	while(i < length )
	{
		if(arr[(int)str2[i]] == 1)
		{
			arr[(int)str2[i]] = 2;
		}

		i++;
	}
	
	length = strlen(str3);
	i = 0;
	while(i < length )
	{
		arr[(int)str3[i]] = 0;
		++i;
	}

	printf("the characters that exist in first and second arrays and not exist in the third are: \n");
	i = 0;
	while(i < 255)
	{
		if (arr[i] == 2)
		{
			printf("(%c)", i);
		}
		++i;

	}
	printf("\n");
}

void CheckIfLittleEndian()
{
 	int i = 1; 
 	char *c = (char*)&i; 
   	if (*c == 1)     
       printf("The result of running little endian function: LITTLE_ENDIAN!\n"); 
   	else
       printf("BIG_ENDIAN\n"); 

   printf("The result of running WHICH_ENDIAN MACRO    : %s!\n",WHICH_ENDIAN);
}


