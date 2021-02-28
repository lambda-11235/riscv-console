
#include <tos/input.h>

#include "syscall.h"


int input_ctlr_poll(struct input_ctlr* ev) {
    uint32_t args[5];
    args[0] = (uint32_t) ev;
    return syscall(128, args);
}
