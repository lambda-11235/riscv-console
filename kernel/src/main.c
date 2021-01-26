#include <stdint.h>

#include "registers.h"

volatile char *CARTRIDGE_MEMORY = (volatile char *)(0x20000000);
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
const unsigned int TEXT_HEIGHT = 36;
const unsigned int TEXT_WIDTH = 64;

volatile uint32_t a07_regs[6];

void handle_syscall(void);

int main() {
    asm("li a0, 0\n"
        "ecall");
    while(1) {}
    return 0;
}

void c_interrupt_handler(void){
    uint64_t new_comp;
    uint32_t mcause = csr_mcause_read();
    uint32_t mip = csr_mip_read();
    uint32_t ip = INTERRUPT_PENDING;

    switch (mcause) {
    case 0x80000007: // Timer Interrupt
        new_comp = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
        new_comp += 100;
        MTIMECMP_HIGH = new_comp>>32;
        MTIMECMP_LOW = new_comp;
        break;
    case 0xB: // ECALL
        csr_mepc_inc();
        handle_syscall();
        break;
    default:
        // Error: Unrecognize interrupt
        break;
    }

    if (ip & 1) {
        VIDEO_MEMORY[0] = 'L';
        // TODO: Load cartridge and run
        uint32_t ret_code = ((int (*)(void)) CARTRIDGE_MEMORY)();
    }

    if (ip & 2) {
        // TODO: Video interrupt
    }

    if (ip & 4) {
        // TODO: CMD button press
    }

    // Reset interrupts
    INTERRUPT_PENDING = 0;
    
    /*if (mcause != 0x80000007) {
        if (mcause == 0)
            VIDEO_MEMORY[TEXT_WIDTH-1] = '0';

        for (int i = 0; mcause > 0; i++) {
            int32_t res = mcause%16;
            char dig;

            if (res < 10)
                dig = '0' + res;
            else
                dig = 'a' + res - 10;
        
            VIDEO_MEMORY[3*TEXT_WIDTH - 1 - i] = dig;
            mcause /= 16;
        }
        }*/
}

void handle_syscall(void) {
    VIDEO_MEMORY[0] = 'S';
    switch (a07_regs[0]) {
    case 0: // exit
        // TODO
        VIDEO_MEMORY[1] = 'E';
        break;
    }
}
