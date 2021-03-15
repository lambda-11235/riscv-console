#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "alloc.h"
#include "context.h"
#include "fault.h"
#include "input.h"
#include "registers.h"
#include "signal.h"
#include "time.h"
#include "thread.h"
#include "video.h"


#define OS_VERSION 3


int run_cartridge(void);
volatile int start_program = 0;
volatile int program_running = 0;

volatile uint32_t c_mcause;

/* Current running context when not servicing an interrupt.
 * Is saved on entering the interrupt handler, and restored after.
 * Context switching is done by reassigning it.
 */
volatile struct context current_ctx;

// Contains stack and global pointer for interrupt handler.
volatile struct context interrupt_ctx;


int main() {
    char buf[256];
    uint32_t ret;

    mem_init();
    time_init();
    thread_init();

    video_write_text(0, 0, "Please insert cartridge");

    while(1) {
        if (start_program) {
            start_program = 0;
            ret = run_cartridge();
            // TODO: Cleanup

            video_set_mode(TEXT_MODE);
            video_clear_text();
            video_write_text(0, 0, "Program exited with");

            sprintf(buf, "%lX", ret);
            video_write_text(4, 1, buf);
        }
    }

    while(1) {}

    return 0;
}


void c_interrupt_handler(void){
    switch (c_mcause) {
    case 0x80000007: // Timer Interrupt
        time_on_timeout();
        signal_raise(1);
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

        if (CARTRIDGE & 0x1)
            start_program = 1;

        signal_raise(2);

        // Clear interrupt
        INTERRUPT_PENDING &= 1;
    }

    if (INTERRUPT_PENDING & 2) {
        // Video interrupt
        signal_raise(3);

        // Clear interrupt
        INTERRUPT_PENDING &= 2;
    }

    if (INTERRUPT_PENDING & 4) {
        // CMD button press
        signal_raise(4);

        // Clear interrupt
        INTERRUPT_PENDING &= 4;
    }
}


void c_syscall_handler(void) {
    uint32_t a0 = current_ctx.a0;
    uint32_t a1 = current_ctx.a1;
    uint32_t a2 = current_ctx.a2;
    uint32_t a3 = current_ctx.a3;
    //uint32_t a4 = current_ctx.a4;
    //uint32_t a5 = current_ctx.a5;
    uint32_t ret = 0;
    char buf[256];

    switch (a0) {
    case 0:
        ret = OS_VERSION;
        break;
    case 1:
        fault((const char*) a1);
        break;
    case 2:
        ret = (uint32_t) mem_alloc((size_t) a1);
        break;
    case 3:
        mem_free((void*) a1);
        break;
    case 4:
        ret = (uint32_t) mem_realloc((void*) a1, (size_t) a2);
        break;
    case 5:
        ret = time_us((uint64_t*) a1);
        break;
    case 6:
        ret = sleep_us((uint64_t*) a1);
        break;
    case 7:
        ret = set_timeout_us((uint64_t*) a1);
        break;

    // Input
    case 128:
        ret = input_ctlr_poll((struct input_ctlr*) a1);
        break;

    // Video
    case 256:
        ret = video_set_mode((enum video_mode) a1);
        break;
    case 257:
        ret = video_clear_text();
        break;
    case 258:
        ret = video_write_text(a1, a2, (char*) a3);
        break;
    case 259:
        ret = video_write_bg_palette_data(a1, (uint32_t*) a2);
        break;
    case 260:
        ret = video_write_sprite_palette_data(a1, (uint32_t*) a2);
        break;
    case 261:
        ret = video_write_bg_data(a1, (uint8_t*) a2);
        break;
    case 262:
        ret = video_write_bg_control(a1, (struct bg_control*) a2);
        break;
    case 263:
        ret = video_write_ls_data(a1, (uint8_t*) a2);
        break;
    case 264:
        ret = video_write_ls_control(a1, (struct ls_control*) a2);
        break;
    case 265:
        ret = video_write_ss_data(a1, (uint8_t*) a2);
        break;
    case 266:
        ret = video_write_ss_control(a1, (struct ss_control*) a2);
        break;

    case 384:
        ret = thread_create((int (*)(void*)) a1, (void*) a2);
        break;
    case 385:
        ret = thread_yield();
        break;
    case 386:
        ret = thread_join(a1, (int*) a2);
        break;
    case 387:
        thread_exit(a1);
        break;
    case 388:
        ret = thread_set_preemption(a1);
        break;

    // Signals
    case 512:
        ret = signal_register(a1, (void (*)(int)) a2);
        break;
    case 513:
        ret = signal_raise(a1);
        break;

    default:
        sprintf(buf, "Got unknown syscall %lu", a0);
        fault(buf);
    }

    current_ctx.a0 = ret;
    current_ctx.mepc += 4;
}
