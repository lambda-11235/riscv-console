
#include<tos/thread.h>

#include "context.h"

#ifndef _THREAD_H_
#define _THREAD_H_


/**
 * Initializes threading library.
 * Mainly this just creates a thread for the kernel process.
 */
void thread_init(void);

/**
 * Should be called at the start of interrupt handler.
 */
void thread_enter_int(void);

/**
 * Should be called at the start of interrupt handler.
 */
void thread_exit_int(void);


#endif /* _THREAD_H_ */
