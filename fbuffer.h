#ifndef __FBUFFER_H__
#define __FBUFFER_H__

typedef unsigned short buffer_t;

unsigned short xcolor(unsigned char r,unsigned char g,unsigned char b,unsigned char i);
void blit_init(void);
void blit_buffer(buffer_t buf[XMAS_LIGHT_COUNT]);
void merge_blit(buffer_t a[XMAS_LIGHT_COUNT], buffer_t b[XMAS_LIGHT_COUNT]);
void shift_buffer_down(buffer_t buf[XMAS_LIGHT_COUNT]);
void shift_buffer_up(buffer_t buf[XMAS_LIGHT_COUNT]);

#endif
