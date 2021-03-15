
#include "time.h"

#include "registers.h"

#define USECS_PER_TICK 1000
#define MIN_TIMEOUT_TICKS 1
#define DEFAULT_TIMEOUT_TICKS 10

uint64_t start_time;
uint64_t timeout_ticks;
int no_timeout;


inline void get_time(uint64_t* t) {
    *t = (((uint64_t)MTIME_HIGH)<<32) | MTIME_LOW;
}


void time_init(void) {
    get_time(&start_time);
    timeout_ticks = DEFAULT_TIMEOUT_TICKS;
    no_timeout = 0;

    time_on_timeout();
}


void time_on_timeout(void) {
    if (no_timeout) {
        MTIMECMP_HIGH = 0xffffffff;
        MTIMECMP_LOW = 0xffffffff;
    } else if (timeout_ticks < MIN_TIMEOUT_TICKS) {
        fault("timeout_ticks less than MIN_TIMEOUT_TICKS");
    } else {
        uint64_t new_comp;
        get_time(&new_comp);
        new_comp += timeout_ticks;
        MTIMECMP_HIGH = new_comp>>32;
        MTIMECMP_LOW = new_comp;
    }
}


int time_us(uint64_t* t) {
    get_time(t);
    *t -= start_time;
    *t *= USECS_PER_TICK;
    return 0;
}


int set_timeout_us(uint64_t* period) {
    if (*period == 0) {
        no_timeout = 1;
    } else {
        no_timeout = 0;
        timeout_ticks = (*period)/USECS_PER_TICK;

        // Can't have zero timeout
        if (timeout_ticks < MIN_TIMEOUT_TICKS)
            timeout_ticks = MIN_TIMEOUT_TICKS;
    }

    time_on_timeout();

    return 0;
}
