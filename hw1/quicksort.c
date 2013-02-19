
#ifndef _REENTRANT
#define _REENTRANT /* Used for multithreaded programms */
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 9
#define MAXVALUE 99
#define MAXWORKERS 10

#define DEBUG

/* Public varialbes */
int list[MAXSIZE];
int list_size = MAXSIZE;

pthread_mutex_t baglock; 
pthread_cond_t  printresult;

void *Worker(void *);

struct position {
  int start;
  int end;
  struct position* next;
};

struct bag_of_tasks {
  struct position *head;
  struct position *tail;
};

int bagsize;

struct bag_of_tasks* make_bag_of_tasks(void) {
  printf("initialize the bag\n");
  struct bag_of_tasks *p = malloc( sizeof(p));
  p->head = p->tail = NULL;
  bagsize = 0;
  return p;
}

struct bag_of_tasks* tasks = NULL;

void init_data() {
  /* initialize random seed */
  srand ( time(NULL) );
  
  /* Fill list with random values */
  int i;
  for (i = 0; i < list_size; i++) {
    int value = rand()%MAXVALUE;
    list[i] = value;
  }

  tasks = make_bag_of_tasks();
}

void print_list() {
  int i;
  printf("['");
  for (i = 0; i < list_size; i++) {
    printf("%i', '", list[i]);
  }
  printf("%i']\n", list[i++]);
}

void add_task(const int s, const int e) {
  struct position* p = malloc (1 * sizeof(*p) );
  if( NULL == p ) {
    printf("malloc failed in add_task\n");
  }

  pthread_mutex_lock(&baglock);
    printf("add task from %i to %i\n", s, e);
    p->start  = s;
    p->end    = e;
    p->next   = NULL;

    if (tasks == NULL) {
      printf("List nit initialized\n");
      free(p);
    }

    if (tasks->head == NULL && tasks->tail == NULL) {
      /* Tasklist is empty */
      printf("add first\n");
      tasks->head = p;
      tasks->tail = p;
      bagsize++;
    } else if( tasks->head == NULL || tasks->tail == NULL) {
      printf("this is wrong!\n");
    } else {
      /* not empty. Add to tail. */
      printf("add to end\n");
      tasks->tail->next = p;
      tasks->tail       = p;
      bagsize++;
    }
  
  pthread_mutex_unlock(&baglock);
}

struct position *pop() {
  struct position *p = tasks->head;

  return p;
}

struct position* get_head() {
  struct position* p  = NULL;
  struct position* pn = NULL;

  pthread_mutex_lock(&baglock);
    
    if (tasks->head == NULL) {
      /* The queue is empty*/
      printf("Head is null\n");
    } else {
      p = tasks->head;
      printf("Head is something: %i -> %i\n", p->start, p->end);
      
      if (p->next != NULL) {
        printf("next == null");
        // if we have a next, then make it the head.
        tasks->head = p->next;
      } else {
        printf("next == null");
        // if not set it to null;
        //(*tasks)->head = NULL;
      }
      
      bagsize--;
    }
  pthread_mutex_unlock(&baglock);
  printf("head: %i\n", tasks->head->end);
  return p;
}


void print_tasks() {
  struct position *p = pop();

  while (p != NULL) {

  }
}

void main(int argc, char *argv) {
  printf("main\n");

  make_bag_of_tasks();

  print_tasks();

}

void main2(int argc, char *argv[]) {
  printf("Start Quicksort\n");

  init_data();
  
  pthread_mutex_init(&baglock, NULL);
  pthread_cond_init(&printresult, NULL);

  pthread_attr_t  attr;
  pthread_t       workerid[MAXWORKERS];
  
  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  
  print_list();

  int i; 
  for (i = 0; i < 1; i++)
    pthread_create(&workerid[i], &attr, Worker, (void *) &i);

  add_task(0, list_size);
  printf("Waiting for workers to finish..\n");
  
  // Wait for a bradcast that tells ut we are finished.
  pthread_cond_wait(&printresult, &baglock);

  printf("End Quicksort\n");
}

void *Worker(void *arg) {
  int myid = 0;//(int) arg;
  int start = 0;
  int end   = 0;
  int finished = 0;

  while(finished == 0) {
    printf("worker: %d\n", myid);

    struct position* p = get_head();
  
    if (p != NULL) {
      end       = p->end;
      start     = p->start;
      int size  = end - start;
      printf("start: %i end: %i size: %i\n", start, end, size);

      if (size < 2) {
        /*
        Basfallet! 
        Vi ska inte ändra något
        */
        printf("this list is done! Only on elemet left thus it is sorted\n");
      } else {
        int pivo_idx = size / 2;
        int pivo = list[pivo_idx];
        printf("pivo: %i at %i \n", pivo, pivo_idx);
        int i = 0;
        int j = 0;
        int less[size];
        int greater[size];
        int less_i    = 0;
        int greater_i = 0;

        for (i = start; i < end; i++) {
          if (i == pivo_idx) {
            // pivo element, skip
          } else if (list[i] < pivo) {
            less[less_i] = list[i];
            less_i++;
          } else {
            greater[greater_i] = list[i];
            greater_i++;
          }
        }

        for (i = 0; i < less_i; i++) {
          list[start + i] = less[start + i];
        }
        list[i] = pivo;
        pivo_idx = i;
        i++;
        for (j = 0; j < greater_i; j++) {
          list[start + j+i] = greater[j];
        }
        printf("the list\n");
        print_list();
        add_task(start, pivo_idx-1);
        add_task(pivo_idx, end);
        printf("bagsize: %i\n", bagsize);
      }
      printf("end\n");
    } else {
      printf("p is null\n");
    }
    if (bagsize == 5) {
      pthread_cond_broadcast(&printresult);
    }
  }
}
