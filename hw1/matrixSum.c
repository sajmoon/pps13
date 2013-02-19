/* matrix summation using pthreads

   features: uses a barrier; the Worker[0] computes
             the total sum from partial sums computed by Workers
             and prints the total sum to the standard output

   usage under Linux:
     gcc matrixSum.c -lpthread
     a.out size numWorkers

*/
#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000       /* maximum matrix size */
#define MAXWORKERS 10       /* maximum number of workers */

pthread_mutex_t updatelock; /* lock for updating variables */
pthread_mutex_t baglock;    /* changed from the barrier to a bag of tasks */
pthread_cond_t  finished;   /* condition variable for leaving */

int numWorkers;             /* number of workers */ 
int numArrived = 0;         /* number who have arrived */

int final_sum;
int final_max, final_maxi, final_maxj;
int final_min, final_mini, final_minj;
int final_num_workers;
int current_row;
double start_time, end_time;  /* start and end times */
int size;                     /* assume size is multiple of numWorkers */

int matrix[MAXSIZE][MAXSIZE]; /* matrix */

void *Worker(void *);
void set_values(int, int, int, int, int, int);
void set_max(int, int, int);
void set_min(int, int, int);

// Used to sum up partial results. 
void update_values(int sum, int max, int maxi, int maxj, int min, int mini, int minj) {
  pthread_mutex_lock(&updatelock);
  int id = rand()%99;
  final_sum  += sum;
  final_num_workers++;
  
  set_values(max, maxi, maxj, min, mini, minj);

  if (final_num_workers == size)
    pthread_cond_broadcast(&finished);

  pthread_mutex_unlock(&updatelock);
}

void set_values(int max, int maxi, int maxj, int min, int mini, int minj) {
  if (max > final_max)
    set_max(max, maxi, maxj);
  if (min < final_min)
    set_min(min, mini, minj);
}

void set_max(int max, int maxi, int maxj) {
  final_max  = max;
  final_maxi = maxi;
  final_maxj = maxj;
}

void set_min(int min, int mini, int minj) {
  final_min  = min;
  final_mini = mini;
  final_minj = minj;
}

/* timer */
double read_timer() {
  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if( !initialized )
  {
    gettimeofday( &start, NULL );
    initialized = true;
  }
  gettimeofday( &end, NULL );
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

void print_result() {
  printf("The total is %d\n", final_sum);
  printf("The max is %d at %d,%d\n", final_max, final_maxi, final_maxj);
  printf("The min is %d at %d,%d\n", final_min, final_mini, final_minj);
  printf("The execution time is %g sec\n", end_time - start_time);
}


/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&updatelock, NULL);
  pthread_mutex_init(&baglock, NULL);
  pthread_cond_init(&finished, NULL);

  final_num_workers = 0;
  final_min = 1000;

  current_row = 0;

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
  
  /* initialize random seed */
  srand ( time(NULL) );

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
      matrix[i][j] = rand()%99;
	  }
  }

  /* print the matrix */
#ifdef DEBUG
  for (i = 0; i < size; i++) {
	  printf("[ ");
	  for (j = 0; j < size; j++) {
	    printf(" %d", matrix[i][j]);
	  }
	  printf(" ]\n");
  }
#endif

  /* do the parallel work: create the workers */
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *) l);

  pthread_cond_wait(&finished, &updatelock);
  
  end_time = read_timer();
  
  print_result();

  pthread_exit(NULL);
}

int get_new_row() {
  pthread_mutex_lock(&baglock);
  
  int c = current_row;
  current_row++;
  
  pthread_mutex_unlock(&baglock);
  return c;
}

void *Worker(void *arg) {
  int row = 0;
  while (row < size) {
    
    row = get_new_row();
  
    if (row >= size)
      break;
    int total, i, j, first, last, max, maxi, maxj, min, mini, minj;

  #ifdef DEBUG
    printf("worker %d (pthread id %d) calcultaion row %d of size: %d\n", pthread_self(), pthread_self(), row, size);
  #endif

    /* determine first and last rows of my strip */
    i = row;

    /* sum values in my strip */
    total = 0;
    max   = -1000;
    maxi  = 0;
    maxj  = 0;
    min   = 1000;
    mini  = 0;
    minj  = 0;

    for (j = 0; j < size; j++) {
      if (matrix[i][j] > max) { 
        max  = matrix[i][j];
        maxi = i;
        maxj = j;
      }
      if (matrix[i][j] < min) {
        min  = matrix[i][j];
        mini = i;
        minj = j;
      }
      total += matrix[i][j];
    }
    update_values(total, max, maxi, maxj, min, mini, minj);
  } 
} 
