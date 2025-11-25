#include <pthread.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
     
int num_threads = 0;
int node_count = 0;  
pthread_mutex_t stack_mutex;
pthread_mutex_t cas_mutex;

typedef struct node { 
     int node_id;      //a unique ID assigned to each node
     struct node *next;
} Node;

Node *top; // top of stack

/*Option 1: Mutex Lock*/
void push_mutex() { 
     Node *old_node;
     Node *new_node;
     new_node = malloc(sizeof(Node));
     
     //update top of the stack below
     pthread_mutex_lock(&stack_mutex);  
     old_node = top;  
     new_node->next = old_node;  
     //assign a unique ID to the new node
     new_node->node_id = node_count++;  
     top = new_node;  
     printf("Pushed node id: %d\n", new_node->node_id);  
     pthread_mutex_unlock(&stack_mutex);  
}

int pop_mutex() { 
     Node *old_node;
     Node *new_node;
     
     //update top of the stack below
     pthread_mutex_lock(&stack_mutex);  

     if (top == NULL) {  
          pthread_mutex_unlock(&stack_mutex);  
          return -1;  
     }

     old_node = top;  
     top = old_node->next;  

     pthread_mutex_unlock(&stack_mutex);  

     int old_node_id = old_node->node_id;  
     free(old_node);  
     printf("Popped node id: %d\n", old_node_id);  
     return old_node_id;  
}

/*Option 2: Compare-and-Swap (CAS)*/
void push_cas() { 
     Node *old_node;
     Node *new_node;
     new_node = malloc(sizeof(Node)); 
     
     
     //update top of the stack below
     while (!__sync_bool_compare_and_swap(&top, old_node, new_node)){
          old_node = top;
          new_node->next = old_node;
     }
     
     //assign a unique ID to the new node
     pthread_mutex_lock(&cas_mutex);
     new_node->node_id = node_count++;
     pthread_mutex_unlock(&cas_mutex);

     printf("Pushed node id: %d\n", new_node->node_id);
}

int pop_cas() { 
     Node *old_node;
     Node *new_node;

     //update top of the stack below

     return old_node->node_id;
}

/* the thread function */
void *thread_func(int opt) { 
     /* Assign each thread an id so that they are unique in range [0, num_thread -1 ] */
     int my_id;

     if( opt==0 ){
          push_mutex();push_mutex();pop_mutex();pop_mutex();push_mutex();
     }else{
          push_cas();push_cas();pop_cas();pop_cas();push_cas();
     }
     
     printf("Thread %d: exit\n", my_id);
     pthread_exit(0);
}

int main(int argc, char *argv[])
{
     num_threads = atoi(argv[1]);
     top = NULL;  
     node_count = 0;  
     pthread_mutex_init(&stack_mutex, NULL); 
     pthread_mutex_init(&cas_mutex, NULL); 


     /* Option 1: Mutex */ 
     pthread_t *workers = malloc(num_threads * sizeof(pthread_t));
     int opt1 = 0;
     for (int i = 0; i < num_threads; i++) { 
          pthread_attr_t attr;
          pthread_attr_init(&attr);
          pthread_create(&workers[i], &attr, thread_func, &opt1);  
     }
     for (int i = 0; i < num_threads; i++) 
          pthread_join(workers[i], NULL);  

     //Print out all remaining nodes in Stack
     printf("Mutex: Remaining nodes \n");
     while (top != NULL) {
          Node *temp = top;
          printf("Node id: %d\n", temp->node_id);
          top = top->next;
          free(temp);
     }

     /*free up resources properly */
     free(workers);
     pthread_mutex_destroy(&stack_mutex);
     /* Option 2: CAS */ 
     /* REMOVE HEREEEEEEEE

     for (int i = 0; i < num_threads; i++) { 
          pthread_attr_t attr;
          pthread_attr_init(&attr);
          pthread_create(...); 
     }
     for (int i = 0; i < num_threads; i++) 
          pthread_join(...);

     //Print out all remaining nodes in Stack
     printf("CAS: Remaining nodes \n");
     
     /*free up resources properly */
     
}