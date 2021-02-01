#include <stdlib.h>
#include <stdint.h>

#include "fault.h"
#include "registers.h"
#include "util.h"
#include "vfs.h"

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
const unsigned int TEXT_HEIGHT = 36;
const unsigned int TEXT_WIDTH = 64;

const int (*CARTRIDGE)(void) = (int (*)(void))(0x20000000);
volatile int start_program = 0;
volatile int program_running = 0;

volatile uint32_t a07_regs[6];

void handle_syscall(void);


int main() {
    uint32_t ret;
    int i, j;
    int mode_id;
    int pal_id;
    int controls_id;
    int data_id;
    volatile uint32_t* mode;
    volatile uint32_t* palette;
    volatile uint32_t* controls;
    volatile uint8_t* data;


    // This shows how to use the file system to interface with graphics.
    // My idea is that a user level library will memmap to gain access to the pointers,
    // and maintain the mapping for the life of the program.
    file_system_init();

    mode_id = open("/sys/dev/video/mode", READ|WRITE);
    if (mode_id == -1) fault("Could not open mode");
    mode = memmap(mode_id, 0);
    if (mode == NULL) fault("Could not memmap mode");

    pal_id = open("/sys/dev/video/graphic/palette/background0", READ|WRITE);
    if (pal_id == -1) fault("Could not open palette");
    palette = memmap(pal_id, 0);
    if (palette == NULL) fault("Could not memmap palette");

    controls_id = open("/sys/dev/video/graphic/background0/control", READ|WRITE);
    if (controls_id == -1) fault("Could not open controls");
    controls = memmap(controls_id, 0);
    if (controls == NULL) fault("Could not memmap controls");

    data_id = open("/sys/dev/video/graphic/background0/data", READ|WRITE);
    if (data_id == -1) fault("Could not open data");
    data = memmap(data_id, 0);
    if (data == NULL) fault("Could not memmap data");

    *mode = 1;

    for (i = 0; i < 256; i++)
        palette[i] = 0x0000FFFF;

    *controls = (288<<12) | (512 << 2);

    for (i = 0; i < 147456; i++)
        data[i] = 0;

    close(mode_id);
    close(pal_id);
    close(controls_id);
    close(data_id);

    while(1) {}


    while(1) {
        if (start_program) {
            start_program = 0;
            program_running = 1;
            ret = CARTRIDGE();

            for (i = 0; i < TEXT_WIDTH; i++)
                VIDEO_MEMORY[TEXT_WIDTH+i] = 0;

            u32_to_str(VIDEO_MEMORY, ret);

            program_running = 0;
        }
    }

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
        if (program_running) {
            // TODO: cleanup
        }

        start_program = 1;

        // Clear interrupt
        INTERRUPT_PENDING &= 1;
    }

    if (ip & 2) {
        // TODO: Video interrupt

        // Clear interrupt
        INTERRUPT_PENDING &= 2;
    }

    if (ip & 4) {
        // TODO: CMD button press

        // Clear interrupt
        INTERRUPT_PENDING &= 4;
    }

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
    switch (a07_regs[0]) {
    case 0: // exit
        // TODO
        break;
    }
}
