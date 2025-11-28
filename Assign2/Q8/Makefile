CC = gcc
CFLAGS = -pthread

all:
	$(CC) $(CFLAGS) -o pthread_stack pthread_stack.c

clean:
	rm -f pthread_stack

run:
	./pthread_stack 1
	./pthread_stack 2
	./pthread_stack 4
	./pthread_stack 8
	./pthread_stack 16
	./pthread_stack 32