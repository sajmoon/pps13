/* a simple producer/consumer using semaphores and threads

   usage on Solaris:
     gcc thisfile.c -lpthread -lposix4
     a.out numIters

*/
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#define SHARED 0
#define MAXBABIES 100

void *Producer(void *);  /* the two threads */
void *Consumer(void *);

sem_t empty, full;    /* the global semaphores */
int data;             /* shared buffer         */
int numIters;
int numBabies;

/* main() -- read command line and create threads, then
             print result when the threads have quit */

int main(int argc, char *argv[]) {
	/* thread ids and attributes */
	numBabies = (argc > 1)? atoi(argv[1]) : MAXBABIES;
	pthread_t pid; 
	pthread_t workerid[MAXBABIES]; 

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	numIters = 10;
	sem_init(&empty, SHARED, 1);  /* sem empty = 1 */
	sem_init(&full, SHARED, 0);   /* sem full = 0  */

	printf("main started\n");
	pthread_create(&pid, &attr, Producer, NULL);
	int w = 0;
	for (w = 0; w < MAXBABIES; w++) {
		pthread_create(&workerid[w], &attr, Consumer, (void *) w);	
	}
	
	pthread_join(pid, NULL);
	
	printf("main done\n");
}

/* deposit 1, ..., numIters into the data buffer */
void *Producer(void *arg) {
	int produced;
	printf("Mumma bird gave birt! \n");

	for (;;) {
		// infintite!!
		sem_wait(&empty);
		printf("Mumma bird is off on a hunt!\n");
		produced = rand()%5 + 2;
		data += produced;
		printf("Mumma bird came back with %i worms! Yum!\n", produced);
		sem_post(&full);
	}
}

/* fetch numIters items from the buffer and sum them */
void *Consumer(void *arg) {
  int total = 0, consumed;
  printf("A babybird is born\n");
  for (;;) {
    sem_wait(&full);
    	
    	if (data > 0)
	    	data -= 1;
	    printf("Baby %d is hungry, eats 1. we now have %i left\n", pthread_self(), data);

	    if (data > 0)
	    	sem_post(&full);
	    int sleeptime = rand()%10;
	    printf("baby sleeps for %i seconds\n", sleeptime);
	    sleep(sleeptime);
    if (data <= 0) {
		printf("Babybird squuueeeeals! SOOOO HUNGRY!\n");
		sem_post(&empty);	
    }
    
  }
  printf("for %d iterations, the total is %d\n", numIters, total);
}
