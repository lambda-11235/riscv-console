
#include <stddef.h>

#include "registers.h"

#include "signal.h"

#define MAX_SIGNALS 256

typedef void (*handler_t)(int);
handler_t handlers[MAX_SIGNALS];


void signal_init(void) {
    for (size_t i = 0; i < MAX_SIGNALS; i++) {
        handlers[i] = NULL;
    }
}


int signal_register(int sig, void (*handler)(int)) {
    handlers[sig] = handler;
}


void signal_raise_user(int sig, handler_t hand);

int signal_raise(int sig) {
    if (handlers[sig] == NULL) {
        // Ignore
    } else {
        // User handler
        signal_raise_user(sig, handlers[sig]);
    }

    return 0;
}
