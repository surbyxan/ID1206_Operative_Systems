#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>


struct request {
	size_t offset;
	size_t size;
};

struct values
{
	 const char *file;
	 char *buffer;
	 struct request *list;
	 int num_requests;
};
void *reader_thread_func(void *arg) {
     // @Add code for reader threads
     // given struct values vals containing file, buffer, start_index, end_index
     struct values *values = (struct values *)arg;
     for(int i=0; i<values->num_requests; i++) {
          fseek(values->file, values->list[i].offset, SEEK_SET);
          fread(values->buffer, 1, values->list[i].size, values->file);
     }
     // @for each: read bytes_i from offset_i
     pthread_exit(0);
}

void *writer_thread_func(void *arg) {
     // @Add code for writer threads
     //given a struct values vals containing file, buffer, start_index, end_index
     struct values *values = (struct values *)arg;
     for(int i=0; i<values->num_requests; i++) {
          fseek(values->file, values->list[i].offset, SEEK_SET);
          fwrite(values->buffer, 1, values->list[i].size, values->file);
     }

     // @for each: write bytes_i to offset_i

     pthread_exit(0);

}



int main(int argc, char *argv[])


{

     int n = atoi(argv[1]); //number of bytes
     int p = atoi(argv[2]); //number of threads

     // @create a file for saving the data
     const char *fname = "datafile.dat";
     

     // @allocate a buffer and initialize it
     char *buffer = malloc(n);
     for (int i = 0; i < n; i++) {
          buffer[i] = 'a';
     }

     struct values args[p];
     pthread_t writers[p];
     pthread_t readers[p];

     struct values thread_args[p]; // One argument struct per thread
     int reqs_per_thread = 100 / p; // Divide the 100 requests among P threads
     struct timeval start_time, end_time;
	 //BLOCK LIST1
     FILE *fp = fopen(fname, "w");

     gettimeofday(&start_time, NULL);  

     struct request list1[100];
	 size_t current_offset = 0;
	 size_t request_size = 16384;
	 for (int i = 0; i < 100; i++) {
		list1[i].offset = current_offset;
		list1[i].size = request_size;
		current_offset += request_size; 
	}

	 
     for (int i = 0; i < p; i++) {
          thread_args[i].file = fp;
          thread_args[i].buffer = buffer;
		  thread_args[i].list = &list1[i * reqs_per_thread];
          thread_args[i].num_requests = reqs_per_thread;
		  
          if (i == p - 1) {
                thread_args[i].num_requests = 100 - (i * reqs_per_thread);
			}
			pthread_create(&writers[i], NULL, writer_thread_func, (void *)&thread_args[i]);
		}
		
		for (int i = 0; i < p; i++) {
			pthread_join(writers[i], NULL);
		}
		
	fclose(fp);
     // @end timing
     gettimeofday(&end_time, NULL);
     double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
	 (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
	 
	 printf("List1: Write %f MB, use %d threads, elapsed time %f s, write bandwidth: %f MB/s \n", (double)n / (1024 * 1024), p, elapsed_time, (double)n / (1024 * 1024) / elapsed_time);
	 
     fp = fopen("datafile.dat", "r");
     gettimeofday(&start_time, NULL);

     for (int i = 0; i < p; i++) {
          thread_args[i].file = fp;
          thread_args[i].buffer = buffer;
          thread_args[i].list = &list1[i * reqs_per_thread];
          thread_args[i].num_requests = reqs_per_thread;
		  
          if (i == p - 1) {
			  thread_args[i].num_requests = 100 - (i * reqs_per_thread);
			}
     
			pthread_create(&readers[i], NULL, reader_thread_func, (void *)&thread_args[i]);
     }
	 
     for (int i = 0; i < p; i++) {
		 pthread_join(readers[i], NULL);
		}

	fclose(fp);
    gettimeofday(&end_time, NULL);
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
	(end_time.tv_usec - start_time.tv_usec) / 1000000.0;  
	 
	 //@Print out the read bandwidth
     printf("List1: Read %f MB, use %d threads, elapsed time %f s, read bandwidth: %f MB/s \n", (double)n / (1024 * 1024), p, elapsed_time, (double)n / (1024 * 1024) / elapsed_time);
   
	 
	 // BLOCK LIST2
    fp = fopen(fname, "w");

     gettimeofday(&start_time, NULL);  

	 struct request list2[100];
	 for (int i = 0; i < 100; i++) {
		  list2[i].size = 128;
		  list2[i].offset = (rand() % (n / 4096)) * 4096;
	 }
	 
     for (int i = 0; i < p; i++) {
          thread_args[i].file = fp;
          thread_args[i].buffer = buffer;
		  thread_args[i].list = &list2[i * reqs_per_thread];
          thread_args[i].num_requests = reqs_per_thread;
		  
          if (i == p - 1) {
                thread_args[i].num_requests = 100 - (i * reqs_per_thread);
			}
			pthread_create(&writers[i], NULL, writer_thread_func, (void *)&thread_args[i]);
		}
		
		for (int i = 0; i < p; i++) {
			pthread_join(writers[i], NULL);
		}
		
	fclose(fp);
     // @end timing
     gettimeofday(&end_time, NULL);
     elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
	 (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
	 
	 printf("List2: Write %f MB, use %d threads, elapsed time %f s, write bandwidth: %f MB/s \n", (double)n / (1024 * 1024), p, elapsed_time, (double)n / (1024 * 1024) / elapsed_time);
	 
     fp = fopen("datafile.dat", "r");
     gettimeofday(&start_time, NULL);

     for (int i = 0; i < p; i++) {
          thread_args[i].file = fp;
          thread_args[i].buffer = buffer;
          thread_args[i].list = &list2[i * reqs_per_thread];
          thread_args[i].num_requests = reqs_per_thread;
		  
          if (i == p - 1) {
			  thread_args[i].num_requests = 100 - (i * reqs_per_thread);
			}
     
			pthread_create(&readers[i], NULL, reader_thread_func, (void *)&thread_args[i]);
     }
	 
     for (int i = 0; i < p; i++) {
		 pthread_join(readers[i], NULL);
		}

	fclose(fp);
    gettimeofday(&end_time, NULL);
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
	(end_time.tv_usec - start_time.tv_usec) / 1000000.0;  
	 
	 //@Print out the read bandwidth
     printf("List2: Read %f MB, use %d threads, elapsed time %f s, read bandwidth: %f MB/s \n", (double)n / (1024 * 1024), p, elapsed_time, (double)n / (1024 * 1024) / elapsed_time);
     /*free up resources properly */
     free(buffer);

     return 0;
	 
	}
	
	