
#include <stdint.h>
#include <string.h>

#include "alloc.h"



enum mem_flags {
    USED = 1
};

struct block_header {
    size_t len;
    uint32_t flags;
};

#define HEAP_START ((struct block_header*) 0x70800000)
#define HEAP_END   ((struct block_header*) 0x71000000)

struct block_header* head;


struct block_header* next_block(struct block_header* block) {
    void* ptr = (void*) block;
    ptr += block->len;

    if (ptr >= (void*) HEAP_END)
        return NULL;
    else
        return (struct block_header*) ptr;
}


// Combine current unused block with consecutive unused blocks.
void combine_blocks(struct block_header* block) {
    struct block_header* next = next_block(block);

    if (block->flags & USED)
        return;

    while (next != NULL && !(next->flags & USED)) {
        block->len += next->len + sizeof(struct block_header);
        next = next_block(block);
    }
}


void* to_ptr(struct block_header* block) {
    return ((void*) block) + sizeof(struct block_header);
}

struct block_header* from_ptr(void* ptr) {
    return (struct block_header*) (ptr - sizeof(struct block_header));
}


void mem_init(void) {
    head = HEAP_START;
    head->len = HEAP_END - HEAP_START - sizeof(struct block_header);
    head->flags = 0;
}


void* mem_alloc(size_t size) {
    size_t old_len;
    struct block_header* block = head;
    struct block_header* tmp;

    // Readjust to be multiple of header length
    size = sizeof(struct block_header)*(1 + size/sizeof(struct block_header));

    // Reap unused blocks
    combine_blocks(block);

    // Search for large enough block
    while (block->flags & USED || block->len < size) {
        block = next_block(block);

        if (block == NULL)
            return NULL;

        combine_blocks(block);
    }

    // Couldn't find large enough block
    if (block->flags & USED || block >= HEAP_END)
        return NULL;

    // Split block in to two parts
    old_len = block->len;
    block->len = size;
    block->flags |= USED;

    if (size < old_len) {
        tmp = next_block(block);
        tmp->len = old_len - size - sizeof(struct block_header);
        tmp->flags = 0;
    }

    return to_ptr(block);
}


void mem_free(void* ptr) {
    from_ptr(ptr)->flags &= !USED;
}


void* mem_realloc(void* old_ptr, size_t new_size) {
    // TODO: Make this more efficient for some cases.
    void* new_ptr = mem_alloc(new_size);
    memcpy(new_ptr, old_ptr, new_size);
    mem_free(old_ptr);
    return new_ptr;
}
