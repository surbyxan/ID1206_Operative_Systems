#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int num_threads = 0;
int strip_size;
float *array; 
int bins = 30;

void *thread_func(void *arg); /* the thread function */


int main(int argc, char *argv[])
{
	num_threads = atoi(argv[1]);
/* Initialize an array of random values */
	int array_size = atoi(argv[2]);
    srand((unsigned)time(NULL));
    strip_size = array_size/num_threads;
	array = (float *)malloc(sizeof(float) * array_size);
	for (int i = 0; i < array_size; i++) {
		array [i] = (double)rand() / RAND_MAX;
	}

	double time_serial = 0.0;
    
    clock_t start, end;

	int i;
    int serial_hist[bins];

    for (int i = 0; i < bins; i++) {
        serial_hist[i] = 0;
    }

    start = clock(); /* Master thread adding the array into a histogram*/
    for (int i = 0; i < array_size; i++) {
        int bin = (int)(array[i] * bins);  
 
        if (bin == bins) {
            bin = bins - 1;
        }

    serial_hist[bin]++; 
    }   
//Timer Begin
    end = clock();
    time_serial = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("\n=== SERIAL HISTOGRAM ===\n");
printf("Creation time: %.6f seconds\n\n", time_serial);
for (i = 0; i < bins; i++)
{
    printf("Bin %2d: %d\n", i, serial_hist[i]);
}

	/* Create a pool of num_threads workers and keep them in workers */
	pthread_t *workers;
	workers = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    int thread_ids[num_threads];
	double time_parallel = 0.0;
	//Timer Begin
    clock_t begin, stop;
    
    begin = clock();
    
	for (i = 0; i < num_threads; i++) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
    thread_ids[i] = i;
	pthread_create(&workers[i], &attr, thread_func, &thread_ids[i]);
	}

	int parallel_hist[bins];
	for (int i = 0; i < bins; i++) {
    parallel_hist[i] = 0;
	}

	for (int i=0; i<num_threads; i++) {
		int *thread_result;
			
		pthread_join(workers[i], (void**)&thread_result);
    
		for (int j = 0; j < bins; j++) {
			parallel_hist[j] += thread_result[j];
		}
		
		free(thread_result); 
	}
	
	//Timer End
    stop = clock();
    time_parallel = ((double) (stop - begin)) / CLOCKS_PER_SEC;
	printf("\n=== PARALLEL HISTOGRAM ===\n");
printf("Creation time: %.6f seconds\n\n", time_parallel);
for (i = 0; i < bins; i++) {
    printf("Bin %2d: %d\n", i, parallel_hist[i]);
}
    free(array);
}

void *thread_func(void *arg) {
/* Assign each thread an id so that they are unique in range [0, num_thread -1
] */
	int my_id = *((int *)arg);
	//printf("Hi im thread: %d \n", my_id);

	int first, last;

  	/* determine first and last rows of my strip */
  	first = my_id*strip_size;
  	last = first + strip_size - 1;
/* Perform Partial Parallel histogram Here */
	int *my_hist = calloc(bins, sizeof(int));
    
	for (int i = first; i <= last; i++) {
        int bin = (int)(array[i] * bins);  
 
        if (bin == bins) {
            bin = bins - 1;
        }

  		my_hist[bin]++; 
    } 
    
	pthread_exit(my_hist);
}