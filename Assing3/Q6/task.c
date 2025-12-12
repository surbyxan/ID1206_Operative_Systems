#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){
	size_t n = atoi(argv[1]);
	long size = getpagesize();
	size_t b = n*size;
	 
	void *allocmem = malloc(b);

	memset(allocmem, 0, b); 
	printf("Successfully allocated %zu bytes (Address: %p)\n", b, allocmem);
	free(allocmem);
	return 0;

}