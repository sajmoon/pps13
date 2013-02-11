#include <string.h>
#include <stdio.h>
#include <omp.h>

/* Defines max workers and ma dictionary size */
#define WORDSIZE 27000
#define MAXWORKERS 10 

int numWorkers, size;
double start_time, end_time;

/* reverses a string and overwrites it with the new value. */
char* rev(char* str)
{
  int end= strlen(str)-1;
  int start = 0;

  while( start<end )
  {
    str[start] ^= str[end];
    str[end] ^=   str[start];
    str[start]^= str[end];

    ++start;
    --end;
  }

  return str;
}

/* Main method */
int main(int argc, char *argv[]) {
  	int totalWordCount = 1;
  	
	printf("\n\nFind palindromc words\n");
	totalWordCount = 0;

	//sets workers and dictionary list according to input parameters
	numWorkers = (argc > 1)? atoi(argv[1]) : MAXWORKERS;
	totalWordCount = (argc > 2)? atoi(argv[2]) : WORDSIZE;

	printf("Using %i processors\n", numWorkers);
	char * words[WORDSIZE] = {0};
	int  palindromicWords[WORDSIZE] = {0};
	
	int wordCount = 1;
	for (wordCount = 0; wordCount < totalWordCount; wordCount++) {
		char* pStr = NULL;

		pStr = (char*)malloc(50);
		gets(pStr);

		if (strcmp(pStr, "") == 0 ) {
			// if end of file
			// We have no more workds to play with.
			
			break;
		}
		
		// Keeps track of how manywords we have. 
		words[wordCount] = pStr;
		
	}

	// set number of threads/workers
	omp_set_num_threads(numWorkers);

 	start_time = omp_get_wtime();
	int i = 0;
	int j = 0;
	int palindromCount = 0;

	for (j = 0; j < wordCount; j++) {
		// Check all words once.
		char str[50];

		strcpy( str, words[j] );
		rev(str);
		
		int a;
		#pragma omp parallel for reduction (+:palindromCount) private(i)
		for (i = 0; i < wordCount; i++) {
			// and for that word(i) check agains all other words
			// if we only want one of the terms on the list, 
			// like draw / ward we could set this to i = j;
			char str2[50];
			strcpy(str2, words[i]);
		
			if ( strcmp(str, str2) == 0 ) {
				// found a palindromic word
				palindromicWords[i] = 1;
				palindromCount++;
			}
		}
	}


  	end_time = omp_get_wtime();

  	FILE *fp;
	fp=fopen("result", "w");
	

  	for (i = 0; i < wordCount; i++) {
  		if (palindromicWords[i] == 1) {
  			fprintf(fp, "%s\n", words[i]);
  		}
  	}

  	printf("\nResult:\n");
	printf("Found %i palindroms, ", palindromCount);
  	printf("it took %g seconds\n", end_time - start_time);
  	printf("using %i workers for %i words\n", numWorkers, wordCount);
}