
#include <stddef.h>
#include <stdint.h>

#ifndef _UTIL_H_
#define _UTIL_H_

/**
 * Compares two strings for equality.
 *
 * @return 1 if equal, 0 otherwise.
 */
int streq(const char* str1, const char* str2);

/**
 * Converts a uint32_t to a string.
 *
 * @param buf buffer to store the string in
 * @param x integer to convert
 *
 * @return 0 on success, -1 on failure.
 */
int u32_to_str(char* buf, uint32_t x);

/**
 * Converts a int32_t to a string.
 *
 * @param buf buffer to store the string in
 * @param x integer to convert
 *
 * @return 0 on success, -1 on failure.
 */
int i32_to_str(char* buf, int32_t x);

#endif /* _UTIL_H_ */
