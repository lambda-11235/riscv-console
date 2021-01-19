
#include <tos/stdlib.h>


struct malloc_header {
    size_t size;
    
    // Bit encoded flags
    // 0x1 = Allocated
    uint8_t flags;
};


extern struct malloc_header _heap_start;

// NULL indicates unintialized
struct malloc_header* root = NULL;


extern void* malloc(size_t size) {
    if (root == NULL) {
        root = &_heap_start;
        root->flags = 0x1;
        root->size = size;

        return root;
    } else {
        return NULL;
    }
}

extern int free(void* ptr) {
    return -1;
}
