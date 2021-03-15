
#ifndef _TOS_THREAD_H_
#define _TOS_THREAD_H_


typedef int thread_t;
struct mutex_t;
struct condvar_t;


/**
 * @brief Creates and starts a new thread.
 *
 * @param func The entry point to the thread.
 * @param data Data to pass to the entry point as an argument.
 *
 * @return On success returns the ID to the thread, and on failure returns -1.
 */
thread_t thread_create(int (*func)(void*), void* data);


/**
 * @brief Yields execution to the next in line thread.
 *
 * @return On success returns 0, and on failure -1.
 */
int thread_yield(void);


/**
 * @brief Suspends execution until another thread exits.
 *
 * @param t The thread to join with.
 * @param exit_code The exit code of the joined thread.
 *
 * @return On success returns 0, and on failure -1.
 */
int thread_join(thread_t t, int* exit_code);


/**
 * @brief Causes thread to quit running and yield.
 * Note that this does not cleanup allocations made by the thread.
 *
 * @param exit_code The exit code.
 */
void thread_exit(int exit_code);


/**
 * @brief Changes whether threads are preempted.
 *
 * @param enable If true enable preemption, otherwise disable it.
 *
 * @return On success returns 0, and on failure -1.
 */
int thread_set_preemption(int enable);


/**
 * @brief Creates a new mutex for use.
 *
 * @return On success returns a pointer to the mutex, and on failure
 * NULL.
 */
struct mutex_t* mutex_new(void);


/**
 * @brief Locks a mutex.
 *
 * @param m The mutex to lock.
 *
 * @return On success returns 0, and on failure -1.
 */
int mutex_lock(struct mutex_t* m);


/**
 * @brief Unlocks a mutex.
 *
 * @param m The mutex to unlock.
 *
 * @return On success returns 0, and on failure -1.
 */
int mutex_unlock(struct mutex_t* m);


/**
 * @brief Frees resources used by a mutex.
 *
 * @param m The mutex to free.
 *
 * @return On success returns 0, and on failure -1.
 */
int mutex_free(struct mutex_t* m);


/**
 * @brief Creates a new condition variable for use.
 *
 * @return On success returns a pointer to the variable, and on
 * failure NULL.
 */
struct condvar_t* condvar_new(void);


/**
 * @brief Wait for a condition variable.
 *
 * @param cv The condition variable to wait for.
 * @param m Mutex to unlock while waiting.
 *
 * @return On success returns 0, and on failure -1.
 */
int condvar_wait(struct condvar_t* cv, struct mutex_t* m);


/**
 * @brief Wakes up one of the threads waiting on a condition variable.
 *
 * @param cv The condition variable signaling.
 *
 * @return On success returns 0, and on failure -1.
 */
int condvar_signal(struct condvar_t* cv);


/**
 * @brief Wakes up all of the threads waiting on a condition variable.
 *
 * @param cv The condition variable signaling.
 *
 * @return On success returns 0, and on failure -1.
 */
int condvar_broadcast(struct condvar_t* cv);


/**
 * @brief Frees a condition variable from use.
 *
 * @param cv The condition variable to destroy.
 *
 * @return On success returns 0, and on failure -1.
 */
int condvar_free(struct condvar_t* cv);


#endif /* _TOS_THREAD_H_ */
