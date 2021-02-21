
#include <stddef.h>

#ifndef _ALLOC_H_
#define _ALLOC_H_


void mem_init(void);

void* mem_alloc(size_t size);
void mem_free(void* ptr);
void* mem_realloc(void* old_ptr, size_t new_size);


#endif /* _ALLOC_H_ */
