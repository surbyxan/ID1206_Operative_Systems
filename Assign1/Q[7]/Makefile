# Compiler and flags
CC = gcc

# Target executable
TARGET = main

# Default build
all: clean $(TARGET)

# Build rule
$(TARGET): Q7.c
	$(CC) -o $(TARGET) Q7.c

# Run program
run: clean $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)
	