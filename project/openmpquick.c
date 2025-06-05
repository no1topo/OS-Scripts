#include<stdio.h>
#include<omp.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define ARRAY_MAX_SIZE 500000

int arr[ARRAY_MAX_SIZE];


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


void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int partition (int arr[], int low, int high)
{
	int pivot = arr[high];
	int i = (low - 1); 
	for (int j = low; j <= high- 1; j++)
	{
		
		if (arr[j] <= pivot)
		{
			i++; 
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}


void quickSort(int arr[], int low, int high)
{
	if (low < high)
	{
		
		int pi = partition(arr, low, high);


		#pragma omp task firstprivate(arr,low,pi)
		{
			quickSort(arr,low, pi - 1);

		}

		//#pragma omp task firstprivate(arr, high,pi)
		{
			quickSort(arr, pi + 1, high);

		}


	}
}

void printArray(int arr[], int size)
{
	int i;
	for (i=0; i < size; i++)
		printf("%d ", arr[i]);
	printf("\n");
}


int main(int argc, char *argv[])
{
	int n  = (argc > 1) ? atoi(argv[1]) : 10;
	int print  = (argc > 2) ? atoi(argv[2]) : 0;
	int thread  = (argc > 3) ? atoi(argv[3]) : 4;
	int *arr = malloc(n*sizeof(int));
	clock_t begin,end;
	double time_spend;
	fillupRandomly (arr, n, 0, 100);
	if(print){printArray(arr, n);}
omp_set_num_threads(thread);
begin=clock();
#pragma omp parallel
 {

 int id = omp_get_thread_num();
    int nthrds = omp_get_num_threads();
//printf("Thread is %d\n",id);
#pragma omp single nowait


	 quickSort(arr, 0, n-1);


}
	end=clock();
	time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
	printf("Time to sort data: %f Seconds\n", time_spend);
	if(print){printf("Sorted array: \n");
	printArray(arr, n);}
  	free(arr);
	return 0;
}
