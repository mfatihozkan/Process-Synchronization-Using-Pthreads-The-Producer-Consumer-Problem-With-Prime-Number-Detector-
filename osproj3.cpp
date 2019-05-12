//*********************************************************
//
// Mehmet Ozkan
// Operating Systems
// Project #3 Process Synchronization Using Pthreads: The Producer / Consumer Problem With Prime Number Detector 
// November 15, 2017
// Instructor: Dr. Ajay K. Katangur
//*********************************************************

#include <pthread.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/time.h>
#include "buffer.h"
#include "statistics.h"
#include <semaphore.h>
#include <cstdlib>

using namespace std;


struct Parameters 
{
   bool Buffersnapshot; // yes or no for output buffer snapshots
   int simtime; // simulation length
   int maximumsleep; // maximum sleep time for producer and consumer threads
   int NumberOfProThreads; // number of producer threads
   int NumberOfConThreads; // number of consumer threads 
  
};

 // Function prototypes
 
void Arguments(int, char **);
bool ifPrime(buffer_item);
void *Producer(void * param);
void *Consumer(void * param);

Parameters object; 
bool flag = true; // bool variable to set a simulation flag  

unsigned int seed;
unsigned int seed1;
unsigned int seed2;
unsigned int seed3;


int main( int argc, char *argv[] )
{
  
  // Get command line arguments by user 
   Arguments(argc, argv);
  
  //Initialize the buffer
   buffer_initialize();
     
  
   Statistics objectstats(object.simtime, object.maximumsleep,object.NumberOfProThreads, object.NumberOfConThreads,BUFFER_SIZE);

   
    pthread_t tidP[object.NumberOfProThreads]; // Producer threads id array
    pthread_t tidC[object.NumberOfConThreads]; // Consumer thread id array
      
   cout << "Starting Threads...\n";
   
   if(object.Buffersnapshot) //if snapshots enabled
    buffer_display(); //print snapshots
			   
   //Create producer threads
   for(int i=0; i < object.NumberOfProThreads; i++)
   {
       pthread_create( &tidP[i], NULL, Producer, (void*)&object);
   }

   //Create consumer threads
   for(int i=0; i < object.NumberOfConThreads; i++)
   {
       pthread_create( &tidC[i], NULL, Consumer, (void*)&object);
   }
  
   // Thread Safe Sleep
   
   struct timespec globaltime;
   globaltime.tv_sec = object.simtime;
   globaltime.tv_nsec = 0;
   nanosleep(&globaltime,(struct timespec*)NULL);
   
   flag = false;

   
   // Accept array from threads
   int *stats_array_threads = new int[2];

   // Join Threads
  
   for(int i=0; i < object.NumberOfProThreads; i++)
   {
       
       // Store stats for each thread
       (int) pthread_join( tidP[i], (void**) &stats_array_threads );
	  
       objectstats.totalThreadNumProduced[i] = stats_array_threads[0];
       objectstats.numof_timesbufferfull += stats_array_threads[1];
	
   }
       
   for(int i=0; i < object.NumberOfConThreads; i++)
   {
       //Store stats for each thread
       (int) pthread_join( tidC[i],(void**)&stats_array_threads );

       objectstats.totalThreadNumConsumed[i] = stats_array_threads[0];
       objectstats.numof_timesbufferempty += stats_array_threads[1];
   }
       
  
   
   objectstats.numof_itemsremaining = bufferCount;

   //Display Statistics
   objectstats.display();

   //Exit
   return 0;
}

//********************************************************************
//
// Arguments function
//
// This function gets the command line arguments and
// places them into a struct data.
//
// Value Parameters
// ----------------
// argc       int       Number of arguments in argv
//
// Reference Parameters
// --------------------
// argv    char       	 Array of char arrays
//

void Arguments(int argc, char *argv[])
{
	
 for(int i=0; i <= argc; i++)
	{
   if(argc >= 7 || argc < 6 )
   {
       cerr << "Please write 5 arguments.The arguments should be ";
       cerr << "in the following order:\nSimulation length\n";
       cerr << "Max thread sleep\nNumber of producers\nNumber ";
       cerr << "of consumers\nyes or no for displaying buffer snapshots\n";
       exit(-1);
   }
	}
	
 for(int i=1; i<=4; i++)
 {
	 if(atoi(argv[i])<=0)
	 {
	cerr << "Please write positive integers\n";
    exit(-1);	 
	 }
 }
 
   object.simtime = atoi(argv[1]);
   object.maximumsleep = atoi(argv[2]);
   object.NumberOfProThreads = atoi(argv[3]);
   object.NumberOfConThreads = atoi(argv[4]);
    
    
   if(strcmp(argv[5],"yes") == 0)
       object.Buffersnapshot = 1;
   else if(strcmp(argv[5],"no") == 0)
       object.Buffersnapshot = 0;
   else
   {
       cerr << "The last argument must be 'yes' or 'no'"<<endl;
       exit(-1);
   }

}

