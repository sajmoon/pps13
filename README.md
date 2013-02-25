Concurrent Programming ID1217
=============================

Course at KTH. 
http://www.imit.kth.se/courses/ID1217/

Assignments:
============

Homework 1:
==========

1. Compute Sum, Min and Max of Matrix Elements (20 points)

The purpose of this problem is to introduce you to the Pthreads library.
Download, compile and run a program matrixSum.c. The program computes a sum of matrix elements in parallel using Pthreads. It is described in Ch 5 of the MPD book (see Figure 5.18 on page 248).
(a) Extend the program so that in addition to sum it finds and prints a value and a position (indexes) of the maximum element of the matrix, and a value and a position of the minimum element of the matrix.  Initialize elements of the matrix to random values (rather than to all ones like in the above example) to check your solution. 
(b) Change the program developed in (a) so that the main thread prints the final results. Do not call the Barrier function, and do not use arrays for partial results, such as  sums in the above example.
(c) Change the program developed in (b) so that it uses a "bag of tasks" that is represented as a row counter, which is initialized to 0. A worker gets a task (i.e. the number of the row to process) out of the bag by reading a value of the counter and incrementing the counter as described for the matrix multiplication problem in Slides 27-29 about "bag of tasks" in Lecture 5  (see also Section 3.6.1 of the MPD textbook by Gregory Andrews). 


2. Quicksort (20 points)

The quicksort algorithm sorts the list of numbers by first dividing the list into two sublists, so that all the numbers if one sublist  are smaller than all the numbers in the other sublist. This is done by selecting one number (called a pivot) against which all other number are compared: the numbers which are less then the pivot are placed in one sublist, the numbers which more than the pivot are placed in another sublist. The pivot can be either placed in one sublist or could be withheld and placed in its final position. Develop a parallel multithreaded program (in C using Pthreads, or in Java) with recursive parallelism that implements the quicksort algorithm for sorting an array of n values. Performance evaluation: Measure and print also the execution time of your program using the times function or the gettimeofday function (see how it is done in matrixSum.c.). To calculate the execution time, read the clock after you have initialized all variables and just before you create the threads. Read the clock again as soon as the computation is complete and the worker threads have terminated.


Homework 2:
===========
3. Find Palindromic Words (40 points)

Develop a parallel multithreaded program (in C/C++ using OpenMP) to solve the following problem. There is an online dictionary in /usr/dict/words under Linux or Solaris. The file contains 25,143 words (and is used by the Unix spell command). You can download the file words. Recall that a palindrome is a word or phrase that reads the same in either direction, i.e., if you reverse all the letters you get the same word or phrase. Your task is to find all palindromic words in the dictionary. A word is palindromic if its reverse is also in the dictionary. For example, "noon" is palindromic, because it is a palindrome and hence it's reverse is trivially in the dictionary. A word like "draw" is palindromic because "ward" is also in the dictionary. 
Your parallel program should use for-loop parallelism or tasks and the number of threads used should be a command-line argument. Do the input and output phases sequentially and the rest in parallel. Your program should write the palindromic words to a results file. 

Run the program on different number of processors and report the speedup (sequential execution time divided by parallel execution time) for different number of processors (up to at least 4) and different workloads (at least 3 different sizes of a dictionary). Run each program several times (at least 5) and use the median value for execution time. Try to provide reasonable explanations to your  results. Measure only the parallel part of your program and try to adapt the workload so that the sequential execution time is at least 1-2 seconds. Specify the number of processors used by specifying different number of threads (set the OMP_NUM_THREADS environment variable or use a call to omp_set_num_threads(), see the OpenMP specification).
To measure the execution time, use  the omp_get_wtime function (see omp_get_wtime).

Homework 3:
==========

1. The Hungry Birds Problem  (one producer - multiple consumers) (20 points)

Given are n baby birds and one parent bird. The baby birds eat out of a common dish that initially contains W worms. Each baby bird repeatedly takes a worm, eats it, sleeps for a while, takes another worm, and so on. If the dish is empty, the baby bird who discovers the empty dish chirps real loud to awaken the parent bird. The parent bird flies off and gathers W more worms, puts them in the dish, and then waits for the dish to be empty again. This pattern repeats forever.
Develop and implement a multithreaded program to simulate the actions of the birds. Represent the birds as concurrent threads (i.e. array of "babyBird" threads and a "parentBird" thread), and the dish as a critical shared resource that can be accessed by at most one bird at a time. Use only semaphores for synchronization. Your program should print a trace of interesting simulation events. Is your solution fair? Explain in comments to the source code. 
2. The Bear and Honeybees Problem (multiple producers - one consumer) (20 points)

Given are n honeybees and a hungry bear. They share a pot of honey. The pot is initially empty; its capacity is H portions of honey. The bear sleeps until the pot is full, then eats all the honey and goes back to sleep. Each bee repeatedly gathers one portion of honey and puts it in the pot; the bee who fills the pot awakens the bear.
Develop and implement a multithreaded program to simulate the actions of the bear and honeybees. Represent the bear and honeybees as concurrent threads (i.e. a "bear" thread and an array of "honeybee" threads), and the pot as a critical shared resource that can be accesses by at most one thread at a time. Use only semaphores for synchronization. Your program should print a trace of interesting simulation events. Is your solution fair (w.r.t. honeybees)? Explain in comments to the source code.

Homework 4:
===========
1. A Fuel Space Station (40 points)

Consider a future fuel station in space that supplies nitrogen and quantum fluid. Assume that the station has a capacity of handling V space vehicles in parallel (simultaneously) and a maximum storage for N liters of nitrogen and for Q liters of quantum fluid. When a vehicle arrives at the station it requests the needed amount of fuel of the two different types or only one of the types. If there is not enough fuel of either type, the vehicle has to wait without blocking other vehicles. Special supply vehicles deliver fuel in fixed quantities that far exceed a fuel tank capacity of any vehicle. When a supply vehicle arrives at the station, it delays until there is enough space to deposit the delivered amount of fuel. To travel back, the supply vehicles also request a certain amount of fuel of the two different types or one of the types just like an ordinary vehicle, not necessarily the type it supplies.
Develop and implement a multithreaded application (in Java or C++) that simulates the actions of the ordinary space vehicles and supply vehicles represented as concurrent threads. Your simulation program should implemented all "real world" concurrency in actions of the vehicles as described in the above scenario.Represent the fuel space station as a monitor (a synchronized object) containing a set of counters that define the amounts of available fuel of different types (at most  N liters of nitrogen and at most Q liters of quantum fluid) and the number of free docking places (at most V places). The monitor should be used to control access to and the use of the station. Develop and implement the monitor's methods. The vehicle threads calls the monitor methods to request and release  access to the station in order to get or/and deposit fuel. 

In your simulation program, assume that each vehicle periodically arrives at the station to get/supply fuel. Have the vehicles sleep (pause) for a random amount of time between arriving at the station to simulate the time it takes to travel in space; and have the vehicles sleep (pause) for a smaller random amount of time to simulate the time it takes to get/supply the fuel at the station. Stop the simulation after each vehicle has arrived at the station trips (the given amount) times. Your program should print a trace of the interesting events in the program. 
Is your solution fair? Explain (in comments in the source code).

