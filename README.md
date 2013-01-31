Concurrent Programming ID1217
=============================

Course at KTH. 
http://www.imit.kth.se/courses/ID1217/

Assignments:
============

Homework 1:

1. Compute Sum, Min and Max of Matrix Elements (20 points)

The purpose of this problem is to introduce you to the Pthreads library.
Download, compile and run a program matrixSum.c. The program computes a sum of matrix elements in parallel using Pthreads. It is described in Ch 5 of the MPD book (see Figure 5.18 on page 248).
(a) Extend the program so that in addition to sum it finds and prints a value and a position (indexes) of the maximum element of the matrix, and a value and a position of the minimum element of the matrix.  Initialize elements of the matrix to random values (rather than to all ones like in the above example) to check your solution. 
(b) Change the program developed in (a) so that the main thread prints the final results. Do not call the Barrier function, and do not use arrays for partial results, such as  sums in the above example.
(c) Change the program developed in (b) so that it uses a "bag of tasks" that is represented as a row counter, which is initialized to 0. A worker gets a task (i.e. the number of the row to process) out of the bag by reading a value of the counter and incrementing the counter as described for the matrix multiplication problem in Slides 27-29 about "bag of tasks" in Lecture 5  (see also Section 3.6.1 of the MPD textbook by Gregory Andrews). 


2. Quicksort (20 points)

The quicksort algorithm sorts the list of numbers by first dividing the list into two sublists, so that all the numbers if one sublist  are smaller than all the numbers in the other sublist. This is done by selecting one number (called a pivot) against which all other number are compared: the numbers which are less then the pivot are placed in one sublist, the numbers which more than the pivot are placed in another sublist. The pivot can be either placed in one sublist or could be withheld and placed in its final position. Develop a parallel multithreaded program (in C using Pthreads, or in Java) with recursive parallelism that implements the quicksort algorithm for sorting an array of n values. Performance evaluation: Measure and print also the execution time of your program using the times function or the gettimeofday function (see how it is done in matrixSum.c.). To calculate the execution time, read the clock after you have initialized all variables and just before you create the threads. Read the clock again as soon as the computation is complete and the worker threads have terminated.

