#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>


typedef struct a1
{
    int l;
    int r;
    int p;
    int *a; // Change here
} qs_param;


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




void *thread_quicksort(void *arg);
int partition(int[], int, int);
void quicksort(int[], int, int, int); // Declaration

int main(int argc, char *argv[]) 
{
    int parallel_recursion_level = 2;
    int MAX  = (argc > 1) ? atoi(argv[1]) : 20;
    int print = (argc > 2) ? atoi(argv[2]) : 0;
    parallel_recursion_level = (argc > 3) ? atoi(argv[3]) : 2;
    clock_t begin,end;
    double time_spend;
    int *a = malloc(sizeof(int) * MAX);
    
    begin=clock();
    fillupRandomly (a, MAX, 0, 100);
    end=clock();
    time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("Time to fill array with random data: %f Seconds\n", time_spend);
    int i;
    if(print){printf("\nUnsorted array:\t");
    for(i = 0; i < MAX; ++i)
        printf(" %d ", a[i]);}
    printf("\n");
    begin=clock();
    quicksort(a, 0, MAX-1, parallel_recursion_level); // Update function call
    end=clock();
    time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("Time to sort data: %f Seconds\n", time_spend);
    if(print){printf("\n\nSorted array:\t");
    for(i = 0; i < MAX; ++i){
        printf(" %d ", a[i]);}}

    return 0;
}

void *thread_quicksort(void *arg)
{
    qs_param *qp = (qs_param *) arg;
    int *a = qp->a;
    int l = qp->l;
    int r = qp->r;
    int p = qp->p;
    
    int j;

    if( l < r ) 
    {
        j = partition(a, l, r);

        qs_param qp_left;
        qp_left.a = a;
        qp_left.l = l;
        qp_left.r = j - 1;
        qp_left.p = p - 1;

        qs_param qp_right;
        qp_right.a = a;
        qp_right.l = j + 1;
        qp_right.r = r;
        qp_right.p = p - 1;

        if(p > 0)
        {
            pthread_t thread[2]; // Declare array of threads
            pthread_create(&thread[0], NULL, 
                thread_quicksort, (void*)&qp_left);
            pthread_create(&thread[1], NULL, 
                thread_quicksort, (void*)&qp_right);

            pthread_join(thread[0], NULL);
            pthread_join(thread[1], NULL);
        }
        else
        {
            quicksort(a, l, j - 1, 0);
            quicksort(a, j + 1, r, 0);
        }
    }
}

void quicksort(int a[], int l, int r, int p)
{
    if (l < r)
    {
        int j = partition(a, l, r);
        quicksort(a, l, j - 1, p);
        quicksort(a, j + 1, r, p);
    }
}

int partition(int a[], int l, int r)
{
    int pivot = a[l];
    int i = l, j = r + 1;
    int t;

    while(1)
    {
        do ++i; while(a[i] <= pivot && i <= r);
        do --j; while(a[j] > pivot);
        if(i >= j) break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }

    t = a[l];
    a[l] = a[j];
    a[j] = t;

    return j;
}
