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
#define MAXBEES 20
#define MAXIMUMHONEY 100

void *Honeybees(void *);  /* the two threads */
void *Bear(void *);

sem_t notFull, full;    	/* the global semaphores */
int honeypoy;             	/* shared buffer         */
int numBees;
int maxHoney;

/* main() -- read command line and create threads, then
             print result when the threads have quit */

int main(int argc, char *argv[]) {
	/* thread ids and attributes */
	numBees = (argc > 1)? atoi(argv[1]) : MAXBEES;
	maxHoney = (argc > 2)? atoi(argv[2]) : MAXIMUMHONEY;
	printf("Starting simulation\n");
	printf("Lets spawn %i bees\n\n", numBees);

	

	pthread_t pid; 
	pthread_t workerid[MAXBEES]; 

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	sem_init(&notFull, SHARED, 1);  /* sem notFull = 1 */
	sem_init(&full, SHARED, 0);   	/* sem full = 0  */

	pthread_create(&pid, &attr, Bear, NULL);
	

	int w = 0;
	for (w = 0; w < numBees; w++) {
		pthread_create(&workerid[w], &attr, Honeybees, (void *) w);	
	}
	pthread_join(pid, NULL);

	
	printf("main done\n");
}

/* deposit 1 honey after some random seconds */
void *Honeybees(void *arg) {
	int produced;
	printf("A little bee!\n");

	for (;;) {
		// infintite!!
		
		printf("Bee %i is off collecting honey!\n", pthread_self());
		
		produced = 1;
		
		int sleepTime = rand()%10+1;
		sleep(sleepTime);
		printf("Bee is back, waiting for an opening at the pot.\n");
		sem_wait(&notFull);
		honeypoy += produced;

		printf("The bee had %i unit of honey. It took %i seconds.\n", produced, sleepTime);
		printf("The pot now has %i units of honey.\n", honeypoy);

		if (honeypoy >= maxHoney) {
			printf("pot is full, lets wake the bear!\n");
			sem_post(&full);
		} else {
			sem_post(&notFull);
		}
			
	}
}

/* will wait for &full to be set and then eats all the hnoey. */
void *Bear(void *arg) {

  printf("There is a bear and he is hungry!\n");
  printf("He will sleep until we have %i units of honey\n", maxHoney);
  for (;;) {
    sem_wait(&full);
    	
	int ate = honeypoy;
	honeypoy = 0;
    printf("\n\nRAAWR!\nThe bear has woken up!\nThe bear ate %i \n", ate);

	sem_post(&notFull);	
    
  }
}
