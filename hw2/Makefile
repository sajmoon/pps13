all:	matrixSum-openmp

matrixSum-openmp:	matrixSum-openmp.o
	gcc -O -fopenmp -o matrixSum-openmp.o matrixSum-openmp.c 

clean:	
	rm -f *.o
