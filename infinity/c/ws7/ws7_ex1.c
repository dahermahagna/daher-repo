#include <stdio.h>/*printf sprintf*/
#include <string.h>/*strcpy strlen*/
#include <stdlib.h>/*malloc realloc*/

void PrintInt(const void *num);
int AddInt (void **num , int add_num);
void FreeNothing (void * num);

void PrintFloat(const void *num);
int AddFloat (void **num, int add_num);

void PrintString(const void *str);
int AddString(void **str , int add_num);
void FreeString(void * str);

enum State {WORKING, ERROR_ADDING};

#define NUM_OF_ELEMENTS (5)
#define TERMINATOR (1)


typedef void (*print_func_t)(const void*);
typedef int (*add_func_t)(void**, int);
typedef void (*free_func_t)(void*);

typedef struct function_struct
{
	print_func_t print_var;
	add_func_t  add_to_var;
	free_func_t free_var;
	
}function_struct_t;

static const function_struct_t int_functions = {PrintInt, AddInt,FreeNothing};
static const function_struct_t float_functions = {PrintFloat, AddFloat,FreeNothing};
static const function_struct_t string_functions = {PrintString, AddString,FreeString};


struct element
{
	void *data;
	const function_struct_t *operations;
};

void PrintInt(const void *num)
{
	printf("integer number: %d\n", *(int*)&num);
}

int AddInt(void **num, int add_num)
{
	*(int*)num = *(int*)num + add_num;
	
	return (WORKING);
}

void FreeNothing(void * num)
{
	(void) num;
}
void PrintFloat(const void *num)
{

	printf("float number  : %f\n", *(float *)&num);
}

int AddFloat(void **num, int add_num)
{
	*(float *)num = *(float *)num + (float)add_num;

	return (WORKING);
}


void PrintString(const void *str)
{
	printf("string        : %s\n", (char*)str);
}

int AddString(void **str , int add_num)
{

	int mirror_int = 0, add_num_dub = add_num, int_len = 0;
	assert(str != NULL);
	while(0 != add_num)
	{
		mirror_int = mirror_int * 10 + add_num % 10;
		++int_len;
		add_num /= 10;
	}

	*str = (char*) realloc(*str, strlen((char*)*str)+int_len + TERMINATOR);
	if(NULL == str)
	{
		return ERROR_ADDING;
	}
	sprintf(*(char**)(str), "%s%d",(char*)*(str), add_num_dub);

	return WORKING;
}
void FreeString(void *str)
{
	free(str);
	str = NULL;
	
}


void IntInitiation(struct element arr[], int index, int num)
{
	*(int *)&arr[index].data = num;
	arr[index].operations = &int_functions;
}
void FloatInitiation(struct element arr[], int index, float num)
{
	*(float *)&arr[index].data = num;
	arr[index].operations = &float_functions;
}
void StringInitiation(struct element arr[], int index, char *str)
{
	arr[index].data = (char*)malloc(strlen(str)+1);
	
	arr[index].data = strcpy(arr[index].data,str);
	arr[index].operations = &string_functions;
}


int main()
{
	struct element arr_of_elements[NUM_OF_ELEMENTS] = {0};
	int i = 0;

	FloatInitiation(arr_of_elements,0,4.2);
	FloatInitiation(arr_of_elements,1,6.7);
	StringInitiation(arr_of_elements,2,"chapter");
	IntInitiation(arr_of_elements,3,12);
	FloatInitiation(arr_of_elements,4,56.32);

	printf("\033[31;1m before adding to the elements \033[0m\n");
	for(i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		printf("\033[3%d;1m(%d)\033[0m",i+2,i);
		arr_of_elements[i].operations->print_var(arr_of_elements[i].data);
	}

	for(i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		arr_of_elements[i].operations->add_to_var(&arr_of_elements[i].data,10);
	}

	printf("\033[33;1mafter adding to the elements\033[0m\n");
	for(i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		printf("\033[3%d;1m(%d)\033[0m",i+2,i);
		arr_of_elements[i].operations->print_var(arr_of_elements[i].data);
	}
	

	for(i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		printf("freeing element num \033[3%d;1m(%d)\033[0m\n",i+2,i);
		arr_of_elements[i].operations->free_var(arr_of_elements[i].data);
	}

	return 0;

}