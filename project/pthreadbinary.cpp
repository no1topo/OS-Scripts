#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

// size of array
int n;
// maximum number of threads
int max_threads;
int *a;
int key = 110;
bool found = false;
int part = 0;


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


void* binary_search(void* arg)
{
 
    // Each thread checks 1/4 of the array for the key
    int thread_part = part++;
    int mid;
 
    int low = thread_part * (n / 4);
    int high = (thread_part + 1) * (n / 4);
 
    // search for the key until low < high
    // or key is found in any portion of array
    while (low < high && !found) {
 
        // normal iterative binary search algorithm
        mid = (high - low) / 2 + low;
 
        if (a[mid] == key) {
            found = true;
            break;
        }
 
        else if (a[mid] > key)
            high = mid - 1;
        else
            low = mid + 1;
    }
}

// Driver Code
int main(int argc, char *argv[])
{

	
	n  = (argc > 1) ? atoi(argv[1]) : 20;
        int print = (argc > 2) ? atoi(argv[2]) : 0;
        key = (argc > 3) ? atoi(argv[3]) : 110;
        max_threads= (argc > 4) ? atoi(argv[4]) : 4;
        pthread_t threads[max_threads];
        clock_t begin,end;
	double time_spend;
	a = (int*)malloc(sizeof(int) * n);
	begin=clock();
	fillupRandomly (a, n, 0, 100);
	end=clock();
	time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
	printf("Time to fill array with random data: %f Seconds\n", time_spend);
	if(print){cout << "Given array is: ";printArray(a,n);}
	begin=clock();
	for (int i = 0; i < max_threads; i++)
        pthread_create(&threads[i], NULL, binary_search,
                       (void*)NULL);
 
    	for (int i = 0; i < max_threads; i++)
        pthread_join(threads[i], NULL);
	end=clock();
	time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
	printf("Time to search value: %f Seconds\n", time_spend);
	
	if (found)
        cout << key << " found in array" << endl;
 
    	// key not found in array
    	else
        cout << key << " not found in array" << endl;
	free(a);

	return 0;
}

