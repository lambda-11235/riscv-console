
#include <stdint.h>


#ifndef _TOS_VIDEO_H
#define _TOS_VIDEO_H


enum video_mode {TEXT_MODE=0, GRAPHICS_MODE=1};

/**
 * @brief Sets the video mode.
 *
 * @param mode Either text or graphics mode.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_set_mode(enum video_mode mode);


/// Height of text on screen.
#define TEXT_HEIGHT 36
/// Width of text on screen.
#define TEXT_WIDTH 64

/**
 * @brief Clears all text from the screen.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_clear_text(void);

/**
 * @brief Writes a string to the screen.
 *
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param str A NULL terminated string to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_text(int x, int y, char* str);


/// Background palette entries.
#define BG_PALETTE_SIZE 256
/// Sprite palette entries.
#define SPRITE_PALETTE_SIZE 256

/**
 * @brief Writes background palette ARGB data.
 *
 * @param pal_id The ID of the background palette to modify.
 * @param data The data to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_bg_palette_data(int pal_id, uint32_t data[BG_PALETTE_SIZE]);

/**
 * @brief Writes sprite palette ARGB data.
 *
 * @param pal_id The ID of the sprite palette to modify.
 * @param data The data to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_sprite_palette_data(int pal_id, uint32_t data[SPRITE_PALETTE_SIZE]);


/// Number of pixels in a background.
#define BG_SIZE 0x24000

/**
 * @brief Writes background pixel data.
 *
 * @param bg_id The ID of the background to modify.
 * @param data The data to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_bg_data(int bg_id, uint8_t data[BG_SIZE]);


struct bg_control {
    int x; int y; int z;
    int palette;
};

/**
 * @brief Writes background control information.
 *
 * @param bg_id The ID of the background to modify.
 * @param control The control data to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_bg_control(int bg_id, struct bg_control* control);


/// Number of pixels in a large sprite.
#define LS_SIZE 0x1000

/**
 * @brief Writes large sprite pixel data.
 *
 * @param ls_id The ID of the large sprite to modify.
 * @param data The data to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_ls_data(int ls_id, uint8_t data[LS_SIZE]);


struct ls_control {
    int h; int w;
    int x; int y;
    int palette;
};

/**
 * @brief Writes large sprite control information.
 *
 * @param ls_id The ID of the large sprite to modify.
 * @param control The control data to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_ls_control(int ls_id, struct ls_control* control);


/// Number of pixels in a small sprite.
#define SS_SIZE 0x100

/**
 * @brief Writes small sprite pixel data.
 *
 * @param ss_id The ID of the small sprite to modify.
 * @param data The data to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_ss_data(int ss_id, uint8_t data[SS_SIZE]);


struct ss_control {
    int h; int w;
    int x; int y;
    int palette;
};

/**
 * @brief Writes small sprite control information.
 *
 * @param ss_id The ID of the small sprite to modify.
 * @param control The control data to write.
 *
 * @return On success returns 0, and on failure returns -1.
 */
int video_write_ss_control(int ss_id, struct ss_control* control);


#endif /* _TOS_VIDEO_H */
