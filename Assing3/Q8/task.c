#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int n;
int m;
int player_finished = 0; // Flag for player used by checker
typedef struct page {
  int page_id;
  int reference_bit;
  struct page *next;
  int total_references; 
} Page;

Page *page_table[1000];     
int reference_string[1000]; 
pthread_mutex_t list_mutex;
// @create an active list
Page *active_list_head = NULL; 
Page *active_list_tail = NULL; 
int active_list_counter = 0;
// @create an inactive list
Page *inactive_list_head = NULL; 
int remove_from_list(Page **head, Page *node);

void *player_thread_func(void *) {
  // @Add code for player

  int i;
  for (i = 0; i < sizeof(reference_string) / sizeof(int); i++) {
    usleep(10);
    pthread_mutex_lock(&list_mutex);
    int page_id = reference_string[i];
    Page *target_page = page_table[page_id];

    // detach the page from either list if present
    int did_remove_yesno = remove_from_list(&active_list_head, target_page);
    if (did_remove_yesno)
      active_list_counter--;

    remove_from_list(&inactive_list_head, target_page);
    target_page->next = NULL;

    target_page->reference_bit = 1;

    // MOVING SECTION
    if (active_list_counter < (0.7 * n)) { /// add to active list

      // add page to active list head
      Page *temp = active_list_head;
      active_list_head = target_page;
      active_list_head->next = temp;
      active_list_counter++;

    } else {
      // add to inactive list
      int move_count = (active_list_counter * 0.2);
      Page *chunk_start = active_list_head;
      Page *chunk_end = active_list_head;

      // limit move_count to active_list_counter
      if (move_count > active_list_counter)
        move_count = active_list_counter;

      // move chunk_end to the end of the chunk check for NULL <--(NEW)
      for (int j = 0; j < move_count && chunk_end->next; j++) {
        chunk_end = chunk_end->next;
      }

  

      Page *new_head = chunk_end->next;
      active_list_head = new_head;

      if (active_list_head == NULL) {
        active_list_tail = NULL;
      }

      chunk_end->next = inactive_list_head;
      Page *temp = inactive_list_head;
      inactive_list_head = chunk_start;
      chunk_start->next = temp;
      active_list_counter -= move_count;
    }

    pthread_mutex_unlock(&list_mutex);
  }
  player_finished = 1;
  pthread_exit(0);
}
void *checker_thread_func(void *arg) {
  while (player_finished == 0) {
    // @Add code for checker
    usleep(m); 
    pthread_mutex_lock(&list_mutex);
    Page *current_page = active_list_head;
    while (current_page != NULL) {
      if (current_page->reference_bit == 1) {
        current_page->total_references++;
        current_page->reference_bit = 0;
      }
      current_page = current_page->next;
    }
    pthread_mutex_unlock(&list_mutex);
  }
  pthread_exit(0);
}

int main(int argc, char *argv[]) {
  n = atoi(argv[1]);
  m = atoi(argv[2]);
  // @create a random reference string of N
  for (int i = 0; i < 1000; i++) 
  {
    reference_string[i] = rand() % n;
  }


  // populate the page table list
  for (int i = 0; i < n; i++) {
    Page *p = malloc(sizeof(Page));
    p->page_id = i;
    p->reference_bit = 0;
    p->total_references = 0;
    p->next = NULL;
    page_table[i] = p;
  }

  /* Create two workers */
  pthread_mutex_init(&list_mutex, NULL);
  pthread_t player;
  pthread_t checker;
  pthread_create(&player, NULL, player_thread_func, NULL);
  pthread_create(&checker, NULL, checker_thread_func, (void *)&m);
  pthread_join(player, NULL);
  pthread_join(checker, NULL);

  void print_list(Page * head) {
    Page *current = head;
    while (current != NULL) {
      printf("%d ", current->page_id);
      current = current->next;
    }
    printf("\n");
  }
  printf("Page_Id, Total_Referenced\n");
  //@Print out the statistics of page references
  for (int i = 0; i < n; i++) {
    Page *p = page_table[i];
    printf("%d, %d\n", p->page_id, p->total_references);
  }
  //@Print out the list of pages in active list
  printf("Pages in active list: \n");
  print_list(active_list_head);

  //@Print out the list of pages in inactive list
  printf("pages in inactive list:\n");
  print_list(inactive_list_head);
  /*free up resources properly */
  free(active_list_head);
  free(inactive_list_head);
}

int remove_from_list(Page **head, Page *node) {
  Page *prev = NULL, *curr = *head;
  while (curr) {
    if (curr == node) {
      if (prev)
        prev->next = curr->next;
      else
        *head = curr->next;
      return 1;
    }
    prev = curr;
    curr = curr->next;
  }
  return 0;
}
