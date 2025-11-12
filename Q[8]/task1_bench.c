#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BUFSIZE 100
#define N 1000000
#define TRIALS 1000

int task1(int arr[]) { 
	int i;
	int fd[2]; //! read 0 write 1 assertions
	pipe(fd); //! creating pipe
	
	int pid= fork(); //! child 1
	
	if (pid == 0) {//! child 1
		int sum = 0;
		for (i = 0; i < N/2; i++) {
			sum += arr[i];
		}
		//printf("im child 1, my sum is %d\n", sum);
		close(fd[0]);
		write(fd[1], &sum, sizeof(sum));
		close(fd[1]);
		free(arr);
		exit(0);
	}
	
	int pid2 = fork();
	if (pid2 == 0){ //! child 2
		int sum = 0;
		for (i = N/2; i < N; i++) {
			sum += arr[i];
		}
		//printf("im child 2, my sum is %d \n", sum);
		close(fd[0]);
		write(fd[1], &sum, sizeof(sum));
		close(fd[1]);
		free(arr);
		exit(0);
	}
	
	//! parent
		int sum1;
		int sum2;
		wait();
		wait();
		read(fd[0], &sum1, sizeof(sum1));
		read(fd[0], &sum2, sizeof(sum2));
		//printf("im parent, sum1 is %d, sum2 is %d\n", sum1, sum2);
		
		//printf("Total sum: %d\n", sum1 + sum2);
		
}


int main() {
	int i;
	clock_t start, end;
	double cpu_time_used, total_cpu_time_used;
	int *arr ; //todo malloc this
	arr = (int *)malloc(N * sizeof(int));
	for (i = 0; i < N; i++)
	{
		arr [i] = rand() % 2;
        
	}

	for (i = 0; i < TRIALS; i++) {

    start = clock();
	task1(arr);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	total_cpu_time_used += cpu_time_used;

	}
    printf("Mean Time taken: %.2f microseconds (μs)\n", (total_cpu_time_used/TRIALS)* 1000000.0); //! in microseconds

    return 0;
}