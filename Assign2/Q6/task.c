#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int num_threads = 0;
int strip_size;
float *array; 

void *thread_func(void *arg); /* the thread function */


int main(int argc, char *argv[])
{
	num_threads = atoi(argv[1]);
/* Initialize an array of random values */
	int array_size = 1000000;
    strip_size = array_size/num_threads;
	array = (float *)malloc(sizeof(float) * array_size);
	for (int i = 0; i < array_size; i++) {
		array [i] = rand() % 2;
	}
/* Perform Serial Sum */
	float sum_serial = 0.0;
	double time_serial = 0.0;
    
    clock_t start, end;

	int i;

    start = clock(); /* Master thread adding the array*/
	for (i = 0; i < array_size; i++)
	{
		sum_serial += array[i];
	}
	
	    
//Timer Begin
    end = clock();
    time_serial = ((double) (end - start)) / CLOCKS_PER_SEC;
//Timer End
	printf("Serial Sum = %.3f, time = %.3f \n", sum_serial, time_serial);
	/* Create a pool of num_threads workers and keep them in workers */
	pthread_t *workers;
	workers = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    int thread_ids[num_threads];
	double time_parallel = 0.0;
	double sum_parallel = 0.0;
	//Timer Begin
    clock_t begin, stop;
    
    begin = clock();
    
	for (i = 0; i < num_threads; i++) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
    thread_ids[i] = i;
	pthread_create(&workers[i], &attr, thread_func, &thread_ids[i]);
	}

	for (i = 0; i < num_threads; i++){
		float *thread_result;
		
		pthread_join(workers[i], (void**)&thread_result);  // get return value
		sum_parallel += *thread_result;
	}
	
	//Timer End
    stop = clock();
    time_parallel = ((double) (stop - begin)) / CLOCKS_PER_SEC;
	printf("Parallel Sum = %.3f, time = %.3f \n", sum_parallel, time_parallel);
	/*free up resources properly */
    free(array);
}

void *thread_func(void *arg) {
/* Assign each thread an id so that they are unique in range [0, num_thread -1
] */
	int my_id = *((int *)arg);
	printf("Hi im thread: %d \n", my_id);

	int first, last;

  	/* determine first and last rows of my strip */
  	first = my_id*strip_size;
  	last = first + strip_size - 1;
/* Perform Partial Parallel Sum Here */
	float *my_sum = malloc(sizeof(float));
    *my_sum = 0.0;
	for (int i = first; i <= last; i++) {
		*my_sum += array[i];
	}
	printf("Thread %d sum = %f\n", my_id, *my_sum);
    
	pthread_exit(my_sum);
}