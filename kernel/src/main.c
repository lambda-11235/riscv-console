#include <stdlib.h>
#include <stdint.h>

#include "fault.h"
#include "registers.h"
#include "util.h"
#include "vfs.h"

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
const unsigned int TEXT_HEIGHT = 36;
const unsigned int TEXT_WIDTH = 64;


int run_cartridge(void);
const int (*CARTRIDGE)(void) = (int (*)(void))(0x20000000);
volatile int start_program = 0;
volatile int program_running = 0;

volatile uint32_t c_mcause;
volatile uint32_t c_mip;
volatile uint32_t a05_regs[6];

volatile int cmd_pressed = 0;
volatile int draw = 0;
volatile int pal_sel = 0;

// TODO: Remove after demo
#define TICKS_PER_SEC 1000
volatile int timer = 0;

int main() {
    uint32_t ret;
    int i, j;
    int mode_id;
    int pal_id;
    int controls_id;
    int ctrlr_id;
    volatile uint32_t* mode;
    volatile uint32_t* palette;
    volatile uint32_t* controls;
    volatile uint32_t* ctrlr;


    file_system_init();


    strcpy_(VIDEO_MEMORY, "Please insert cartridge");

    while(1) {
        if (start_program) {
            start_program = 0;
            ret = run_cartridge();
            // TODO: Cleanup

            // TODO: Remove after demo
            for (i = 0; i < TEXT_WIDTH; i++)
                VIDEO_MEMORY[i] = 0;

            cmd_pressed = 0;
            u32_to_str(VIDEO_MEMORY + strcpy_(VIDEO_MEMORY, "Program returned "), ret);
            strcpy_(VIDEO_MEMORY + TEXT_WIDTH, "Press CMD to continue");
            strcpy_(VIDEO_MEMORY + 3*TEXT_WIDTH, "WARNING: Do not continue if you have epilepsy triggered by flashing lights");
            while (!cmd_pressed) {}

            break;
        }
    }

    // This shows how to use the file system to interface with graphics.
    // My idea is that a user level library will memmap to gain access to the pointers,
    // and maintain the mapping for the life of the program.
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


    ctrlr_id = open("/sys/dev/input/controller/ctrlr0", READ|WRITE);
    if (ctrlr_id == -1) fault("Could not open ctrlr0");
    ctrlr = memmap(ctrlr_id, 0);
    if (ctrlr == NULL) fault("Could not memmap ctrlr0");

    *mode = 1;

    palette[0] = 0xFFFFFFFF;
    palette[1] = 0xFFFF0000;
    palette[2] = 0xFF00FF00;
    palette[3] = 0xFF0000FF;

    *controls = (288<<12) | (512 << 2);

    close(mode_id);
    close(pal_id);
    close(controls_id);

    draw = 1;

    while(1) {}

    return 0;
}


int run_cartridge(void) {
    int ret;

    program_running = 1;

    ret = CARTRIDGE();

    // Restore global pointer
    asm(".option norelax\n"
        "la gp, __global_pointer$");

    program_running = 0;

    return ret;
}


void c_interrupt_handler(void){
    uint64_t new_comp;
    uint32_t ret = 0;

    switch (c_mcause) {
    case 0x80000007: // Timer Interrupt
        new_comp = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
        new_comp += TICKS_PER_SEC;
        MTIMECMP_HIGH = new_comp>>32;
        MTIMECMP_LOW = new_comp;

        timer++;
        break;
    case 0xB: // ECALL
        fault("Syscall handled improperly as asynchronous interrupt");
        break;
    default:
        // Unrecognized interrupt
        break;
    }

    if (INTERRUPT_PENDING & 1) {
        if (program_running) {
            // TODO: cleanup
            fault("Cartridge removed, cleanup not implemented");
        }

        start_program = 1;

        // Clear interrupt
        INTERRUPT_PENDING &= 1;
    }

    if (INTERRUPT_PENDING & 2) {
        // TODO: Video interrupt
        if (draw) {
            int data_id;
            volatile uint8_t* data;

            data_id = open("/sys/dev/video/graphic/background0/data", READ|WRITE);
            if (data_id == -1) fault("Could not open data");
            data = memmap(data_id, 0);
            if (data == NULL) fault("Could not memmap data");

            for (int i = 0; i < 147456; i++)
                data[i] = pal_sel;

            close(data_id);

            pal_sel = (pal_sel + 1)%4;
        }

        // Clear interrupt
        INTERRUPT_PENDING &= 2;
    }

    if (INTERRUPT_PENDING & 4) {
        // TODO: CMD button press
        cmd_pressed = 1;

        // Clear interrupt
        INTERRUPT_PENDING &= 4;
    }
}


uint32_t c_syscall_handler(void) {
    uint32_t ret = 0;

    switch (a05_regs[0]) {
    case 0: // fault
        fault((const char*) a05_regs[1]);
        break;
    case 1: // int time_secs(void) TODO: Remove after demo
        ret = timer;
        break;
    }

    return ret;
}
