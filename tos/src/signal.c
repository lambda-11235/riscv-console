
#include <tos/signal.h>

#include "syscall.h"


int signal_register(int sig, void (*handler)(int)) {
    uint32_t args[5];
    args[0] = (uint32_t) sig;
    args[1] = (uint32_t) handler;
    return syscall(512, args);
}


int signal_raise(int sig) {
    uint32_t args[5];
    args[0] = (uint32_t) sig;
    return syscall(513, args);
}
