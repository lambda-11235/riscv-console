
#include <stddef.h>

#include "registers.h"
#include "thread.h"

#include "signal.h"


extern struct context current_ctx;


#define MAX_SIGNALS 256

struct handler_entry {
    struct context ctx;
    void (*func)(int);
};

struct handler_entry handlers[MAX_SIGNALS];


void signal_init(void) {
    for (size_t i = 0; i < MAX_SIGNALS; i++) {
        handlers[i].func = NULL;
    }
}


int signal_register(int sig, void (*handler)(int)) {
    handlers[sig].ctx = current_ctx;
    handlers[sig].func = handler;
    return 0;
}


int signal_raise(int sig) {
    if (handlers[sig].func == NULL) {
        // Ignore
    } else {
        thread_create_from_ctx((int (*)(void*)) handlers[sig].func,
                               (void*) sig,
                               handlers[sig].ctx);
    }

    return 0;
}
