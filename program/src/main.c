
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <tos/input.h>
#include <tos/signal.h>
#include <tos/stdlib.h>
#include <tos/thread.h>
#include <tos/video.h>

uint64_t tm = 100000;

volatile char* TEXT_DATA = (char*) 0x500FE800;

int thread_b(void* data) {
    char buf[256];
    int cnt = 0;

    while (1) {
        cnt = (cnt + 1)%256;
        sprintf(buf, "Thread B: %d", cnt);
        video_write_text(0, 1, buf);
        thread_yield();
    }

    return 0;
}

int main() {
    char buf[256];
    int cnt = 0;

    video_set_mode(TEXT_MODE);
    video_clear_text();
    set_timeout_us(&tm);
    
    if (thread_create(thread_b, NULL) == -1) {
        fault("Could not create thread");
    }

    while (1) {
        uint64_t start, time;

        cnt = (cnt + 1)%256;
        sprintf(buf, "Thread A: %d", cnt);
        video_write_text(0, 0, buf);
        thread_yield();

        time_us(&start);
        time = start;

        while (time < start + tm) {
            time_us(&time);
        }
    }

    return 0;
}
