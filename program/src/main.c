
#include <stdint.h>

const int ret = 17;

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
volatile uint32_t* ctrlr0 = (uint32_t*) 0x40000018;
char* msg = "Press any button (except command) to return 17";

int main() {
    for (int i = 0; msg[i] != 0; i++)
        VIDEO_MEMORY[i] = msg[i];

    while(!(*ctrlr0)) {};
    
    return ret;
}
