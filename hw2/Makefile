all:	matrixSum-openmp palindromic

matrixSum-openmp:	matrixSum-openmp.o
	gcc -O -fopenmp -o matrixSum-openmp.o matrixSum-openmp.c 

palindromic:	palindromic.o
	gcc -O -fopenmp -o palindromic.o palindromic.c

clean:	
	rm -f *.o
