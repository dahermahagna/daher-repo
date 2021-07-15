#include <stdio.h>
#include <string.h>
int bursa(int arr[], size_t num_of_element, size_t *buy, size_t * sell);


int main()
{
	int arr[]={2,12,3,5,1,4,11,2};
	int max_profit = 0;
	size_t buy = 0, sell = 0;
	size_t length = sizeof(arr)/sizeof arr[0];
	max_profit = bursa(arr,length,&buy,&sell);

	printf("max_profit:%d\n", max_profit);
	printf("buy_index:%ld\n", buy);
	printf("Sell_index:%ld\n", sell);

	return 0;
	
}

int bursa(int arr[], size_t num_of_element, size_t *buy, size_t *sell)
{
	size_t i=i, min_buy = 0;
	int max_profit = 0;
	*buy = 0;
	*sell = 1;
	while(i < num_of_element)
	{
		if (arr[i]-arr[min_buy] > max_profit)
		{
			max_profit = arr[i]-arr[min_buy];
			*sell = i;
			*buy = min_buy;
		}
		if (arr[i] < arr[min_buy])
		{
			min_buy = i;
		}
		++i;
	}

	return max_profit;
	

}