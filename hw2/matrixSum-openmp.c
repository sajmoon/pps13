/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>

double start_time, end_time;

#include <stdio.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */

int numWorkers;
int size; 
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *);

struct position
{
  int x;
  int y;
};

void print_pos(struct position p) {
  printf(" at: %i, %i\n", p.x, p.y);
}

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j, total=0;

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
    //  printf("[ ");
	  for (j = 0; j < size; j++) {
      matrix[i][j] = rand()%99;
//      	  printf(" %d", matrix[i][j]);
	  }
//	  	  printf(" ]\n");
  }

  /* List all active threads */
  printf("All workers:\n");
  #pragma omp parallel
    {
      // Code inside this region runs in parallel.
      printf("One worker!\n");
    } 
  printf("\n");

  start_time = omp_get_wtime();
  
  int current_min = 100;
  struct position min_at = {-1, -1};
  struct position max_at = {-1, -1};
  
  int current_max = -100;
#pragma omp parallel for reduction (+:total) private(j)
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++){
      total += matrix[i][j];
      if (matrix[i][j] < current_min) {
        current_min = matrix[i][j];
        min_at.x = i;
        min_at.y = j;
      }
      if (matrix[i][j] > current_max) {
        current_max = matrix[i][j];
        max_at.x = i;
        max_at.y = j;
      }
         
    }
// implicit barrier

  end_time = omp_get_wtime();

  printf("the total is %d\n", total);
  printf("the minimum is %d", current_min);
  print_pos(min_at);
  printf("the maximum is %d", current_max);
  print_pos(max_at);
  printf("it took %g seconds\n", end_time - start_time);

}
