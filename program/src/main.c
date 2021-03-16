
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <tos/input.h>
#include <tos/signal.h>
#include <tos/stdlib.h>
#include <tos/thread.h>
#include <tos/video.h>


char bufA[256];
char bufB[256];

struct mutex_t* lock;
volatile int shared;


void video_handler(int sig) {
    char buf[256];
    
    mutex_lock(lock);

    video_write_text(0, 0, bufA);
    video_write_text(0, 1, bufB);

    sprintf(buf, "Shared: %d", shared);
    video_write_text(0, 2, buf);

    mutex_unlock(lock);
}


int thread_b(void* data) {
    uint64_t tm = 1000000;
    int cnt = 0;

    while (1) {
        mutex_lock(lock);
        
        cnt = (cnt + 1)%256;
        sprintf(bufB, "Thread B: %d", cnt);
        shared++;

        mutex_unlock(lock);
        
        thread_sleep_us(&tm);
    }

    return 0;
}


int main() {
    uint64_t tm = 1000000;
    int cnt = 0;

    lock = mutex_new();
    
    video_set_mode(TEXT_MODE);
    video_clear_text();
    signal_register(3, video_handler);
    //thread_set_preemption(0);

    if (thread_create(thread_b, NULL) == -1) {
        fault("Could not create thread");
    }

    while (1) {
        mutex_lock(lock);

        cnt = (cnt + 1)%256;
        sprintf(bufA, "Thread A: %d", cnt);
        shared++;

        mutex_unlock(lock);

        thread_sleep_us(&tm);
    }

    return 0;
}
