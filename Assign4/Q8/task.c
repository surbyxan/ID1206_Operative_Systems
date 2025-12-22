//this program took inspiration from the following stackoverflow post: https://stackoverflow.com/questions/26259421/use-mmap-in-c-to-write-into-memory

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

#define FILE_NAME "file_to_map.txt"
#define FILE_SIZE (1024 * 1024)   // 1 MB

int main() {
    int fd;
    void *map_addr;
    pid_t pid;

    fd = open(FILE_NAME, O_RDWR);

    pid = fork();

    map_addr = mmap(NULL, FILE_SIZE,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED,
                    fd, 0);

    if (pid == 0) {
        // Child process

        memcpy(map_addr, "01234", 5);

        char buffer[5];
        memcpy(buffer, map_addr+4096, 5);

        printf("Child Process ( PID: %d ); read from mmaped [4096]: %s\n",
               getpid(), buffer);
    } else {
        // Parent process
        memcpy(map_addr+4096,"56789", 5);

        char buffer[5];
        memcpy(buffer, map_addr, 5);

        printf("Parent Process ( PID: %d ); read from mmaped [0]: %s\n",
               getpid(), buffer);
    }

    munmap(map_addr, FILE_SIZE); // Unmap the memory
    close(fd);

    
}
