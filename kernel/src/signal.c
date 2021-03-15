
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
    return 0;
}


int signal_raise(int sig) {
    if (handlers[sig] == NULL) {
        // Ignore
    } else {
        // TODO: Old code breaks context switching.
        //       Should spawn thread to handle signal instead.
        //       Set parrent thread to waiting.
    }

    return 0;
}
