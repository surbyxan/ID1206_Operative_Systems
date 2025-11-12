# Compiler and flags
CC = gcc
N = 100  # default value
CFLAGS = -DN=$(N)

# Target executable
TARGET = main

# Default build
all: clean $(TARGET)

# Build rule
$(TARGET): task1.c
	$(CC) $(CFLAGS) -o $(TARGET) task1.c

# Run program
run: clean $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)
