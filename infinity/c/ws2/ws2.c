#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

void SwapInt(int *firstnum, int *secundnum);
char* Arr_Copy(char arr[]);
void Print_Address();
void Swap_size_t(size_t *swap1,size_t *swap2);
int Strlen(char str[]);
int Strcmp(char *str1, char *str2);

int main()
{
	
	int swap1 = 10,swap2 = 20,i = 0;
	char arr[] = "hellooooo";
	char *a;
	size_t swa1 = 10,swa2 = 20;
	size_t *swa1_pt = &swa1;
	size_t *swa2_pt = &swa2;
	int len_str=0;
	int strcmp_result=0;
	char arr2[]="hella";
	
	//exercise 1 swap 2 variables
	SwapInt(&swap1,&swap2);
	
	//exercise 2 copy the array
	a = Arr_Copy(arr);
	for(i = 0;i < (int)Strlen(a);i++)
		printf("%c",a[i]);
	
	free(a);

	//exersice 3 printing the map
	Print_address();
	
	//exersice 4 size_t swap
	Swap_size_t(&swa1,&swa2);
	printf("\nswa1-> %p\nswa2->%p",(void *)&swa1,(void *)&swa2);
	
	swap_size_t((size_t*)&swa1_pt,(size_t*)&swa2_pt);
	printf("\nswa1-> %p\nswa2->%p",(void *)&swa1,(void *)&swa2);

	//strlen
	len_str=Strlen(arr);
	printf("\ngodel string ->%d\n",len_str);
	printf("godel string -> %ld\n",(sizeof(arr)-1)/sizeof(arr[0]));
	
	//strcmp
	strcmp_result=Strcmp(arr,arr2);
	printf("difference between the first different chars in arrays->%d\n",strcmp_result );
	printf("godel string -> %ld\n",(sizeof(arr)-1)/sizeof(arr[0]));

	return 0;
}

void SwapInt(int *firstnum, int *secundnum)
{
	int tmp;
	/*printf("\n\nswap the numbers\n");*/
	printf("before the swap => x->%d  y->  %d",*firstnum,*secundnum);
	
	tmp = *firstnum;
	*firstnum = *secundnum;
	*secundnum = tmp;
	
	printf("\nafter the swap => x->%d  y->  %d\n",*firstnum,*secundnum);
}
char* Arr_Copy(char arr[])
{
	char *ptr_new_arr;
	int i=0;
	
	ptr_new_arr=malloc(strlen(arr)*sizeof(*arr)+1);
	
	for(i = 0 ; i < (int)strlen(arr); ++i)
		ptr_new_arr[i]=arr[i];

	return ptr_new_arr;
}

void Print_Address()
{
	static int s_i=7;
	int i=7;
	int *ptr = &i;
	int *ptr2 =(int *)malloc(sizeof(int));


	printf ("\033[33;1m\n\nvariable          value                       address              STACK/HEAP \033[0m\n");
	printf("s_i                %d                         %p        STACK\n",s_i,(void *)&s_i);
	printf("i                  %d                         %p        STACK\n",i,(void *)&i);
	printf("*ptr               %p            %p        STACK\n",(void *)ptr,(void *)&ptr);
	printf("*ptr2              %p            %p        HEAP\n",(void *)ptr2,(void *)&ptr2);
	
	
	free(ptr2);
}

void Swap_size_t(size_t *swap1,size_t *swap2)
{
	size_t tmp;
	tmp=*swap1;
	*swap1=*swap2;
	*swap2=tmp;
	
}	

int Strlen(const char *str)
{
	int count=0;
	
	while(*str)
	{
		++count;
		++str;
	}
	return count;
}


int Strcmp(char *str1, char *str2)
{
	while('\0' != *str1 && *str1 == *str2)
	{
		++str1;
		++str2;
	}
	return *str1-*str2;
}
