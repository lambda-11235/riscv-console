
#include <stdint.h>
#include <stddef.h>

#include <tos/input.h>
#include <tos/stdlib.h>
#include <tos/video.h>


void setup_graphics(void);

int main() {
    uint64_t last_time, time;
    struct input_ctlr ctlr;
    struct bg_control bgc;
    struct ls_control lsc;
    int vx = 1; int vy = 1;

    video_set_mode(TEXT_MODE);
    video_clear_text();
    video_write_text(0, 0, "Press left button to start simulation,");
    video_write_text(2, 1, "then press right button to return to kernel.");

    do {
        input_ctlr_poll(&ctlr);
    } while (!ctlr.left);

    video_set_mode(GRAPHICS_MODE);
    setup_graphics();

    bgc.x = 0; bgc.y = 0; bgc.z = 0;
    bgc.palette = 0;
    video_write_bg_control(0, &bgc);

    lsc.x = 0; lsc.y = 0;
    lsc.h = 64; lsc.w = 64;
    lsc.palette = 0;
    video_write_ls_control(0, &lsc);

    time_us(&last_time);

    do {
        int dt, dx, dy;

        time_us(&time);
        dt = (int) (time - last_time);
        dx = vx*dt/10000;
        dy = vy*dt/10000;

        if (dx != 0 && dy != 0) {
            last_time = time;

            lsc.x += dx;
            lsc.y += dy;

            if (lsc.x < 0)
                vx = 1;
            else if (lsc.x > 512-64)
                vx = -1;

            if (lsc.y < 0)
                vy = 1;
            else if (lsc.y > 288-64)
                vy = -1;
        }

        video_write_ls_control(0, &lsc);

        input_ctlr_poll(&ctlr);
    } while (!ctlr.right);

    return ((int) time)/1000000;
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
}
