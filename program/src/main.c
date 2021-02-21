
#include <stdint.h>
#include <stddef.h>

#include <tos/stdlib.h>


const int ret = 17;

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
volatile uint32_t* ctrlr0 = (uint32_t*) 0x40000018;
char* msg = "Press any button (except command) to return ";


// FIXME: move common code to both kernel and application somewhere else.
int u32_to_str(char* buf, uint32_t x) {
    size_t i = 0;
    size_t j = 0;
    char tmp;

    if (x == 0) {
        buf[0] = '0';
        return 0;
    }

    for (i = 0; x > 0; i++) {
        buf[i] = '0' + x%10;
        x /= 10;
    }

    i--;

    while (j < i) {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;

        j++;
        i--;
    }

    return 0;
}


int i32_to_str(char* buf, int32_t x) {
    if (x < 0) {
        buf[0] = '-';
        buf++;
        x = -x;
    }

    return u32_to_str(buf, x);
}

int main() {
    int i, j;
    uint64_t t;
    char buf[1024];

    for (i = 0; msg[i] != 0; i++)
        VIDEO_MEMORY[i] = msg[i];

    while(!(*ctrlr0)) {
        time_us(&t);
        u32_to_str(buf, ((uint32_t) t)/1000000);

        for (j = 0; buf[j] != 0; j++)
            VIDEO_MEMORY[i+j] = buf[j];
    };
    
    return t;
}
