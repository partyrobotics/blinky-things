// 
// Copyright (c) Party Robotics LLC 2010
// Written by Robert Kaye <rob@partyrobotics.com>
//
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "xmas_lib.h"

#define min(a,b) ((a)<(b)?(a):(b))
typedef unsigned short buffer_t;

unsigned short xcolor(unsigned char r,unsigned char g,unsigned char b,unsigned char i) 
{
    return ((b)|((g)<<4)|((r)<<8)|((i)<<12));
}

static buffer_t current[XMAS_LIGHT_COUNT];

void blit_init(void)
{
    memset(&current, 0, sizeof(current));
}

void blit_buffer(buffer_t buf[XMAS_LIGHT_COUNT])
{
    int i;

    for(i = 0; i < XMAS_LIGHT_COUNT; i++)
    {
        unsigned short t = buf[i];
        if (current[i] != t)
        {
            unsigned int in = min((t >> 12) * 14, XMAS_DEFAULT_INTENSITY);
            xmas_set_color(i, in, XMAS_COLOR((t >> 8) & 0xF, (t >> 4) & 0xF, t & 0xF));
            current[i] = t;
        }
    }
}

void merge_blit(buffer_t a[XMAS_LIGHT_COUNT], buffer_t b[XMAS_LIGHT_COUNT])
{
    int i;

    for(i = 0; i < XMAS_LIGHT_COUNT; i++)
    {
        unsigned char r, g, bl;
        unsigned short t;
        unsigned short ta = a[i];
        unsigned short tb = b[i];

        unsigned int in = min(((ta >> 12) * 14) + ((tb >> 12) * 14), XMAS_DEFAULT_INTENSITY);
        r = min(0xF, ((ta >> 8) & 0xF) + ((tb >> 8) & 0xF)),  
        g = min(0xF, ((ta >> 4) & 0xF) + ((tb >> 4) & 0xF)), 
        bl = min(0xF, (ta & 0xF) + (tb & 0xF));
        t = xcolor(r,g,bl,in);
        if (current[i] != t)
        {
            xmas_set_color(i, in, XMAS_COLOR(r, g, bl));
            current[i] = t;
        }
    }
}

void shift_buffer_down(buffer_t buf[XMAS_LIGHT_COUNT])
{
    int i;
    unsigned short carry;

    carry = buf[0];
    for(i = 0; i < XMAS_LIGHT_COUNT - 1; i++)
        buf[i] = buf[i+1];
    buf[XMAS_LIGHT_COUNT-1] = carry;
}

void shift_buffer_up(buffer_t buf[XMAS_LIGHT_COUNT])
{
    int i;
    unsigned short carry;

    carry = buf[XMAS_LIGHT_COUNT-1];
    for(i = XMAS_LIGHT_COUNT - 1; i > 0; i--)
        buf[i] = buf[i-1];
    buf[0] = carry;
}

#if 0
int main(int argc, char argv[])
{
    buffer_t b[50];
    int i;

    memset(&b, 0, sizeof(b));

    blit_init();
    b[0] = xcolor(0xF, 0xF, 0xF, 0);
    b[1] = xcolor(0xF, 0xF, 0, 0);
    b[2] = xcolor(0xF, 0, 0xF, 0);
    b[3] = xcolor(0xE, 0, 0xF, 0);

    for(i = 0; i < 50; i++)
        printf("%2d: %d %d %d %d\n", i, (b[i] >> 12), (b[i] >> 8) & 0xF, (b[i] >> 4) & 0xF, b[i] & 0xF);
    printf("\n");
    shift_buffer_down(b);
    for(i = 0; i < 50; i++)
        printf("%2d: %d %d %d %d\n", i, (b[i] >> 12), (b[i] >> 8) & 0xF, (b[i] >> 4) & 0xF, b[i] & 0xF);
    printf("\n");

    return 0;
}
#endif
