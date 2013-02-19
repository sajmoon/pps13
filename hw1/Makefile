OBJS=matrixSum.o quicksort.o

all:	matrixSum quicksort

matrixSum:	matrixSum.o
	gcc -o matrixSum.o matrixSum.c -lpthread

quicksort:	quicksort.o
	gcc -o quicksort.o quicksort.c -lpthread

clean:	
	rm -f *.o
