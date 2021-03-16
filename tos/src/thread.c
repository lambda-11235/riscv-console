
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


int thread_sleep_us(uint64_t* period) {
    uint32_t args[5];
    args[0] = (uint32_t) period;
    return syscall(389, args);
}




struct mutex_t* mutex_new(void) {
    uint32_t args[5];
    return (struct mutex_t*) syscall(404, args);
}


int mutex_lock(struct mutex_t* m) {
    uint32_t args[5];
    args[0] = (uint32_t) m;
    return syscall(405, args);
}


int mutex_unlock(struct mutex_t* m) {
    uint32_t args[5];
    args[0] = (uint32_t) m;
    return syscall(406, args);
}

int mutex_free(struct mutex_t* m) {
    uint32_t args[5];
    args[0] = (uint32_t) m;
    return syscall(407, args);
}




struct condvar_t* condvar_new(void) {
    uint32_t args[5];
    return (struct condvar_t*) syscall(424, args);
}


int condvar_wait(struct condvar_t* cv, struct mutex_t* m) {
    uint32_t args[5];
    args[0] = (uint32_t) cv;
    args[1] = (uint32_t) m;
    return syscall(425, args);
}


int condvar_signal(struct condvar_t* cv) {
    uint32_t args[5];
    args[0] = (uint32_t) cv;
    return syscall(426, args);
}


int condvar_broadcast(struct condvar_t* cv) {
    uint32_t args[5];
    args[0] = (uint32_t) cv;
    return syscall(427, args);
}


int condvar_free(struct condvar_t* cv) {
    uint32_t args[5];
    args[0] = (uint32_t) cv;
    return syscall(428, args);
}
