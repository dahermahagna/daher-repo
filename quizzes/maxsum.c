#include <stdio.h>
int MaxSum(int arr[],int length, int *first_index, int *last_index);
int main()
{
	int arr[9]={-2,1,-3,4,-1,2,3,-5,4};
	int first,last,max_sum;
	max_sum = MaxSum(arr,9,&first,&last);
	printf("max sum is %d\nfirst_index is  %d\nlast index is %d\n", max_sum , first, last);
	return 0;
}
int MaxSum(int arr[],int length, int *first_index, int *last_index)
{
	int i = 0, max_sum = 0, total_sum = 0, min_sum = 0, min = 0;
	*first_index = 0;
	*last_index = 0;
	while(i < length)
	{	


		if(arr[i] + total_sum > total_sum)
		{
			first_index
		}

		else 
		{
			*first_index = min;
			*last_index = i;
			max_sum = total_sum + arr[i];
		}
		total_sum += arr[i];
		++i;
	}
	return max_sum;
}