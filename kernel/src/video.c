
#include "video.h"


volatile const uint8_t* BG_DATA_START = (uint8_t*) 0x50000000;
volatile const uint8_t* LS_DATA_START = (uint8_t*) 0x500B4000;
volatile const uint8_t* SS_DATA_START = (uint8_t*) 0x500F4000;
volatile const uint32_t* BG_PALETTE_START = (uint32_t*) 0x500FC000;
volatile const uint32_t* SPRITE_PALETTE_START = (uint32_t*) 0x500FD000;
volatile const uint32_t* FONT_DATA = (uint32_t*) 0x500FE000;
volatile const uint32_t* TEXT_DATA = (uint32_t*) 0x500FE800;
volatile const uint32_t* BG_CONTROL_START = (uint32_t*) 0x500FF100;
volatile const uint32_t* LS_CONTROL_START = (uint32_t*) 0x500FF114;
volatile const uint32_t* SS_CONTROL_START = (uint32_t*) 0x500FF214;
volatile const uint32_t* MODE_CONTROL = (uint32_t*) 0x500FF414;


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
    return 0;
}


int video_write_sprite_palette_data(int pal_id, uint32_t data[SPRITE_PALETTE_SIZE]) {
    return 0;
}


int video_write_bg_data(int bg_id, uint8_t data[BG_SIZE]) {
    return 0;
}


int video_write_bg_control(int bg_id, int x, int y, int z, int palette) {
    return 0;
}


int video_write_ls_data(int ls_id, uint8_t data[LS_SIZE]) {
    return 0;
}


int video_write_ls_control(int ls_id, int h, int w, int x, int y, int palette) {
    return 0;
}


int video_write_ss_data(int ss_id, uint8_t data[SS_SIZE]) {
    return 0;
}


int video_write_ss_control(int ss_id, int h, int w, int x, int y, int palette) {
    return 0;
}
