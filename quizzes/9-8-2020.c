#include <stdio.h>
#include <string.h>

int main()
{
	int arr[]={1,6,456,-3,8,12};
	int i=0;
	long tmp = 0;
	int count = 0;
	long *long_ptr;
	size_t length = sizeof(arr)/sizeof arr[0];
	
	long_ptr = SumInPairsToLongs(arr, length);
	
	printf("\n");
	for(i = 0; i < (length/2-1); ++i)
		printf("%ld,",long_ptr[i] );
	printf("%ld", long_ptr[i]);
}

long *SumInPairsToLongs(int ints[], size_t numofelement)
{
	int i=0;
	long tmp = 0;
	int count = 0;
	long arr[];

	for(i = 0; i < numofelement-3; ++i)
	{

		tmp = (long)ints[i+count]+ints[i+count+1];
		*arr = (long)tmp;
		++count;
		++arr;

	}
	return &arr[0];
}