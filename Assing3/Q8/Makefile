CC = gcc
CFLAGS = -pthread

all:
	$(CC) $(CFLAGS) task.c -o task

clean:
	rm -f task

run:
	./task 64 20
	./task 128 200
	./task 256 2000
