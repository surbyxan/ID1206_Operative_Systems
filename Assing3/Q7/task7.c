#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

int main(int argc, char** argv){

  time_t start,end;
    
  int num_pages = atoi(argv[1]);
  int page_size = getpagesize();
  
  printf("Allocating %d pages of %d bytes \n", num_pages, page_size);

  char *addr;
  
  // @Add the start of Timer here
  time(&start);

  // Option 1: @Add mmap below to allocate num_pages anonymous pages 
 // addr = (char*) mmap( NULL, page_size*num_pages ,PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  // Option 2: @Add mmap below to allocate num_pages anonymous pages but using Ausing "huge" pages
  //addr2 = (char*) mmap(NULL, page_size*num_pages, MAP_ANONYMOUS | MAP_HUGETLB);
  addr = (char*) mmap( NULL, page_size*num_pages, PROT_READ | PROT_WRITE, MAP_ANONYMOUS |MAP_HUGETLB, -1, 0);
  //addr = (char*) mmap( ... );
  

  if (addr == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  //the code below updates the pages
  char c = 'a';
  for(int i=0; i<num_pages; i++){
    addr[i*page_size] = c;
    c ++;
  }

  // @Add the end of Timer here
  time(&end);
  // @Add printout of elapsed time in cycles
  printf("Time taken: %.2f seconds\n", difftime(end, start));
  
  for(int i=0; (i<num_pages && i<16); i++){
    printf("%c ", addr[i*page_size]);
  }
  printf("\n");
  
  munmap(addr, page_size*num_pages);
  
}
