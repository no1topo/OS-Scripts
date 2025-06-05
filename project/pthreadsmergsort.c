#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

int opt_a;
int opt_t;
int MAX;
int THREAD_MAX;
int* a;

struct tsk {
    int tsk_no;
    int tsk_low;
    int tsk_high;
};


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

void
merge(int low, int mid, int high)
{

    
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int *left = malloc(n1 * sizeof(int));
    int *right = malloc(n2 * sizeof(int));

    int i;
    int j;

    
    for (i = 0; i < n1; i++)
        left[i] = a[i + low];

    
    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];

    int k = low;

    i = j = 0;

    
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    
    while (i < n1)
        a[k++] = left[i++];

    
    while (j < n2)
        a[k++] = right[j++];

    free(left);
    free(right);
}


void
merge_sort(int low, int high)
{

    
    int mid = low + (high - low) / 2;

    if (low < high) {
        
        merge_sort(low, mid);

        
        merge_sort(mid + 1, high);

        
        merge(low, mid, high);
    }
}




void *
merge_sort123(void *arg)
{
    struct tsk *tsk = arg;
    int low;
    int high;

    
    low = tsk->tsk_low;
    high = tsk->tsk_high;

    
    int mid = low + (high - low) / 2;

    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }

    return 0;
}



int isSorted(int *a, int size){
   for(int i = 0; i < size - 1; i++)
      if(a[i] > a[i + 1])
        return 0;
   return 1;
}

int main(int argc, char *argv[])
{	

	char *cp;
    	struct tsk *tsk;
	MAX  = (argc > 1) ? atoi(argv[1]) : 20;
        int print = (argc > 2) ? atoi(argv[2]) : 0;
        THREAD_MAX = (argc > 3) ? atoi(argv[3]) : 2;
        opt_a = (argc > 4) ? atoi(argv[4]) : 1;
	clock_t begin,end;
	double time_spend;
	a = malloc(sizeof(int) * MAX);
        
        //_______________________________________________

	

        begin=clock();
	fillupRandomly (a, MAX, 0, 100);
	end=clock();
	time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
	printf("Time to fill array with random data: %f Seconds\n", time_spend);
	
	pthread_t threads[THREAD_MAX];
    	struct tsk tsklist[THREAD_MAX];

    	int len = MAX / THREAD_MAX;
	if(print){printf("THREADS:%d MAX:%d LEN:%d\n", THREAD_MAX, MAX, len);}
	int low = 0;
	
	for (int i = 0; i < THREAD_MAX; i++, low += len) {
        tsk = &tsklist[i];
        tsk->tsk_no = i;

        if (opt_a) {
            tsk->tsk_low = low;
            tsk->tsk_high = low + len - 1;
            if (i == (THREAD_MAX - 1))
                tsk->tsk_high = MAX - 1;
        }

        else {
            tsk->tsk_low = i * (MAX / THREAD_MAX);
            tsk->tsk_high = (i + 1) * (MAX / THREAD_MAX) - 1;
        }

        if (print)
            printf("RANGE %d: %d %d\n", i, tsk->tsk_low, tsk->tsk_high);
    }
	
	begin=clock();
    	for (int i = 0; i < THREAD_MAX; i++) {
        	tsk = &tsklist[i];
        	pthread_create(&threads[i], NULL, merge_sort123, tsk);
    	}
    	for (int i = 0; i < THREAD_MAX; i++){
        	pthread_join(threads[i], NULL);}
	end=clock();
	time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
	printf("Time to sort data: %f Seconds\n", time_spend);
	if (print) {
        	for (int i = 0; i < THREAD_MAX; i++) {
            		tsk = &tsklist[i];
            		printf("SUB %d:", tsk->tsk_no);
            		for (int j = tsk->tsk_low; j <= tsk->tsk_high; ++j){
                		printf(" %d", a[j]);}
            		printf("\n");
        		}
    		}
    	if (opt_a) {
        	struct tsk *tskm = &tsklist[0];
        	for (int i = 1; i < THREAD_MAX; i++) {
            		struct tsk *tsk = &tsklist[i];
            		merge(tskm->tsk_low, tsk->tsk_low - 1, tsk->tsk_high);
        		}
    		}
    	else {
        	merge(0, (MAX / 2 - 1) / 2, MAX / 2 - 1);
        	merge(MAX / 2, MAX / 2 + (MAX - 1 - MAX / 2) / 2, MAX - 1);
        	merge(0, (MAX - 1) / 2, MAX - 1);
    		}

	assert(1 == isSorted(a,MAX));
	

        if(print){
           printArray(a, MAX);
        }
        
        free(a);
        return (EXIT_SUCCESS);
	return 0;
}

