
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <tos/input.h>
#include <tos/signal.h>
#include <tos/stdlib.h>
#include <tos/thread.h>
#include <tos/video.h>


#define STR_(X) #X
#define STR(X) STR_(X)
#define F(x) {if (x == -1) fault(__FILE__ " " STR(__LINE__) ": " #x " == -1");}

volatile int cntA;
volatile int cntB;
volatile int cntC;


struct mutex_t* lockA;
struct mutex_t* lockB;
struct condvar_t* cv;


#define US_PER_MS  1000
#define US_PER_SEC 1000000


void video_handler(int sig) {
    char buf[256];
    
    F(mutex_lock(lockA));

    sprintf(buf, "Thread A: %d", cntA);
    F(video_write_text(0, 0, buf));

    sprintf(buf, "Thread C: %d", cntC);
    F(video_write_text(0, 2, buf));
    
    F(mutex_unlock(lockA));

    
    F(mutex_lock(lockB));

    sprintf(buf, "Thread B: %d", cntB);
    F(video_write_text(0, 1, buf));

    F(mutex_unlock(lockB));
}


int thread_b(void* data) {
    uint64_t tm = 700*US_PER_MS;

    while (1) {
        F(mutex_lock(lockB));
        cntB = (cntB + 1)%256;
        F(mutex_unlock(lockB));
        
        F(thread_sleep_us(&tm));
    }

    return 0;
}


int thread_c(void* data) {
    F(mutex_lock(lockA));

    while (1) {
        while (cntA < cntC)
            F(condvar_wait(cv, lockA));

        cntC = 2*cntA;
    }
        
    F(mutex_unlock(lockA));

    return 0;
}


int main() {
    uint64_t tm = 1100*US_PER_MS;

    cntA = 0;
    cntB = 0;
    cntC = 0;

    lockA = mutex_new();
    lockB = mutex_new();
    cv = condvar_new();
    
    video_set_mode(TEXT_MODE);
    video_clear_text();
    signal_register(3, video_handler);
    //thread_set_preemption(0);

    if (thread_create(thread_b, NULL) == -1) {
        fault("Could not create thread B");
    }

    if (thread_create(thread_c, NULL) == -1) {
        fault("Could not create thread C");
    }

    while (1) {
        F(mutex_lock(lockA));

        cntA = (cntA + 1)%256;

        if (cntA >= cntC)
            F(condvar_signal(cv));
        
        F(mutex_unlock(lockA));

        F(thread_sleep_us(&tm));
    }

    F(mutex_free(lockA));
    F(mutex_free(lockB));
    F(condvar_free(cv));

    return 0;
}
