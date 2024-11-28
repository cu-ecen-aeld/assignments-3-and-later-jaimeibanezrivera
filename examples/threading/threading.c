#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n", ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n", ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;

    if (!thread_func_args) {
        ERROR_LOG("thread_param is NULL\n");
        return NULL;
    }

    // Wait before attempting to lock the mutex
    usleep(thread_func_args->wait_to_obtain_ms * 1000);

    // Attempt to lock the mutex
    if (pthread_mutex_lock(thread_func_args->mutex) != 0) {
        ERROR_LOG("Failed to lock mutex\n");
        thread_func_args->thread_complete_success = false;
        return thread_func_args;
    }
    DEBUG_LOG("Mutex locked by thread\n");

    // Hold the mutex for the specified time
    usleep(thread_func_args->wait_to_release_ms * 1000);

    // Unlock the mutex
    if (pthread_mutex_unlock(thread_func_args->mutex) != 0) {
        ERROR_LOG("Failed to unlock mutex\n");
        thread_func_args->thread_complete_success = false;
        return thread_func_args;
    }
    DEBUG_LOG("Mutex unlocked by thread\n");

    // Mark thread as successfully completed
    thread_func_args->thread_complete_success = true;
    return thread_func_args;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
    if (!thread || !mutex) {
        ERROR_LOG("Invalid thread or mutex pointer\n");
        return false;
    }

    // Dynamically allocate memory for thread_data
    struct thread_data *data = (struct thread_data *)malloc(sizeof(struct thread_data));
    if (!data) {
        ERROR_LOG("Failed to allocate memory for thread_data\n");
        return false;
    }

    // Initialize thread_data
    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;
    data->thread_complete_success = false;

    // Create the thread
    if (pthread_create(thread, NULL, threadfunc, (void *)data) != 0) {
        ERROR_LOG("Failed to create thread\n");
        free(data); // Free allocated memory in case of failure
        return false;
    }

    DEBUG_LOG("Thread created successfully\n");
    return true;
}
