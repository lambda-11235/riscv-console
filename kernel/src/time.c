
#include "time.h"

#include "registers.h"

#define USECS_PER_TICK 1000
volatile uint64_t start_time;


uint64_t get_time(void) {
    return (((uint64_t)MTIME_HIGH)<<32) | MTIME_LOW;
}


int time_init(void) {
    start_time = get_time();
    return 0;
}


int time_us(uint64_t* t) {
    *t = USECS_PER_TICK*(get_time() - start_time);
    return 0;
}
