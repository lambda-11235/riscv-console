
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <tos/input.h>
#include <tos/signal.h>
#include <tos/stdlib.h>
#include <tos/video.h>

volatile int cmd_pressed;

int main() {
    struct input_ctlr ctlr = {0};
    char buf[256];
    cmd_pressed = 0;
        
    sprintf(buf, "%x", 26);
    video_set_mode(TEXT_MODE);
    video_clear_text();
    video_write_text(0, 0, buf);

    while (!ctlr.up) {
        input_ctlr_poll(&ctlr);
    }

    return 0;
}
