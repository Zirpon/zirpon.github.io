//#include "apue.h"
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

#define NTHR 8	/* number of threads */
#define NUMNUM 8000000L	/* number of numbers to sort */
#define TNUM (NUMNUM/NTHR)	/* number to sort per thread. It is vital, for this macro set the count each thread have to handle */
 
long nums[NUMNUM];	//store the original numbers
long snums[NUMNUM];	//store the sorted numbers
 
pthread_barrier_t b;
 
#ifdef SOLARIS
#define heapsort qsort
#else
extern int heapsort(void *, size_t, size_t, int (*)(const void *, const void *));
#endif
 
/*Compare two long intergers*/
int complong(const void *, const void *);
 
/*Worker thread to sort a portion of the set of numbers*/
void *thr_fn(void *);
 
/*Merge the results of the individual sorted range.*/
void merge();
 
int main()	//the whole overview is quite clear
{
	/*initialize variables*/
	unsigned long i;
	struct timeval start, end;
	long long startusec, endusec;
	double elapsed;
	int err;
	pthread_t tid;
 
	/*create the initial set of numbers to sort*/
	srandom(1);
	for(i=0; i<NUMNUM; i++)
		nums[i] = random();
 
	/*create 8 threads to sort the numbers*/
	gettimeofday(&start, NULL);
	pthread_barrier_init(&b, NULL, NTHR+1);
	for(i=0; i<NTHR; i++)
	{
		err = pthread_create(&tid, NULL, thr_fn, (void *)(i*TNUM));
		if(err != 0)
			err_exit(err, "can't create thread");
	}
	pthread_barrier_wait(&b);
	merge();
	gettimeofday(&end, NULL);
 
	/*print the sorted list*/
	startusec = start.tv_sec * 1000000 ;//+ start.tvusec;
	endusec = end.tv_sec * 1000000 +end.tv_usec;
	elapsed = (double)(endusec - startusec) / 1000000.0;
	printf("sort took %.4f seconds\n", elapsed);
	for(i=0; i<NUMNUM; i++)
		printf("%ld\n", snums[i]);
 
	exit(0);
} 
 
int complong(const void *arg1, const void *arg2)
{
	long l1 = *(long *)arg1;
	long l2 = *(long *)arg2;
 
	if(l1 == l2)
		return 0;
	else if(l1<l2)
		return -1;
	else
		return 1;
}
 
void *thr_fn(void *arg)
{
	long idx = (long)arg;
 
	heapsort(&nums[idx], TNUM, sizeof(long), complong);	// use heapsort to sort the 1000000 numbers
	pthread_barrier_wait(&b);
 
	/*Go off and perform more work... */
	return((void *)0);
}
 
void merge()
{
	long idx[NTHR];
	long i, minidx, sidx, num;
 
	/*initialize the 8 array headers*/
	for(i=0; i<NTHR; i++)
		idx[i] = i*TNUM;
	/*merge 8 sorted arrays*/
	for(sidx=0; sidx<NUMNUM; sidx++)
	{
		num = LONG_MAX;	// the max number for a long int number;
		for(i=0; i<NTHR; i++)
		{
			if((idx[i] < (i+1)*TNUM) && (nums[idx[i]] < num))
			{
				num = nums[idx[i]];
				minidx = i;
			}
		}
		snums[sidx] = nums[idx[minidx]];
		idx[minidx]++;
	}
}