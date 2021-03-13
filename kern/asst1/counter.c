#include "opt-synchprobs.h"
#include "counter.h"
#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>


/*
 * Declare the counter variable that all threads increment or decrement
 * via the interface provided here.
 *
 * Declaring it "volatile" instructs the compiler to always (re)read the
 * variable from memory and not to optimise by keeping the value in a process 
 * register and avoid memory references.
 *
 * NOTE: The volatile declaration is actually not needed for the provided code 
 * as the variable is only loaded once in each function.
 */

static volatile int the_counter;

/*
 * ********************************************************************
 * INSERT ANY GLOBAL VARIABLES YOU REQUIRE HERE
 * ********************************************************************
 */
struct lock *myLock; 


//void lock_acquire(struct lock *);
//void lock_release(struct lock *);
//bool lock_do_i_hold(struct lock *);  // dont think i need for this part




void counter_increment(void)
{
        //grab lock
        lock_acquire(myLock);      // if it cant get the lock, im pre sure it just chills, dont need to check if it works cos if it doesnt, it will just come back later

        the_counter = the_counter + 1;

        lock_release(myLock);
}

void counter_decrement(void)
{       
        lock_acquire(myLock);

        the_counter = the_counter - 1;

        lock_release(myLock);
}

int counter_initialise(int val)
{
        the_counter = val;


        //aight game plan big dog
        //my understanding is heaps of threads are gona be calling counter up and down and we only want them to 
        // work when another thread isnt using the variable
        // in this case we just wana lock the "the_counter" when somone is using it then unlock when done




        /*
         * ********************************************************************
         * INSERT ANY INITIALISATION CODE YOU REQUIRE HERE
         * ********************************************************************
         */
        myLock = lock_create("myLock"); // ok now that there is a lock created, we wana make sure we can hold it before we do shit
        //im guessing we put that in all the increment and decrement but have no idea who or when it is called. GL king
        
        
        /*
         * Return 0 to indicate success
         * Return non-zero to indicate error.
         * e.g. 
         * return ENOMEM
         * indicates an allocation failure to the caller 
         */
        if(myLock == NULL){       // might need to derefence myLock for this *myLock == NULL , cant remember c
                return ENOMEM;
        } 
        
        return 0;
}

int counter_read_and_destroy(void)
{
        /*
         * **********************************************************************
         * INSERT ANY CLEANUP CODE YOU REQUIRE HERE
         * **********************************************************************
         */

        //think just gota destroy the lock now, idk how to do it accross all threads ? 
        lock_destroy(myLock);

        return the_counter;
}
