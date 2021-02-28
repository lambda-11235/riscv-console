
#include <stddef.h>

#include "util.h"

#include "video.h"


volatile uint8_t* BG_DATA_START = (uint8_t*) 0x50000000;
volatile uint8_t* LS_DATA_START = (uint8_t*) 0x500B4000;
volatile uint8_t* SS_DATA_START = (uint8_t*) 0x500F4000;
volatile uint32_t* BG_PALETTE_START = (uint32_t*) 0x500FC000;
volatile uint32_t* SPRITE_PALETTE_START = (uint32_t*) 0x500FD000;
volatile uint32_t* FONT_DATA = (uint32_t*) 0x500FE000;
volatile char* TEXT_DATA = (char*) 0x500FE800;
volatile uint32_t* BG_CONTROL_START = (uint32_t*) 0x500FF100;
volatile uint32_t* LS_CONTROL_START = (uint32_t*) 0x500FF114;
volatile uint32_t* SS_CONTROL_START = (uint32_t*) 0x500FF214;
volatile uint32_t* MODE_CONTROL = (uint32_t*) 0x500FF414;


int video_set_mode(enum video_mode mode) {
    *MODE_CONTROL = mode;
}


int video_clear_text(void) {
    for (int i = 0; i < TEXT_HEIGHT*TEXT_WIDTH; i++)
        TEXT_DATA[i] = 0;

    return 0;
}


int video_write_text(int x, int y, char* str) {
    return strcpy_(TEXT_DATA + x + TEXT_WIDTH*y, str);
}


int video_write_bg_palette_data(int pal_id, uint32_t data[BG_PALETTE_SIZE]) {
    size_t amount = sizeof(uint32_t)*BG_PALETTE_SIZE;
    return memcpy_((uint8_t*) (BG_PALETTE_START + amount*pal_id),
                   (uint8_t*) data,
                   amount);
}


int video_write_sprite_palette_data(int pal_id, uint32_t data[SPRITE_PALETTE_SIZE]) {
    size_t amount = sizeof(uint32_t)*SPRITE_PALETTE_SIZE;
    return memcpy_((uint8_t*) (SPRITE_PALETTE_START + amount*pal_id),
                   (uint8_t*) data,
                   amount);
}


int video_write_bg_data(int bg_id, uint8_t data[BG_SIZE]) {
    return memcpy_(BG_DATA_START + bg_id*BG_SIZE,
                   data,
                   BG_SIZE);
}


int video_write_bg_control(int bg_id, struct bg_control* control) {
    uint32_t new_val = 0;
    new_val |= control->palette & 0x3;
    new_val |= ((control->x + 512) & 0x3ff) << 2;
    new_val |= ((control->y + 288) & 0x3ff) << 12;
    new_val |= (control->z & 0x7) << 22;

    BG_CONTROL_START[bg_id] = new_val;
    return 0;
}


int video_write_ls_data(int ls_id, uint8_t data[LS_SIZE]) {
    return memcpy_(LS_DATA_START + ls_id*LS_SIZE,
                   data,
                   LS_SIZE);
}


int video_write_ls_control(int ls_id, struct ls_control* control) {
    uint32_t new_val = 0;
    new_val |= control->palette & 0x3;
    new_val |= ((control->x + 64) & 0x3ff) << 2;
    new_val |= ((control->y + 64) & 0x1ff) << 12;
    new_val |= ((control->w - 33) & 0x1f) << 21;
    new_val |= ((control->h - 33) & 0x1f) << 26;

    LS_CONTROL_START[ls_id] = new_val;
    return 0;
}


int video_write_ss_data(int ss_id, uint8_t data[SS_SIZE]) {
    return memcpy_(SS_DATA_START + ss_id*SS_SIZE,
                   data,
                   SS_SIZE);
}


int video_write_ss_control(int ss_id, struct ss_control* control) {
    uint32_t new_val = 0;
    new_val |= control->palette & 0x3;
    new_val |= ((control->x + 16) & 0x3ff) << 2;
    new_val |= ((control->y + 16) & 0x1ff) << 12;
    new_val |= ((control->w - 1) & 0x1f) << 21;
    new_val |= ((control->h - 1) & 0x1f) << 26;

    LS_CONTROL_START[ss_id] = new_val;
    return 0;
}
