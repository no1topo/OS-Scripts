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

void swap(int* p1, int* p2)
{
    int temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}


void quickSort(int arr[], int low, int high)
{
    if (low < high) {

        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
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
    quickSort(arr, 0, N - 1);
    end=clock();
    time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("Time to sort data: %f Seconds\n", time_spend);
    free(arr);
    return 0;
}


