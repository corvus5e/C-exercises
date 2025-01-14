#include <stdio.h>
#define LEN 1000000
int binary_search(int x, int arr[], int size);
int binary_search_improved(int x, int arr[], int size);

void foo()
{
	int arr[LEN];
	for(int i =0; i < LEN; ++i)
		arr[i] = i;

	int size = sizeof(arr) /  sizeof(int);
	int x = -2;
	int pos = -1;

	for(int j = 0; j < 10000; j++)
	for(int i = 0; i < 10000; i++)
	{
		pos = binary_search(x, arr, size);
	}

	printf("pos of %d is %d", x, pos);
}

int main()
{
	foo();

	return 0;
}

int binary_search_improved(int x, int arr[], int size)
{
	int low = 0;
	int hight = size - 1;
	int mid = (low + hight) / 2;

	while(low <= hight && x != arr[mid])
	{
		//printf("low = %d, hight = %d. mid = %d\n", low, hight, mid);
		//getchar();

		if(x < arr[mid])
		{
			hight = mid - 1;
		}
		else
		{
			low = mid + 1;
		}

		mid = (low + hight) / 2;
	}

	return arr[mid] == x ? mid : -1;
}


int binary_search(int x, int arr[], int size)
{
	int low = 0;
	int hight = size - 1;
	int mid = 0; //(low + hight) / 2;

	while(low <= hight)
	{
		//printf("low = %d, hight = %d. mid = %d\n", low, hight, mid);

		mid = (low + hight) / 2;
		if(x < arr[mid])
		{
			hight = mid - 1;
		}
		else if(x > arr[mid])
		{
			low = mid + 1;
		}
		else
		{

			return mid;
		}
	}
	return -1;
}


