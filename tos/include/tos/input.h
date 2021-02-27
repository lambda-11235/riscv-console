
#include <stdbool.h>
#include <stdint.h>


#ifndef _TOS_INPUT_H_
#define _TOS_INPUT_H_


struct input_ctlr {
    bool button1_pressed;
    bool button2_pressed;
    bool button3_pressed;
    bool button4_pressed;

    bool left;
    bool right;
    bool up;
    bool down;
};

/**
 * @brief Polls for controller input.
 *
 * @param ev The current controller input.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int input_ctlr_poll(struct input_ctlr* ev);


#endif /* _TOS_INPUT_H_ */
