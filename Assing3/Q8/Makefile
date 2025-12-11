CC = gcc
CFLAGS = -pthread

all:
	$(CC) $(CFLAGS) task.c -o task

clean:
	rm -f task

run:
	./task 64 20
	./task 64 200
	./task 64 2000
