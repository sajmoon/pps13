#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 10   /* maximum number of workers */

pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers = 0;           /* number of workers */ 
int numArrived;       /* number who have arrived */
int starts[MAXSIZE], stops[MAXSIZE];  /* These arrays keep track of the lower and upper bounds of each subarray in the quicksort */
int currentSubArray = 0; /* Which subarray should be sorted next? */
int insertedSubArray= 0; /* How many subarrays have we found so far? */
int workedOn; /* How many elements have we found? */

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

double start_time, end_time; /* start and end times */
int size;  /* assume size is multiple of numWorkers */
int qarray[MAXSIZE];

void *Worker(void *);

pthread_attr_t attr;

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {

  int i,j;

  unsigned int iseed = (unsigned int)time(NULL);
  srand (iseed);

  long l; /* use long in case of a 64-bit system */

  pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&barrier, NULL);
  pthread_cond_init(&go, NULL);

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;


  /* Randomize the array with a seed set to the time. */
  for (j = 0; j < size; j++) {
    qarray[j] = rand()%99;
  }

  /* print the array */
  printf("Random:\n");
  for (i = 0; i < size; i++)
  {
    printf("%d ", qarray[i]);
  }
  printf("\n");

    /*
      Ugly initialization. Should remake this.
    */
      pthread_mutex_lock(&barrier);
      currentSubArray = 0;
      insertedSubArray = 1;
      workedOn = 0;
      starts[currentSubArray] = 0;
      stops[currentSubArray] = size-1;
      pthread_mutex_unlock(&barrier);

    start_time = read_timer();
      for(l = 0; l < numWorkers; l++){
        pthread_create(&workerid[l], &attr, Worker, (void *) l);
      }


      pthread_cond_wait(&go, &barrier);
    
      printf("The execution time is %g sec\n", end_time - start_time);end_time = read_timer();

    /* print the array */
    #ifdef DEBUG
      printf("We worked on %d this many numbers\n", workedOn);
    #endif

      printf("Sorted:\n");
      for (i = 0; i < size; i++)
      {
        printf("%d ", qarray[i]);
      }
      printf("\n");


      pthread_exit(NULL);
    }


    /* Each worker arranges sub-arrays in order. */
    void *Worker(void *arg) {

      // Just a worker ID
      long myid = (long) arg;

      // The index of the last and first position in this sub-array
      int last;
      int first; 

      //BOOL if the worker should keep working
      bool finished = false;

      //Skips the partition if we have to wait
      bool skip = false;

      #ifdef DEBUG
      printf("worker %d has started working.\n", myid);
      #endif

      // Keep working until we say stop (when the bag of tasks is empty)
      while(!finished){

        //
        skip = false;

        // Lock and check if there is a new sub-array we can sort.
        pthread_mutex_lock(&barrier);
        #ifdef DEBUG
        printf("worker %d has started working with current %d and max %d.\n", myid, currentSubArray, insertedSubArray);
        #endif

        // Check if the current value is below the inserted values.
        if(currentSubArray < insertedSubArray){

          // take out the values
          last = stops[currentSubArray];
          first = starts[currentSubArray];

          //Move the pointer to the next subarray
          currentSubArray++;

          // And we shouldn't skip the quicksort-part
          skip = false;
        }else{
          //If there aren't any sub-array to sort, skip the sorting part.
          skip = true;
        }
        // Unlock so other threads can get a sub-array
        pthread_mutex_unlock(&barrier);

        // Check if the sub-array is larger than a single element
        if( last - first > 0){

          // Skip or not to skip, that is the question.
          if(!skip){


            int low = first;
            int mid = first;
            int high = last;
            int pivot = qarray[last];
            int a;

            #ifdef DEBUG
                printf("The pivot is %d with the value of %d \n", last, pivot);
            #endif


            /*
              This is just a regular quicksort. Which puts values below the pivot-element to the left and 
              values above to the right. Elements which are equal is in the middle.
             */
            while (mid <= high) {
              a = qarray[mid];
              if (a < pivot) {
                qarray[mid] = qarray[low];
                qarray[low] = a;
                low++;
                mid++;
              } else if (a == pivot) {
                mid++;
              } else { 
                qarray[mid] = qarray[high];
                qarray[high] = a;
                high--;
              }
            }

        #ifdef DEBUG
            printf("The pivot is %d with the value of %d \n", low, qarray[low]);
            printf("first is %d with the value of %d \n", first, qarray[first]);
            printf("last is %d with the value of %d \n", last, qarray[last]);
            printf("Low is %d with the value of %d \n", low, qarray[low]);
            printf("Mid is %d with the value of %d \n", mid, qarray[mid]);
            printf("High is %d with the value of %d \n", high, qarray[high]);
        #endif


            // Queue the new sub-arrays so another thread can sort them later.
            pthread_mutex_lock(&barrier);

            //The lowerbound of the subarray
            starts[insertedSubArray] = first;

            //The upperbound of the subarray
            stops[insertedSubArray] = low-1;

            //Increase the numbers of subarrays
            insertedSubArray++;

            //Same as above (for the subarray above the pivot value)
            starts[insertedSubArray] = high+1;
            stops[insertedSubArray] = last;
            insertedSubArray++;
            pthread_mutex_unlock(&barrier);

          }

        }else{
            /**
            * We've reached a single element. Mark it as found for later reference
            */
            pthread_mutex_lock(&barrier);
              workedOn++;
            pthread_mutex_unlock(&barrier);
          }

        #ifdef DEBUG
          printf("At the end of a worker with the workedOn as: %d and the size: %d \n", workedOn, size);
        #endif

          //Should the thread stop working?
          if(workedOn >= size){
            finished = true;
          #ifdef DEBUG
            printf("%s\n", "Reached the end!");
          #endif
            pthread_cond_broadcast(&go);

          }
        }
      }