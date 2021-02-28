
#include <tos/video.h>

#include "syscall.h"


int video_set_mode(enum video_mode mode) {
    uint32_t args[5];
    args[0] = (uint32_t) mode;
    return syscall(256, args);
}


int video_clear_text(void) {
    uint32_t args[5];
    return syscall(257, args);
}


int video_write_text(int x, int y, char* str) {
    uint32_t args[5];
    args[0] = (uint32_t) x;
    args[1] = (uint32_t) y;
    args[2] = (uint32_t) str;
    return syscall(258, args);
}


int video_write_bg_palette_data(int pal_id, uint32_t data[BG_PALETTE_SIZE]) {
    uint32_t args[5];
    args[0] = (uint32_t) pal_id;
    args[1] = (uint32_t) data;
    return syscall(259, args);
}


int video_write_sprite_palette_data(int pal_id, uint32_t data[SPRITE_PALETTE_SIZE]) {
    uint32_t args[5];
    args[0] = (uint32_t) pal_id;
    args[1] = (uint32_t) data;
    return syscall(260, args);
}


int video_write_bg_data(int bg_id, uint8_t data[BG_SIZE]) {
    uint32_t args[5];
    args[0] = (uint32_t) bg_id;
    args[1] = (uint32_t) data;
    return syscall(261, args);
}


int video_write_bg_control(int bg_id, struct bg_control* control) {
    uint32_t args[5];
    args[0] = (uint32_t) bg_id;
    args[1] = (uint32_t) control;
    return syscall(262, args);
}


int video_write_ls_data(int ls_id, uint8_t data[LS_SIZE]) {
    uint32_t args[5];
    args[0] = (uint32_t) ls_id;
    args[1] = (uint32_t) data;
    return syscall(263, args);
}


int video_write_ls_control(int ls_id, struct ls_control* control) {
    uint32_t args[5];
    args[0] = (uint32_t) ls_id;
    args[1] = (uint32_t) control;
    return syscall(264, args);
}


int video_write_ss_data(int ss_id, uint8_t data[SS_SIZE]) {
    uint32_t args[5];
    args[0] = (uint32_t) ss_id;
    args[1] = (uint32_t) data;
    return syscall(265, args);
}


int video_write_ss_control(int ss_id, struct ss_control* control) {
    uint32_t args[5];
    args[0] = (uint32_t) ss_id;
    args[1] = (uint32_t) control;
    return syscall(266, args);
}
