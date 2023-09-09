# ThreadBarrierExplained

# Thread Barrier Implementation in C 🚧

## Problem Statement 📜:

In multi-threaded programs, there are often requirements to synchronize multiple threads to ensure that they all wait for each other at a certain point before proceeding further. This is commonly achieved using a synchronization primitive called a "barrier". The goal of the barrier is to make sure that a specific number of threads reach the barrier before any of them are allowed to proceed. 

In this repository, we've implemented a thread barrier for a C program that uses POSIX threads (`pthread` library) 🧵. The problem is to ensure that three threads synchronize their execution at three distinct points (or barriers) in their execution path.

## Solution 🔍:

### Thread Barrier:

Our solution is a custom thread barrier called `th_barrier_t` which provides:

1. **threshold_count**: The number of threads required to reach the barrier before any can proceed.
2. **curr_wait_count**: The current count of threads that are waiting at the barrier.
3. **is_ready_again**: A flag indicating if the barrier is ready to be used again.
4. **cv**: A condition variable used to block and signal threads at the barrier.
5. **mutex**: A mutex ensuring atomic operations on the barrier.
6. **busy_cv**: A condition variable used to indicate if the barrier is busy or free to use.

### Key Functions 🔑:

- `thread_barrier_init(th_barrier_t *barrier, uint32_t threshold_count)`: Initializes the thread barrier with the given threshold.
  
- `thread_barrier_wait(th_barrier_t *barrier)`: Used by threads to wait at the barrier.
  
- `thread_fn_callback(void *arg)`: The main function executed by each thread. It makes each thread wait at the barrier three times, printing a message after each wait.

### Algorithm 📖:
1. **Initialization**:
   - Set `threshold_count` to the number of threads required to synchronize at the barrier.
   - Initialize `curr_wait_count` to 0.
   - Set `is_ready_again` to true.
   - Initialize `cv` and `mutex`.
   
2. **Thread Waiting at Barrier**:
   - Lock the mutex.
   - Wait until `is_ready_again` is true.
   - If the current `curr_wait_count` + 1 equals `threshold_count`, signal other waiting threads and set `is_ready_again` to false.
   - If not, increment `curr_wait_count` and wait.
   - After being signaled, decrement `curr_wait_count`.
   - If `curr_wait_count` becomes 0, set `is_ready_again` to true and broadcast to other waiting threads, otherwise signal the next waiting thread.
   - Unlock the mutex.

3. **Main Execution**:
   - Initialize the barrier.
   - Create and run three threads.
   - Each thread waits at the barrier three times and prints a message after crossing each barrier.
   - Main thread waits for all other threads to finish.
   - Print the final status of the barrier.

### Flow ⏳:

1. The main function initializes the thread barrier for 3 threads.
2. Three threads (`th1`, `th2`, and `th3`) are created.
3. Each thread waits at the barrier three times, printing a message after each wait.
4. The main function then waits for all threads to complete and prints the final status of the thread barrier.



## How to Run 🚀:

1. Clone this repository 📦.
2. Ensure you have the `pthread` library installed.
3. Compile the program using a C compiler. For example:
   ```
   gcc -o barrier_program threadbarrier.c main.c -lpthread
   ```
4. Run the compiled program 🖥️:
   ```
   ./barrier_program
   ```
   


## Main Flowchart:

```arduino
┌────────────────────────────────────────┐
│ Initialize thread barrier for 3 threads│
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│  Create three threads (th1, th2, th3)  │
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│Run thread_fn_callback for each thread  │
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│        Wait for all threads to finish  │
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│ Print the status of the thread barrier │
└────────────────────────────────────────┘
```

## thread_fn_callback:

```arduino
┌────────────────────────────────────────┐
│ Wait at the barrier using thread_barrier_wait│
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│     Print passing the first barrier    │
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│ Wait at the barrier again using thread_barrier_wait│
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│    Print passing the second barrier    │
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│Wait at the barrier once more using thread_barrier_wait│
└────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│     Print passing the third barrier    │
└────────────────────────────────────────┘
```

## thread_barrier_wait:

```arduino

┌────────────────────────────────────────┐
│                Lock the mutex          │
└────────────────────────────────────────┘
                   ↓
                ┌─────┐
    Is the barrier  │Yes│
       ready?       └─┬─┘
                No   ↓
                   ┌────────────────────────────────────────┐
                   │                  Wait                  │
                   └────────────────────────────────────────┘
                                 ↓
                ┌─────┐
    Does current │Yes│
  waiting count  └─┬─┘
   + 1 == threshold? ↓
┌─────────────────────────────────────────────────────┐
│Set barrier to "not ready", signal the condition var│
└─────────────────────────────────────────────────────┘
                   ↓
┌─────────────────────────────────────────────────────┐
│        Increment waiting count and then wait        │
└─────────────────────────────────────────────────────┘
                   ↓
                ┌─────┐
    Are all threads  │Yes│
     past barrier?   └─┬─┘
                No   ↓
┌─────────────────────────────────────────────────────┐
│            Reset barrier, broadcast to waiting threads │
└─────────────────────────────────────────────────────┘
                   ↓
┌────────────────────────────────────────┐
│             Unlock the mutex           │
└────────────────────────────────────────┘
```

## Expected Output 🖨️:

The program will print which thread has passed each of the three barriers. Due to the concurrent nature of threads, the order might vary between executions, but all three threads will always synchronize at each barrier before proceeding.

![](https://github.com/ANSANJAY/ThreadBarrierExplained/blob/main/pkg/Screenshot%20from%202023-09-08%2019-51-53.png)

## Conclusion 🌟:

This repository provides a simple yet effective implementation of a thread barrier in C using the `pthread` library. It's a great starting point for anyone looking to understand multi-threaded synchronization in C.
