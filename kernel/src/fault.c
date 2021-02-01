#include <stddef.h>
#include <stdint.h>

#include "fault.h"

#define TEXT_HEIGHT 36
#define TEXT_WIDTH 64

void fault(const char* msg) {
    uint32_t* MODE_MEMORY = 0x500FF414;
    char* TEXT_MEMORY = 0x500FE800;
    size_t i;

    *MODE_MEMORY = 0;

    for (i = 0; i < TEXT_HEIGHT*TEXT_WIDTH; i++)
        TEXT_MEMORY[i] = 0;

    for (i = 0; msg[i] != 0; i++)
        TEXT_MEMORY[i] = msg[i];

    fault_imm();
}

void fault_imm(void) {
    while(1) {}
}
