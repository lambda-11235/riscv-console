
#include "syscall.h"

#include "tos/stdlib.h"


uint32_t os_version(void) {
    uint32_t args[5];
    return syscall(0, args);
}


void fault(const char* msg) {
    uint32_t args[5];
    args[0] = (uint32_t) msg;
    syscall(1, args);
}


void* mem_alloc(size_t size) {
    uint32_t args[5];
    args[0] = (uint32_t) size;
    return (void*) syscall(2, args);
}


void mem_free(void* ptr) {
    uint32_t args[5];
    args[0] = (uint32_t) ptr;
    syscall(3, args);
}


void* mem_realloc(void* old_ptr, size_t new_size) {
    uint32_t args[5];
    args[0] = (uint32_t) old_ptr;
    args[1] = (uint32_t) new_size;
    return (void*) syscall(4, args);
}


int time_us(uint64_t* t) {
    uint32_t args[5];
    args[0] = (uint32_t) t;
    return syscall(5, args);
}


int sleep_us(uint64_t* period) {
    uint32_t args[5];
    args[0] = (uint32_t) period;
    return syscall(6, args);
}


int set_timeout_us(uint64_t* period) {
    uint32_t args[5];
    args[0] = (uint32_t) period;
    return syscall(7, args);
}