//********************************************************************
//
// Prime Checking Function
//
// This function determines if a number is prime
//
// Return Value
// ------------
// bool                 True/False if number is prime
//
// Value Parameters
// ----------------
// number			    buffer_item   
//
// Local Variables
// ---------------
// limit       int      Limit for checking a prime number
// result      int     	Result value of mod operation


bool ifPrime(buffer_item number)
{
   int limit = number/2;
   int result;

   for(int i=2; i<=limit; i++)
   {
       result = number % i;
       if(result == 0)
       return false;
   }

   return true;

}

//********************************************************************
//
// Producer function
//
// Producer function to produce numbers and insert them into the buffer
//
//
// Local Variables
// ---------------
// stats    int        stores statisticss for number of times buffer is full and number of items produced.
// time		int		   stores random number in time for sleeping

void *Producer(void * param)
{
   
   int * stats = new int[2]; 
   int time; // stores random number in time for sleeping
   stats[0] = 0; // stores number of items are produced
   stats[1] = 0; // stores number  of times buffer is full
   buffer_item proitem; 

   while(flag)
   {
       // Sleep thread safe with maximumsleep time
	   	time= rand_r(&seed);
		time= time % object.maximumsleep;
		
	    struct timespec protime;
	    protime.tv_sec = time;
		protime.tv_nsec = 0;
		nanosleep(&protime,(struct timespec*)NULL);
		
	   //Generate a random number
       proitem = rand_r ( &seed2 );
	   proitem %= 99;
      

       // Check to see if buffer is full
       if(bufferCount == BUFFER_SIZE)
       {
           cout << "All buffers full. Producer " << pthread_self();
           cout << " waits.\n\n";
           stats[1]=(stats[1] + 1);
       }

       //Wait
       sem_wait( &bufferEmpty );

       //Lock
       pthread_mutex_lock( &mutex );

       //*******CRITICAL SECTION*******
       if(buffer_insert_item(proitem))
       {
           cout << "Producer " << pthread_self() << " writes ";
           cout << proitem << endl;

           stats[0]= (stats[0] + 1); //add number of items produced

           if(object.Buffersnapshot) //if snapshots enabled
               buffer_display(); //print snapshots
       }

       //unlock
       pthread_mutex_unlock( &mutex );

       //signal
       sem_post( &bufferFull );

   }

   pthread_exit( (void*) stats ); //return statistics for this thread
}  

//********************************************************************
//
// Consumer function
//
// Consumer function to consume numbers and check if the number is prime, and remove them from the buffer
//
//
// Reference Parameters
// --------------------
// void
// thread returns stats value
//
// Local Variables
// ---------------
// stats    int        holds stats for  number of times buffer is empty and number of items consumed.
// time		int		   stores random number in time for sleeping

void *Consumer(void * param)
{
   
   buffer_item conitem; // conitem to store received item
   int time; // stores the random time to sleep
   int * stats1 = new int[2]; 
   stats1[0] = 0; // stores number of items consumed
   stats1[1] =0; // stores number of times buffer was empty

   while(flag)
   {
       
	   // Sleep thread safe with maximumsleep time
	   time= rand_r(&seed2);
	   time= time % object.maximumsleep;
       struct timespec contime;
	   contime.tv_sec = time;
	   contime.tv_nsec = 0;
	   nanosleep(&contime,(struct timespec*)NULL);
	   
       if(bufferCount == 0)
       {
           cout << "All buffers empty. Consumer ";
           cout << pthread_self();
           cout << " waits.\n\n";

           stats1[1]= (stats1[1] + 1); //add one time buffer was empty
       }

       // Check semaphore
       sem_wait( &bufferFull );

       // Lock
       pthread_mutex_lock( &mutex );

        //*******CRITICAL SECTION*******
       if(buffer_remove_item( &conitem)) //consumes in if statement
       {
           //line 1
           cout << "Consumer " << pthread_self() << " reads ";
           cout << conitem;

           stats1[0] = (stats1[0]+1); //increase number of items consumed

           if(ifPrime(conitem)) //find if number was prime
           cout << "   * * * PRIME * * *";
           cout << endl;

           // If snapshots are enabled
           if(object.Buffersnapshot)
               buffer_display();
       }

       //Unlock
       pthread_mutex_unlock( &mutex );

       //Signal
       sem_post( &bufferEmpty );

   }

   pthread_exit( (void*)stats1 ); //return statistics for this thread

}
