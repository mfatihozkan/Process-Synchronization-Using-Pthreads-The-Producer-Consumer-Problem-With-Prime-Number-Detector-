//*********************************************************
//
// Mehmet Ozkan
// Operating Systems
// Project #3 Process Synchronization Using Pthreads: The Producer / Consumer Problem With Prime Number Detector 
// November 15, 2017
// Instructor: Dr. Ajay K. Katangur
//*********************************************************

#ifndef _BUFFER_H_DEFINED_
#define _BUFFER_H_DEFINED_

#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <algorithm>

using namespace std;

typedef int buffer_item;

#define BUFFER_SIZE 5
 

unsigned int buffer_write_index=0; // writing index position in buffer
unsigned int buffer_read_index=0; // reading index position in the buffer
unsigned int bufferCount=0;  // tracking the number of items in buffer
double buffer_array[BUFFER_SIZE];

// Functions
void buffer_initialize();
bool buffer_insert_item( buffer_item item );
bool buffer_remove_item( buffer_item *item );
void buffer_display();


// Semaphores and mutexs
sem_t bufferEmpty;
sem_t bufferFull;
pthread_mutex_t mutex;


//********************************************************************
//
// Initializing Buffer
//
// This function initializes the mutex,semaphore and buffer positions to -1
//

void buffer_initialize()
{
   fill_n(buffer_array, 5, -1); // Intialize the items in the buffer with -1
   sem_init(&bufferEmpty, 0, BUFFER_SIZE );
   sem_init( &bufferFull, 0, 0 );
   pthread_mutex_init( &mutex, NULL );
	
}

//********************************************************************
//
// Buffer Insert Item Function
//
// This function inserts an item into the buffer and buffer moves write index 
//
// Value Parameters
// ----------------
// item      	 	buffer_item  			 A value to insert item into buffer

bool buffer_insert_item( buffer_item item )
{
   

   // insert an item into buffer
   buffer_array[buffer_write_index] = item;

   // moving the write buffer index by one 
   buffer_write_index = (buffer_write_index + 1) % BUFFER_SIZE;
  
   
   bufferCount++;

   return true;
}

//********************************************************************
//
// Buffer Remove Item Function
//
// This function removes an item from the buffer, buffer read index moves 
// and decrease the number of items in the buffer
//
// Reference Parameters
// ----------------	
// item       			buffer_item   			A value to remove items from buffer

bool buffer_remove_item( buffer_item *item )
{
   
   *item = buffer_array[buffer_read_index];

   // moving the read index position by one
   buffer_read_index = ( buffer_read_index + 1) % BUFFER_SIZE;

  
   bufferCount--;

   return true;
}

//********************************************************************
//
// Buffer Display
//
// This function displays the number of buffers occupied,items in the buffer and
// displays the position of write and read indexes
// 

void buffer_display()
{
   
   cout << "(buffers occupied: " << (uint) bufferCount % BUFFER_SIZE+1 << ")\n";
   cout << "buffers: ";
     
   for(int i=0; i < BUFFER_SIZE; i++)
       cout << "  " << buffer_array[i] << " ";
   cout << endl; 
   cout << "        ";
   for(int i=0; i < BUFFER_SIZE; i++)
       cout << " ----";
   cout << endl;
   cout << "         ";
   for(int i=0; i < BUFFER_SIZE; i++)
   {
    cout << " ";

       if(buffer_write_index == i || buffer_read_index == i)
       {
           if( bufferCount == BUFFER_SIZE )
               cout << "WR";
           else if( bufferCount == 0 )
               cout << "RW";
           else if( buffer_write_index == i )
               cout << "W ";
           else
               cout << "R ";

               cout << " ";
       }
       else
               cout << "    ";
   }
   cout << endl << endl;


}

#endif // _BUFFER_H_DEFINED_
