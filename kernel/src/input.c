
#include "input.h"

volatile const uint32_t* CONTROLLER_STATUS = (uint32_t*) 0x40000018;

int input_ctlr_poll(struct input_ctlr* ev) {
    uint32_t status = *CONTROLLER_STATUS;

    ev->button1_pressed = (status >> 4) & 1;
    ev->button2_pressed = (status >> 5) & 1;
    ev->button3_pressed = (status >> 6) & 1;
    ev->button4_pressed = (status >> 7) & 1;

    ev->left = (status >> 0) & 1;
    ev->right = (status >> 3) & 1;
    ev->up = (status >> 1) & 1;
    ev->down = (status >> 2) & 1;
}
