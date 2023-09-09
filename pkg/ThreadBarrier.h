#ifndef __TH_BARRIER__
#define __TH_BARRIER__

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

// Define a thread barrier structure
typedef struct th_barrier_ {
    uint32_t threshold_count;         // Number of threads that must call thread_barrier_wait() 
                                      // before any of them successfully return from the call
    uint32_t curr_wait_count;         // Current count of threads waiting on the barrier
    pthread_cond_t cv;                // Condition variable for threads to block until barrier is lifted
    pthread_mutex_t mutex;            // Mutex to protect barrier state
    bool is_ready_again;              // Flag to check if barrier can be reused
    pthread_cond_t busy_cv;           // Condition variable to block threads until barrier is ready again
} th_barrier_t;

// Function to initialize the thread barrier
// Parameters:
// barrier - pointer to the thread barrier object
// threshold_count - number of threads required to lift the barrier
void
thread_barrier_init ( th_barrier_t *barrier, 
                      uint32_t threshold_count);

// Function to make a thread wait at the barrier.
// Once the threshold_count threads have called this function, 
//They can all continue their execution
// Parameters:
// barrier - pointer to the thread barrier object
void
thread_barrier_wait ( th_barrier_t *barrier);

// Function to destroy the thread barrier and free up any resources.
// It's crucial to call this function to avoid any memory leaks and to clean up 
// the mutex and condition variables.
// Parameters:
// barrier - pointer to the thread barrier object
void
thread_barrier_destroy ( th_barrier_t *barrier );

#endif 
