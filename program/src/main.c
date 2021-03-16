
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


#define US_PER_MS  1000
#define US_PER_SEC 1000000
#define SLOW_DOWN 50000


volatile int cntA;
volatile int cntB;
volatile int cntC;


volatile struct mutex_t* lockA;
volatile struct mutex_t* lockB;
volatile struct mutex_t* lockG;
volatile struct condvar_t* cv;


volatile struct bg_control bgc;
volatile struct ls_control lsc;

volatile enum video_mode mode;


void setup_graphics(void);


void on_cmd(int sig) {
    if (mode == TEXT_MODE)
        mode = GRAPHICS_MODE;
    else if (mode == GRAPHICS_MODE)
        mode = TEXT_MODE;

    video_set_mode(mode);
}


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


    F(mutex_lock(lockG));

    sprintf(buf, "lsc = {x = %d, y = %d}", lsc.x, lsc.y);
    F(video_write_text(0, 5, buf));
    F(video_write_ls_control(0, &lsc));

    F(mutex_unlock(lockG));
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


int thread_g(void* data) {
    uint64_t time, last_time;
    int dt, dx, dy;
    struct input_ctlr ctlr;

    time_us(&last_time);

    while (1) {
        // Handle graphics part
        time_us(&time);
        dt = (int) (time - last_time);

        input_ctlr_poll(&ctlr);

        dx = 0;
        dy = 0;

        if (ctlr.left)
            dx -= dt/SLOW_DOWN;
        if (ctlr.right)
            dx += dt/SLOW_DOWN;

        if (ctlr.down)
            dy += dt/SLOW_DOWN;
        if (ctlr.up)
            dy -= dt/SLOW_DOWN;

        if (!ctlr.left && !ctlr.right && !ctlr.down && !ctlr.up) {
            last_time = time;
        } else if (dx != 0 || dy != 0) {
            mutex_lock(lockG);

            last_time = time;

            lsc.x += dx;
            lsc.y += dy;

            if (lsc.x < 0)
                lsc.x = 0;
            else if (lsc.x > 512-64)
                lsc.x = 512-64;

            if (lsc.y < 0)
                lsc.y = 0;
            else if (lsc.y > 288-64)
                lsc.y = 288-64;

            mutex_unlock(lockG);
        }

        input_ctlr_poll(&ctlr);
    }

    return 0;
}


int main() {
    uint64_t tm = 1100*US_PER_MS;

    cntA = 0;
    cntB = 0;
    cntC = 0;

    lockA = mutex_new();
    lockB = mutex_new();
    lockG = mutex_new();
    cv = condvar_new();


    mode = TEXT_MODE;
    F(video_set_mode(mode));

    F(video_clear_text());
    F(signal_register(3, video_handler));
    F(signal_register(4, on_cmd));

    setup_graphics();

    if (thread_create(thread_b, NULL) == -1) {
        fault("Could not create thread B");
    }

    if (thread_create(thread_c, NULL) == -1) {
        fault("Could not create thread C");
    }

    if (thread_create(thread_g, NULL) == -1) {
        fault("Could not create thread G");
    }


    while (1) {
        F(mutex_lock(lockA));

        // Handle text part
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


void setup_graphics(void) {
    uint32_t* palette = mem_alloc(sizeof(uint32_t)*SPRITE_PALETTE_SIZE);
    uint8_t* bg_data = mem_alloc(BG_SIZE);
    uint8_t* ls_data = mem_alloc(LS_SIZE);

    palette[1] = 0xffff0000;
    palette[2] = 0xff0000ff;

    for (size_t i = 0; i < BG_SIZE; i++)
        bg_data[i] = 1;
    for (size_t i = 0; i < LS_SIZE; i++)
        ls_data[i] = 2;

    video_write_bg_palette_data(0, palette);
    video_write_sprite_palette_data(0, palette);
    video_write_bg_data(0, bg_data);
    video_write_ls_data(0, ls_data);

    mem_free(palette);
    mem_free(bg_data);
    mem_free(ls_data);
}
