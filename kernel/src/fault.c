#include <stddef.h>
#include <stdint.h>

#include "fault.h"

#define TEXT_HEIGHT 36
#define TEXT_WIDTH 64

void fault(const char* msg) {
    uint32_t* MODE_MEMORY = (uint32_t*) 0x500FF414;
    char* TEXT_MEMORY = (char*) 0x500FE800;
    char* start = "FAULT ERROR: ";
    size_t i, j;

    *MODE_MEMORY = 0;

    for (i = 0; i < TEXT_HEIGHT*TEXT_WIDTH; i++)
        TEXT_MEMORY[i] = 0;

    for (i = 0; start[i] != 0; i++)
        TEXT_MEMORY[i] = start[i];

    j = i;

    for (i = 0; msg[i] != 0; i++)
        TEXT_MEMORY[i+j] = msg[i];

    fault_imm();
}

void fault_imm(void) {
    while(1) {}
}
