#include <stdint.h>

#include "registers.h"

extern uint8_t _erodata[];
extern uint8_t _data[];
extern uint8_t _edata[];
extern uint8_t _sdata[];
extern uint8_t _esdata[];
extern uint8_t _bss[];
extern uint8_t _ebss[];

// Adapted from https://stackoverflow.com/questions/58947716/how-to-interact-with-risc-v-csrs-by-using-gcc-c-code

void init(void){
    uint8_t *Source = _erodata;
    uint8_t *Base = _data < _sdata ? _data : _sdata;
    uint8_t *End = _edata > _esdata ? _edata : _esdata;

    // Copy static data to RAM
    while(Base < End){
        *Base++ = *Source++;
    }

    // Zero out uninitialized data
    Base = _bss;
    End = _ebss;
    while(Base < End){
        *Base++ = 0;
    }

    csr_write_mie(0x888);       // Enable all interrupt soruces
    csr_enable_interrupts();    // Global interrupt enable

    // FIXME: Video interrupts freeze program due to too high of freq
    INTERRUPT_ENABLE = 1|2|4;

    MTIMECMP_LOW = 1;
    MTIMECMP_HIGH = 0;
}
