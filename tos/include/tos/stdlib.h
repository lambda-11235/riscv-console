
#include <stdint.h>
#include <stddef.h>

#ifndef _TOS_STDLIB_H_
#define _TOS_STDLIB_H_

/**
 * @brief Gets the OS version.
 *
 * Version are gauranteed to be monotonically increasing with time.
 */
uint32_t os_version(void);

/**
 * @brief Causes an irrecoverable kernel panic.
 *
 * @param msg A message indicating what caused the fault.
 *
 * @return None, returning indicates an error.
 */
void fault(const char* msg);


/**
 * @brief allocates memory from the heap.
 *
 * @param size The amount of memory (in bytes) to allocate.
 * @return A pointer to the allocated memory on success, NULL on failure.
 */
void* mem_alloc(size_t size);


/**
 * @brief Frees memory previously allocated by mem\_alloc.
 *
 * @param ptr A pointer to memory allocated by a mem\_* function.
 */
void mem_free(void* ptr);


/**
 * @brief Reallocates memory on the heap.
 *
 * @param old_ptr Pointer to old memory location. mem\_realloc frees this pointer.
 * @param new_size The amount of memory (in bytes) of the new allocation.
 *
 * @return A pointer to the reallocated memory on success, NULL on failure.
 */
void* mem_realloc(void* old_ptr, size_t new_size);

/**
 * @brief Gets the time in microseconds from some arbitrary starting
 * time.
 *
 * Note that the underlying architecture may not support microsecond
 * precision, in which case time will increment by the most precise
 * unit supported.
 *
 * @param t The variable to store the time in.
 *
 * @return The 0 on success, -1 on error.
 */
int time_us(uint64_t* t);


/**
 * @brief Sets the timeout in microseconds between timer interrupts.
 *
 * Note that the underlying architecture may not support microsecond
 * precision, in which case time will increment by the most precise
 * unit supported.
 *
 * @param period The period between timer interrupts in
 * microseconds. A value of 0 disables timer interrupts.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int set_timeout_us(uint64_t* period);


#endif /* _TOS_STDLIB_H_ */
