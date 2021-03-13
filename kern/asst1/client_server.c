/* This file will contain your solution. Modify it as you wish. */
#include <types.h>
#include <lib.h>
#include <synch.h>
#include <kern/errno.h>
#include "client_server.h"

/*
 * Declare any variables you need here to implement and
 *  synchronise your queues and/or requests.
 */


request_t * request_buffer[1000]; // can do as linked list or hard code
volatile int head, tail;

struct lock *myLock; 
struct semaphore *mySem;

/* work_queue_enqueue():
 *
 * req: A pointer to a request to be processed. You can assume it is
 * a valid pointer or NULL. You can't assume anything about what it
 * points to, i.e. the internals of the request type.
 *
 * This function is expected to add requests to a single queue for
 * processing. The queue is a queue (FIFO). The function then returns
 * to the caller. It can be called concurrently by multiple threads.
 *
 * Note: The above is a high-level description of behaviour, not
 * detailed psuedo code. Depending on your implementation, more or
 * less code may be required. 
 */

   

void work_queue_enqueue(request_t *req)
{
        //think this just adds stuff to the buffer, pre much just wana make sure somone else isnt doing at same time so get correct pointers
        lock_acquire(myLock);
        V(mySem);
        request_buffer[tail] =  req;
        tail++;
        lock_release(myLock);

        
}

/* 
 * work_queue_get_next():
 *
 * This function is expected to block on a synchronisation primitive
 * until there are one or more requests in the queue for processing.
 *
 * A pointer to the request is removed from the queue and returned to
 * the server.
 * 
 * Note: The above is a high-level description of behaviour, not
 * detailed psuedo code. Depending on your implementation, more or
 * less code may be required.
 */


request_t *work_queue_get_next(void)
{
        lock_acquire(myLock);
        request_t *req;
        P(mySem);
        req = request_buffer[head];
        head ++;
        lock_release(myLock);

        return req;
}




/*
 * work_queue_setup():
 * 
 * This function is called before the client and server threads are started. It is
 * intended for you to initialise any globals or synchronisation
 * primitives that are needed by your solution.
 *
 * In returns zero on success, or non-zero on failure.
 *
 * You can assume it is not called concurrently.
 */

int work_queue_setup(void)
{
        mySem = sem_create("mySem", 0);    // gota do that error checking shit if theres enough mem
        head = tail = 0;
        myLock = lock_create("myLock");
        for(int i = 0 ; i <= 1000 ; i++){
                request_buffer[i] = NULL;
        }     
        return 0;

}


/* 
 * work_queue_shutdown():
 * 
 * This function is called after the participating threads have
 * exited. Use it to de-allocate or "destroy" anything allocated or created
 * on setup.
 *
 * You can assume it is not called concurrently.
 */

void work_queue_shutdown(void)
{

}
