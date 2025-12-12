CC = gcc
CFLAGS = -memset

all:
	$(CC) $(CFLAGS) task.c -o task

clean:
	rm -f task

run:
	/usr/bin/time --verbose ./task 128
