
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <tos/input.h>
#include <tos/signal.h>
#include <tos/stdlib.h>
#include <tos/video.h>

volatile int cmd_pressed;

void on_command(int sig) {
    cmd_pressed = 1;
}

int main() {
    char buf[256];
    cmd_pressed = 0;
        
    sprintf(buf, "%x", 26);
    video_set_mode(TEXT_MODE);
    video_clear_text();
    video_write_text(0, 0, buf);

    signal_register(4, on_command);

    while (!cmd_pressed) {}

    return 0;
}
