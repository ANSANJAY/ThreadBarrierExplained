#include <stdio.h>
#include <unistd.h>
#include "threadbarrier.h"

// Function to print the current status of the thread barrier
void thread_barrier_print(th_barrier_t *th_barrier) {
    // Print the threshold count for the barrier
    printf("th_barrier->threshold_count = %u\n", th_barrier->threshold_count);
    // Print the current number of waiting threads
    printf("th_barrier->curr_wait_count = %u\n", th_barrier->curr_wait_count);
    // Print if the barrier is ready to be used again
    printf("th_barrier->is_ready_again = %s\n", th_barrier->is_ready_again ? "true" : "false");
}

// Function to initialize the thread barrier
void thread_barrier_init(th_barrier_t *barrier, uint32_t threshold_count) {
    // Set the threshold count for the barrier
    barrier->threshold_count = threshold_count;
    // Initialize the current waiting count to 0
    barrier->curr_wait_count = 0;
    // Initialize the condition variable for the barrier
    pthread_cond_init(&barrier->cv, NULL);
    // Initialize the mutex for the barrier
    pthread_mutex_init(&barrier->mutex, NULL);
    // Set the barrier as ready to be used
    barrier->is_ready_again = true;
    // Initialize the condition variable for busy status
    pthread_cond_init(&barrier->busy_cv, NULL);
}

// Function for threads to wait at the barrier
void thread_barrier_wait(th_barrier_t *barrier) {
    // Lock the mutex to ensure atomic operations on the barrier
    pthread_mutex_lock(&barrier->mutex);

    // Wait until the barrier is ready to be used again
    while (barrier->is_ready_again == false) {
        pthread_cond_wait(&barrier->busy_cv, &barrier->mutex);
    }

    // If the current waiting count plus one equals the threshold, signal the barrier's condition variable
    if (barrier->curr_wait_count + 1 == barrier->threshold_count) {
        barrier->is_ready_again = false;
        pthread_cond_signal(&barrier->cv);
        pthread_mutex_unlock(&barrier->mutex);
        return;
    }

    // Increment the waiting count and wait on the barrier's condition variable
    barrier->curr_wait_count++;
    pthread_cond_wait(&barrier->cv, &barrier->mutex);
    barrier->curr_wait_count--;

    // If all threads have passed the barrier, reset it and broadcast to waiting threads
    if (barrier->curr_wait_count == 0) {
        barrier->is_ready_again = true;
        pthread_cond_broadcast(&barrier->busy_cv);
    } 
    // If not all threads have passed, signal the next waiting thread
    else {
        pthread_cond_signal(&barrier->cv);
    }

    // Unlock the mutex
    pthread_mutex_unlock(&barrier->mutex);
}

