#include <stdio.h>
#include <stdlib.h> 
#include <time.h>


unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    do
    {
        r = rand();
    } 
    while (r >= limit);

    return min + (r / buckets);
}

void fillupRandomly (int *m, int size, unsigned int min, unsigned int max){
    for (int i = 0; i < size; i++)
    m[i] = rand_interval(min, max);
}

void printArray(int *a, int size){
   for(int i = 0; i < size; i++)
       printf("%d ", a[i]);
   printf("\n");
}

// An iterative binary search function.
int binarySearch(int arr[], int l, int r, int x)
{
    while (l <= r) {
        int m = l + (r - l) / 2;

        // Check if x is present at mid
        if (arr[m] == x)
            return m;

        // If x greater, ignore left half
        if (arr[m] < x)
            l = m + 1;

        // If x is smaller, ignore right half
        else
            r = m - 1;
    }

    // If we reach here, then element was not present
    return -1;
}

// Driver code
int main(int argc, char *argv[])
{
    	int N  = (argc > 1) ? atoi(argv[1]) : 10;
    	int x  = (argc > 2) ? atoi(argv[2]) : 152;
	int *arr = malloc(N*sizeof(int));
	int arr_size = N; 
	clock_t begin,end;
	double time_spend;
	fillupRandomly (arr, N, 0, 100);
	begin=clock();
	int result = binarySearch(arr, 0, N - 1, x);
	end=clock();
	time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
	printf("Time to search for value: %f Seconds\n", time_spend);
	free(arr);
    	
    	(result == -1) ? printf("Element is not present in array\n"): printf("Element is present at index %d\n",result);
    return 0;
}

