
#include "time.h"

#include "registers.h"

#define USECS_PER_TICK 1000
volatile uint64_t start_time;
volatile uint64_t timeout_us;


uint64_t get_time(void) {
    return (((uint64_t)MTIME_HIGH)<<32) | MTIME_LOW;
}


void time_init(void) {
    start_time = get_time();
    timeout_us = 0;
}


void time_on_timeout(void) {
    if (timeout_us > 0) {
        uint64_t new_comp = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
        new_comp += timeout_us;
        MTIMECMP_HIGH = new_comp>>32;
        MTIMECMP_LOW = new_comp;
    } else {
        MTIMECMP_HIGH = 0xffffffff;
        MTIMECMP_LOW = 0xffffffff;
    }
}


int time_us(uint64_t* t) {
    *t = USECS_PER_TICK*(get_time() - start_time);
    return 0;
}


// TODO: Merge with threading
int sleep_us(uint64_t* period) {
    uint64_t start = get_time();

    while (get_time() < start + *period) {}

    return 0;
}


int set_timeout_us(uint64_t* period) {
    timeout_us = *period;
    return 0;
}
