
#include <stdint.h>

#ifndef _CONTEXT_H_
#define _CONTEXT_H_

struct context {
    uint32_t ra, sp, gp, tp;
    uint32_t t0, t1, t2;
    uint32_t s0, s1;
    uint32_t a0, a1, a2, a3, a4, a5;
    uint32_t mepc;
};

#endif /* _CONTEXT_H_ */
