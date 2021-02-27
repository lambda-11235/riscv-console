
#include "input.h"

volatile const uint32_t* CONTROLLER_STATUS = (uint32_t*) 0x4000001C;

int input_ctlr_poll(struct input_ctlr* ev) {
    uint32_t status = *CONTROLLER_STATUS;

    ev->button1_pressed = status & 16;
    ev->button2_pressed = status & 32;
    ev->button3_pressed = status & 64;
    ev->button4_pressed = status & 128;
    
    ev->left = status & 1;
    ev->right = status & 2;
    ev->up = status & 4;
    ev->down = status & 8;
}
