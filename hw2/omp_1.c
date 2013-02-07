#include <omp.h>
#include <stdio.h>

int main(int argc, char*argv[]) {

#pragma omp parallel
	{
	 	// Code inside this region runs in parallel.
		printf("Hello!\n");
	}	
}