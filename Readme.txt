//*********************************************************
// Mehmet Ozkan
// Operating Systems
// Project #3 Process Synchronization Using Pthreads: The Producer / Consumer Problem With Prime Number Detector 
// November 15, 2017
// Instructor: Dr. Ajay K. Katangur
//*********************************************************


		Contents of the Directory

1.Readme.txt	- Describing the contents of directory and solutions of the problems
2.osproj3.cpp	- Compiled main source file 
3.buffer.h	- Buffer header file
4.statistics.h	- Statistics header file for displaying statistics
5.Makefile	- Extraction portions to be compiled


		Methods of Achieving the Project Requirements 

- Circular buffer array is created to store and remove items by producer and consumer threads.  
- Random numbers are generated in producer funcntion by using the rand_r() function.
- Nanosleep() thread safe sleep fuctions are used for each consumer and producer threads, and main thread to sleep with specific time. 
- Semaphores and mutex are used to solve producer and consumer problem.
- fill_n() function is used to initialize buffer array by -1.
- Statistics class structure is created to display statistics end of the program in efficient manner. 