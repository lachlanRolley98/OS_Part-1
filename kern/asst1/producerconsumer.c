/* This file will contain your solution. Modify it as you wish. */
#include <types.h>
#include <lib.h>
#include <synch.h>
#include "producerconsumer.h"

/* Declare any variables you need here to keep track of and
   synchronise your bounded buffer. A sample declaration of a buffer is shown
   below. It is an array of pointers to items.
   
   You can change this if you choose another implementation. 
   However, your implementation should accept at least BUFFER_SIZE 
   prior to blocking
*/




#define BUFFLEN (BUFFER_SIZE + 1)

data_item_t * item_buffer[BUFFER_SIZE+1];

struct semaphore *hasRoomSem;   // This sem is itinitalised to 10 and makes sure only 10 things can be produced or it sleeps (goes down when consumed)
struct semaphore *readyToUseSem; // This sem is initialised to 0. Goes up when things are made, down when used

struct lock *myLock; 

volatile int head, tail;


/* consumer_receive() is called by a consumer to request more data. It
   should block on a sync primitive if no data is available in your
   buffer. It should not busy wait! */

data_item_t * consumer_receive(void)
{
        data_item_t * item;

        P(readyToUseSem); // makes sure there is somthing to use
        
        lock_acquire(myLock); 
        item = item_buffer[tail];
        tail = (tail + 1) % BUFFLEN;
        lock_release(myLock);
        
        V(hasRoomSem); // tells producer we made room in the buffer
       

        return item;
}

/* procucer_send() is called by a producer to store data in your
   bounded buffer.  It should block on a sync primitive if no space is
   available in your buffer. It should not busy wait!*/

void producer_send(data_item_t *item)
{
        P(hasRoomSem); // checks if there is room in buffer

        lock_acquire(myLock); 
        item_buffer[head] = item;
        head = (head + 1) % BUFFLEN;
        lock_release(myLock);

        V(readyToUseSem); // tells semaphore used by consumer we good to go
}




/* Perform any initialisation (e.g. of global data) you need
   here. Note: You can panic if any allocation fails during setup */

void producerconsumer_startup(void)
{
        head = tail = 0;
        hasRoomSem = sem_create("hasRoomSem", 10); // This sem is itinitalised to 10 and makes sure only 10 things can be produced or it sleeps. (goes down when consumed)
        readyToUseSem = sem_create("readyToUseSem", 0); 
        myLock = lock_create("myLock");

        
         KASSERT(hasRoomSem != 0);
         KASSERT(readyToUseSem != 0);
         KASSERT(myLock != 0);



}

/* Perform any clean-up you need here */
void producerconsumer_shutdown(void)
{
   sem_destroy(hasRoomSem);
   sem_destroy(readyToUseSem);
   lock_destroy(myLock);
}

