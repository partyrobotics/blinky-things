#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "xmas_lib.h"
#include "fbuffer.h"

#define sbi(x,y)    x|=(1<<y)
#define cbi(x,y)    x&=~(1<<y)

void intensity_test_pattern(void)
{
    while(1) 
    {
        int  i, c, col;

        for(c = 0; c < 3; c++)
        {
            if (c == 0) col = XMAS_COLOR_RED;
            if (c == 1) col = XMAS_COLOR_GREEN;
            if (c == 2) col = XMAS_COLOR_BLUE;
            for(i = 0; i < 50; i++)
            {
                xmas_set_color(
                    i,
                    0xCC / 50 * i,
                    col
                );
            }
            for(i = 0; i < 10; i++)
                _delay_ms(100);
        }
    }
}    

