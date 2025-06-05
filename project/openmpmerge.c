#include<stdio.h>
#include<omp.h>
#include <stdlib.h> 
#include <time.h>
#include <string.h>
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


void merge(int * X, int n, int * tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;
	
   while (i<n/2 && j<n) {
      if (X[i] < X[j]) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; 
	 j++;
      }
   }
   while (i<n/2) { 
      tmp[ti] = X[i];
	ti++;
	i++;
   }
      while (j<n) { 
         tmp[ti] = X[j];
         ti++; 
	 j++;
   }
	
   memcpy(X, tmp, n*sizeof(int));

} 

void mergesort(int * X, int n, int * tmp)
{
   if (n < 2) return;

   #pragma omp task firstprivate (X, n, tmp)
   mergesort(X, n/2, tmp);

   #pragma omp task firstprivate (X, n, tmp)
   mergesort(X+(n/2), n-(n/2), tmp);
   #pragma omp taskwait
   merge(X, n, tmp);
}


int main(int argc, char *argv[])
{
   
   int n  = (argc > 1) ? atoi(argv[1]) : 10;
   int print  = (argc > 2) ? atoi(argv[2]) : 0;
   int *data = malloc(n*sizeof(int));
   int *tmp = malloc(n*sizeof(int));
   clock_t begin,end;
   double time_spend;
   fillupRandomly (data, n, 0, 100);
   if(print){printf("List Before Sorting...\n");
   printArray(data, n);}
   begin=clock();
   #pragma omp parallel
   {
      #pragma omp single
      mergesort(data, n, tmp);
   }
   end=clock();
   time_spend=(double)(end-begin)/CLOCKS_PER_SEC;
   printf("Time to sort data: %f Seconds\n", time_spend);
   if(print){printf("\nList After Sorting...\n");
   printArray(data, n);}
   free(data);
   free(tmp);   
}
