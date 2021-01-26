
#ifndef _VIDEO_H_
#define _VIDEO_H_

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xFE800);
const unsigned int TEXT_HEIGHT = 36;
const unsigned int TEXT_WIDTH = 64;

#endif /* _VIDEO_H_ */
