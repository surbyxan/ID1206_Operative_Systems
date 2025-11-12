#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 100
#define N 100

int main() {
    int arr [N]; //! malloc this
	int i;
	int fd[2]; //! read 0 write 1 assertions
	pipe(fd); //! creating pipe
	for (i = 0; i < N; i++)
	{
		arr [i] = rand() % 2;
        
	}


	int pid = fork(); //! child 1
	int pid2 = fork(); //! child 2

	if (pid == 0) {//! child 1
		int sum = 0;
		for (i = 0; i < N/2; i++) {
            sum += arr[i];
        }
		printf("im child 1, my sum is %d\n", sum);
		close(fd[0]);
		write(fd[1], &sum, sizeof(sum));
		close(fd[1]);
	}
	else if (pid2 == 0){ //! child 2
        int sum = 0;
        for (i = N/2; i < N; i++) {
            sum += arr[i];
        }
		printf("im child 2, my sum is %d \n", sum);
		close(fd[0]);
		write(fd[1], &sum, sizeof(sum));
		close(fd[1]);
	}
    else { //! parent
        int sum1;
        int sum2;
        wait();
        wait();
        read(fd[0], sum1, sizeof(sum1));
        read(fd[0], sum2, sizeof(sum2));
		printf("im parent, sum1 is %d, sum2 is %d\n", sum1, sum2);
        
        printf("Total sum: %d\n", sum1 + sum2);
        
    }
    
}