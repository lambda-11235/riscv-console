
#include <tos/thread.h>

#include "syscall.h"


thread_t thread_create(int (*func)(void*), void* data) {
    uint32_t args[5];
    args[0] = (uint32_t) func;
    args[1] = (uint32_t) data;
    return syscall(384, args);
}


int thread_yield(void) {
    uint32_t args[5];
    return syscall(385, args);
}


int thread_join(thread_t t, int* exit_code) {
    uint32_t args[5];
    args[0] = (uint32_t) t;
    args[1] = (uint32_t) exit_code;
    return syscall(386, args);
}


void thread_exit(int exit_code) {
    uint32_t args[5];
    args[0] = (uint32_t) exit_code;
    syscall(387, args);
}


int thread_set_preemption(int enable) {
    uint32_t args[5];
    args[0] = (uint32_t) enable;
    return syscall(388, args);
}
