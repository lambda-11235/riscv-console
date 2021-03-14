/**
 * This program generates assembly code to manage contexts.
 * Specifically it generates the follow macro for GAS.
 *
 * swap_ctx         old_sym new_sym
 *
 * Swap to context given by symbol new_sym. Current context is stored
 * in old_sym. This clobbers mcratch.
 *
 * Build
 * gcc ctx_code_gen.c -o ctx_code_gen; ./ctx_code_gen > swap_ctx.h
 */


#include <stdio.h>
#include <stddef.h>

#include "context.h"


#define OFF(x) (offsetof(struct context, x))
#define STORE(x, loc) (printf("\tsw " #x ", %d(" #loc ")\n", OFF(x)))
#define LOAD(x, loc) (printf("\tlw " #x ", %d(" #loc ")\n", OFF(x)))

#define LA_NORELAX(x, addr) { \
    printf("\t.option push\n"); \
    printf("\t.option norelax\n"); \
    printf("\tla " x ", " addr "\n"); \
    printf("\t.option pop\n"); \
    }


void body(void);

int main(int argc, char *argv[])
{

    printf(".macro swap_ctx old_sym, new_sym\n");
    body();
    printf(".endm\n");

    return 0;
}


void body(void) {
    // First save clobbered temporary.
    printf("\tcsrw mscratch, t0\n\n");

    LA_NORELAX("t0", "\\old_sym");

    printf("\n");

    STORE(ra, t0);
    STORE(sp, t0);
    STORE(gp, t0);
    STORE(tp, t0);
    //STORE(t0, t0);  // Skip because t0 holds the address
    STORE(t1, t0);
    STORE(t2, t0);
    STORE(s0, t0);
    STORE(s1, t0);
    STORE(a0, t0);
    STORE(a1, t0);
    STORE(a2, t0);
    STORE(a3, t0);
    STORE(a4, t0);
    STORE(a5, t0);
    printf("\n");

    // Save mepc
    printf("\tcsrr t1, mepc\n");
    printf("\tsw t1, %d(t0)\n", OFF(mepc));
    printf("\n");

    // Save t0 (stored in mscratch) now
    printf("\tcsrr t1, mscratch\n");
    printf("\tsw t1, %d(t0)\n", OFF(t0));


    printf("\n\n");

    LA_NORELAX("t0", "\\new_sym");
    printf("\n");

    // Load mepc
    printf("\tlw t1, %d(t0)\n", OFF(mepc));
    printf("\tcsrw mepc, t1\n");
    printf("\n");


    LOAD(ra, t0);
    LOAD(sp, t0);
    LOAD(gp, t0);
    LOAD(tp, t0);
    LOAD(t1, t0);
    LOAD(t2, t0);
    LOAD(s0, t0);
    LOAD(s1, t0);
    LOAD(a0, t0);
    LOAD(a1, t0);
    LOAD(a2, t0);
    LOAD(a3, t0);
    LOAD(a4, t0);
    LOAD(a5, t0);

    // Do last so that we don't lose the address.
    LOAD(t0, t0);
}
