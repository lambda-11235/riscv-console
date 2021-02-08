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


volatile uint32_t a07_regs[6];
void handle_syscall(void);


volatile int cmd_pressed = 0;


int main() {
    uint32_t ret;
    int i, j;
    int mode_id;
    int pal_id;
    int controls_id;
    int data_id;
    int ctrlr_id;
    volatile uint32_t* mode;
    volatile uint32_t* palette;
    volatile uint32_t* controls;
    volatile uint8_t* data;
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

    data_id = open("/sys/dev/video/graphic/background0/data", READ|WRITE);
    if (data_id == -1) fault("Could not open data");
    data = memmap(data_id, 0);
    if (data == NULL) fault("Could not memmap data");

    ctrlr_id = open("/sys/dev/input/controller/ctrlr0", READ|WRITE);
    if (ctrlr_id == -1) fault("Could not open ctrlr0");
    ctrlr = memmap(ctrlr_id, 0);
    if (ctrlr == NULL) fault("Could not memmap ctrlr0");

    *mode = 1;

    palette[0] = 0xFFFFFFFF;
    palette[1] = 0xFF0000FF;
    palette[2] = 0x00FF00FF;
    palette[3] = 0x0000FFFF;

    *controls = (288<<12) | (512 << 2);

    int pal_sel = 0;
    while (1) {
        if ((*ctrlr) & 1)
            pal_sel = 0;
        else if ((*ctrlr) & 2)
            pal_sel = 1;
        else if ((*ctrlr) & 4)
            pal_sel = 2;
        else if ((*ctrlr) & 8)
            pal_sel = 3;
        
        for (i = 0; i < 147456; i++)
            data[i] = pal_sel;
    }

    close(mode_id);
    close(pal_id);
    close(controls_id);
    close(data_id);

    while(1) {}

    return 0;
}


int run_cartridge(void) {
    uint32_t gp_save;
    int ret;
    
    program_running = 1;
    asm volatile ("mv %0, gp" : "=r"(gp_save));

    // NOTE: This should be fine since CARTRIDGE is loaded from data
    // before the call, but if any thing goes wrong check here.
    ret = CARTRIDGE();

    asm volatile ("mv gp, %0" : : "r"(gp_save));
    program_running = 0;

    return ret;
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
        // Unrecognized interrupt
        break;
    }

    if (ip & 1) {
        if (program_running) {
            // TODO: cleanup
            fault("Cartridge removed, cleanup not implemented");
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
        cmd_pressed = 1;

        // Clear interrupt
        INTERRUPT_PENDING &= 4;
    }
}

void handle_syscall(void) {
    switch (a07_regs[0]) {
    case 0: // exit
        // TODO
        break;
    }
}
