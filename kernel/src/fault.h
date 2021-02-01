
#ifndef _FAULT_H_
#define _FAULT_H_

/**
 * Causes entire system to crash due to an error.
 *
 * @param msg a message to print before crashing.
 */
void fault(const char* msg);

/**
 * Causes entire system to crash due to an error.
 * The version exits immediately, and should only be used in extreme
 * cases where permenant system damage may occur from printing to
 * screen.
 */
void fault_imm(void);

#endif /* _FAULT_H_ */
