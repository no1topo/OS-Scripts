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


void merge(int arr[], int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 = r - m; 
	int L[n1], R[n2]; 
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1 + j]; 

	i = 0; 
	j = 0; 
	k = l; 
	while (i < n1 && j < n2) { 
		if (L[i] <= R[j]) { 
			arr[k] = L[i]; 
			i++; 
		} 
		else { 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 

	while (i < n1) { 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 

	while (j < n2) { 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
} 

void mergeSort(int arr[], int l, int r) 
{ 
	if (l < r) { 
		int m = l + (r - l) / 2; 
		mergeSort(arr, l, m); 
		mergeSort(arr, m + 1, r); 
		merge(arr, l, m, r); 
	} 
} 


int main(int argc, char *argv[]) 
{ 
	int N  = (argc > 1) ? atoi(argv[1]) : 10;
	int *arr = malloc(N*sizeof(int));
	int arr_size = N; 
	clock_t begin,end;
	double time_spend;
	fillupRandomly (arr, N, 0, 100);
	begin=clock();
	mergeSort(arr, 0, arr_size - 1); 
	end=clock();
	time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
	printf("Time to sort data: %f Seconds\n", time_spend);
	free(arr);
	return 0; 
}

