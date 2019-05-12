//*********************************************************
//
// Mehmet Ozkan
// Operating Systems
// Project #3 Process Synchronization Using Pthreads: The Producer / Consumer Problem With Prime Number Detector 
// November 15, 2017
// Instructor: Dr. Ajay K. Katangur
//*********************************************************


#ifndef Statistics_H
#define Statistics_H

#include <iostream>
using namespace std;

class Statistics
{
   private:
      
       int sim_time; // simulation time
       int maxthread_sleep_time; //  max sleep time for threads
       int numof_producer_threads;  // number of producer threads
       int numof_consumer_threads; // number of consumer threads
       int sizeof_buffer; // size of the buffer
       int totalNumConsumed;// number of items consumed
       int totalNumProduced; // number of items produced

   public:
       
       int *totalThreadNumProduced; // number of items produced in array for each thread
       int *totalThreadNumConsumed; // number of items consumed in array for each thread
       int numof_timesbufferfull; // number of times the buffer was full
       int numof_timesbufferempty; // number of times the buffer was empty
       int numof_itemsremaining; // number of items that were left in the buffer

       
       void display(); // display the statistics 
       Statistics(int, int, int, int, int); // constructor for statistics class 
       ~Statistics(); // deconstructor for statistics class 
};

//********************************************************************
//
// Statistics Constructor 
//
// This function initializes all of the parameters which are given by user
// 
//
// Value Parameters
// ----------------
// simt        		int    simulation time
// maximumtsleep    int    maximum threads sleep time
// numpro    		int    number of producer threads
// numcon    		int    number of consumer threads
// buffersize       int    size of buffer

Statistics::Statistics(int simt, int maximumtsleep, int numpro, int numcon, int buffersize)
{
   // Parameters initialization
   sim_time =simt;
   maxthread_sleep_time = maximumtsleep;
   numof_producer_threads = numpro;
   numof_consumer_threads = numcon;
   sizeof_buffer = buffersize;

   // Allocating memory
   totalThreadNumProduced = new int[numpro];
   totalThreadNumConsumed = new int[numcon];

}

//********************************************************************
//
// ~Statistics Deconstructor 
//
// This function deletes pointer arrays
//

Statistics::~Statistics()
{
   delete [] totalThreadNumProduced;
   delete [] totalThreadNumConsumed;
}

//********************************************************************
//
// Display Function
//
// This function displays the statistics end of the program 
//
//
// Local Variables
// ---------------
// totalproducers		int			total produced items in each producer threads 
// totalconsumers		int			total consumed items in each consumer thread

void Statistics::display()
{
	int totalproducers = 0;
	int totalconsumers = 0;
	
   cout << "PRODUCER / CONSUMER SIMULATION COMPLETE\n";
   cout << "=======================================\n";
   cout << "Simulation Time:" << setw (28) << sim_time << endl;
   cout << "Maximum Thread Sleep Time:" << setw(18) << maxthread_sleep_time << endl;
   cout << "Number of Producer Threads:" << setw(17) << numof_producer_threads << endl;
   cout << "Number of Consumer Threads:" << setw(17) << numof_consumer_threads << endl;
   cout << "Size of Buffer:" << setw(29) << sizeof_buffer << "\n\n";
     
   for(int i=0; i < numof_producer_threads; i++)
    totalproducers += totalThreadNumProduced[i];

   //Displaying each number of items produced
   cout << "Total Number of Items Produced:" << setw(13) << totalproducers << endl;

   
   for(int i=0; i < numof_producer_threads; i++)
   {
       cout << "   Thread " << setw(2);
       cout << (i+1) << ":" << setw(31);
       cout << totalThreadNumProduced[i] << endl;
   }

   cout << endl;
  
  
   for(int i=0; i < numof_consumer_threads; i++)
    totalconsumers += totalThreadNumConsumed[i];

   // Displaying total number of items consumed
   cout << "Total Number of Items Consumed:" << setw(13) << totalconsumers << endl;

   // Displaying each threads number of items consumed
   for(int i=0; i < numof_consumer_threads; i++)
   {
       cout << "   Thread " << setw(2);
       cout << (i+numof_producer_threads+1) << ":" << setw(31);
       cout << totalThreadNumConsumed[i] << endl;
   }

   cout << endl;
   
   cout << "Number Of Items Remaining in Buffer:" << setw(8) << numof_itemsremaining << endl;
   cout << "Number Of Times Buffer Was Full:" << setw(12) << numof_timesbufferfull << endl;
   cout << "Number Of Times Buffer Was Empty:" << setw(11) << numof_timesbufferempty << endl;

}

#endif