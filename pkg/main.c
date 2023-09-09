#include <stdio.h>
#include "threadbarrier.h"

// Declare and initialize the thread barrier.
static th_barrier_t th_barrier;

// The main function for each thread.
void * thread_fn_callback (void *arg) {
    
    // First wait at the thread barrier.
    thread_barrier_wait(&th_barrier);
    // Print which thread has passed the first barrier.
    printf("1st barricade cleared by thread %s\n", (char *)arg);

    // Second wait at the thread barrier.
    thread_barrier_wait(&th_barrier);
    // Print which thread has passed the second barrier.
    printf("2nd barricade cleared by thread %s\n", (char *)arg);

    // Third wait at the thread barrier.
    thread_barrier_wait(&th_barrier);
    // Print which thread has passed the third barrier.
    printf("3rd barricade cleared by thread %s\n", (char *)arg);
    
    // Exit the thread after all barriers are crossed.
    pthread_exit(0);
    return NULL;
}

// Thread objects.
static pthread_t pthreads[3];

int main() {
    // Initialize the thread barrier for 3 threads.
    thread_barrier_init(&th_barrier, 3);
    
    // Create the first thread and pass the thread's name.
    static const char *th1 = "th1";
    pthread_create(&pthreads[0], NULL, thread_fn_callback, (void *)th1);
    
    // Create the second thread and pass the thread's name.
    static const char *th2 = "th2";
    pthread_create(&pthreads[1], NULL, thread_fn_callback, (void *)th2);
    
    // Create the third thread and pass the thread's name.
    static const char *th3 = "th3";
    pthread_create(&pthreads[2], NULL, thread_fn_callback, (void *)th3);
    
    // Join the threads, i.e., wait for all threads to finish.
    pthread_join(pthreads[0], NULL);
    pthread_join(pthreads[1], NULL);
    pthread_join(pthreads[2], NULL);

    // Print the final status of the thread barrier.
    thread_barrier_print(&th_barrier);
    return 0;
}


