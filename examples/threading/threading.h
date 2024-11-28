#ifndef THREADING_H
#define THREADING_H

#include <stdbool.h>
#include <pthread.h>

/**
 * This structure is dynamically allocated and passed as
 * an argument to the thread using pthread_create.
 * It should be returned by the thread so it can be freed by
 * the joiner thread.
 */
struct thread_data {
    pthread_mutex_t *mutex;         // Pointer to the mutex to lock/unlock
    int wait_to_obtain_ms;          // Time to wait before obtaining the mutex
    int wait_to_release_ms;         // Time to hold the mutex before releasing it
    bool thread_complete_success;   // Indicates whether the thread completed successfully
};

/**
 * Thread entry function.
 * Waits for a specified duration, locks a mutex, holds it for a while, and then releases it.
 * The thread returns a pointer to its `thread_data` structure when it exits.
 */
void* threadfunc(void* thread_param);

/**
 * Starts a thread that:
 * - Waits `wait_to_obtain_ms` milliseconds before locking the mutex.
 * - Locks the `mutex` passed in.
 * - Holds the lock for `wait_to_release_ms` milliseconds.
 * - Unlocks the mutex and exits.
 * 
 * The `start_thread_obtaining_mutex` function dynamically allocates the `thread_data`
 * structure for the thread. This structure is used to communicate between the thread
 * and the caller. The thread returns a pointer to the structure, which the caller
 * must free.
 *
 * @param thread Pointer to the thread ID created by pthread_create.
 * @param mutex Pointer to the mutex to be used by the thread.
 * @param wait_to_obtain_ms Time to wait before locking the mutex (in milliseconds).
 * @param wait_to_release_ms Time to hold the mutex before unlocking it (in milliseconds).
 * 
 * @return true if the thread was successfully created, false otherwise.
 */
bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms);

#endif // THREADING_H
