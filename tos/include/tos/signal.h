
#ifndef _TOS_SIGNAL_H_
#define _TOS_SIGNAL_H_


/**
 * @brief Registers an action to take when a signal is raised.
 *
 * @param sig The signal to attach the handler to.
 *
 * @param handler A function to call when a signal is raised.  Note
 * that this function will replace any handler previously set by
 * signal\_register. The int argument identifies the raised signal.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int signal_register(int sig, void (*handler)(int));


/**
 * @brief Raises a signal.
 *
 * @param sig The signal to raise.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int signal_raise(int sig);


#endif /* _TOS_SIGNAL_H_ */
