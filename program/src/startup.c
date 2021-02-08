#include <stdint.h>

extern uint8_t _erodata[];
extern uint8_t _data[];
extern uint8_t _edata[];
extern uint8_t _sdata[];
extern uint8_t _esdata[];
extern uint8_t _bss[];
extern uint8_t _ebss[];

// Adapted from https://stackoverflow.com/questions/58947716/how-to-interact-with-risc-v-csrs-by-using-gcc-c-code

void init(void){
    uint8_t *source = _erodata;
    uint8_t *base = _data < _sdata ? _data : _sdata;
    uint8_t *end = _edata > _esdata ? _edata : _esdata;

    // Copy static data to RAM
    while(base < end){
        *base++ = *source++;
    }

    // Zero out uninitialized data
    base = _bss;
    end = _ebss;
    while(base < end){
        *base++ = 0;
    }
}
