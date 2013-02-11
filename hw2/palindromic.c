#include <string.h>
#include <stdio.h>
#include <omp.h>

#define WORDSIZE 27000
#define MAXWORKERS 10 

int numWorkers, size;
double start_time, end_time;

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


int main(int argc, char *argv[]) {
  	int totalWordCount = 1;
  	
	printf("\n\nFind palindromc words\n");
	

	numWorkers = (argc > 1)? atoi(argv[1]) : MAXWORKERS;
	totalWordCount = (argc > 2)? atoi(argv[2]) : WORDSIZE;

	printf("Using %i processors\n", numWorkers);
	char * words[WORDSIZE] = {0};
	
	int wordCount = 1;
	for (wordCount = 0; wordCount < totalWordCount; wordCount++) {
		char* pStr = NULL;

		pStr = (char*)malloc(50);
		gets(pStr);

		if (strcmp(pStr, "") == 0 ) {
			// if end of file
			
			break;
		}
		
		words[wordCount] = pStr;
		
	}

	omp_set_num_threads(numWorkers);

 	start_time = omp_get_wtime();
	int i = 0;
	//for (i = 0; i < wordCount; i++) {
	//	printf("words in list: %s\n", words[i]);
	//
	int j = 0;
	int palindromCount = 0;

	for (j = 0; j < wordCount; j++) {
		char str[50];

		strcpy( str, words[j] );
		rev(str);
		// printf("words %s %s\n", str, words[j]);

		// printf("str: %s\n", str);
		
		// printf("revstr: %s\n", str);
		
		// printf("revstr: %i %s -> %s (%i)\n", j, str, words[j], strcmp(str, words[j]));

		// printf("rev(str) %s\n", rev(str));
		// printf("str: %s\n", str);
		int a;
#pragma omp parallel for reduction (+:palindromCount) private(i)
		for (i = j; i < wordCount; i++) {
			char str2[50];
			strcpy(str2, words[i]);
			// printf("%s %s %i\n", str, str2, strcmp(str, str2));
			// printf("evstr: %s -> %s (%i)\n", str, str2, strcmp(str, str2));

			if ( strcmp(str, str2) == 0 ) {
				//printf("found palidrom %s -> %s\n", words[i], words[j]);
				palindromCount++;
			}
		}
	}


  	end_time = omp_get_wtime();

  	printf("\nResult:\n");
	printf("Found %i palindroms, ", palindromCount);
  	printf("it took %g seconds\n", end_time - start_time);
  	printf("using %i workers for %i words\n", numWorkers, wordCount);
}