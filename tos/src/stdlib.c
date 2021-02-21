
#include "syscall.h"

#include "tos/stdlib.h"


void fault(const char* msg) {
    uint32_t args[5];
    args[0] = (uint32_t) msg;
    syscall(0, args);
}

int time_us(uint64_t* t) {
    uint32_t args[5];
    args[0] = (uint32_t) t;
    return syscall(4, args);
}
