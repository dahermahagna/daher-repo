#include <stdio.h>

#define MAX2(a, b) (a) > (b) ? (a) : (b)

#define MAX3(a, b, c) (a) > (b) ? ((a) > (c) ? (a) : (c)) : (b) > (c) ? (b) : (c)

#define SIZEOF_VAR(x) ((char *)(&x) - (char *)(&x - 1)) 


#ifndef SIZEOF_TYPE
#define SIZEOF_TYPE(var_type) (size_t)((var_type *)0 + 1)
#endif

struct student
{	
	char *name;
	int id;

};

typedef struct student st;
int main()
{
	int i = 7;
	size_t s = 9;
	float f = 4.5;
	short sh = 2;
	

	if((7 == (MAX2(4,7))) && (-4 == (MAX2(-4,-8))))
	{
		printf("MAX2 is working correctly !\n");
	}
	else
	{
		printf("MAX2 is not working :(\n");
	}

	if((7 == (MAX3(4,7,3))) && (-4 == (MAX3(-4,-8,-5))))
	{
		printf("MAX3 is working correctly !\n");
	}
	else
	{
		printf("MAX3 is not working :(\n");
	}

	if((sizeof(i) == (SIZEOF_VAR(i))) && sizeof(s) == (SIZEOF_VAR(s)) && sizeof(sh) == (SIZEOF_VAR(sh)) && sizeof(f) == (SIZEOF_VAR(f)))
		printf("SIZEOF_VAR is working correctly !\n");
	else
		printf("SIZEOF_VAR is not working :(\n");

	if((4 == (SIZEOF_TYPE(int))) && 8 == (SIZEOF_TYPE(size_t)) && 2 == (SIZEOF_TYPE(short)) && 4 == (SIZEOF_TYPE(float)) && 16 == (SIZEOF_TYPE(st)))
		printf("SIZEOF_TYPE is working correctly !\n");
	else
		printf("SIZEOF_TYPE is not working :(\n");


	print
	return 0;


}