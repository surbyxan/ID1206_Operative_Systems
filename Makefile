# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Target executable
TARGET = main

# Default build
all: $(TARGET)

# Build rule
$(TARGET): task1.c
	$(CC) $(CFLAGS) -o $(TARGET) task1.c

# Run program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)
