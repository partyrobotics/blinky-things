#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "xmas_lib.h"

#define sbi(x,y)    x|=(1<<y)
#define cbi(x,y)    x&=~(1<<y)

void flash_led(unsigned char num)
{
    int i;
    for(i = 0; i < num; i++)
    {
        sbi(PORTD, 4);
        _delay_ms(100);

        cbi(PORTD, 4);
        _delay_ms(100);
    }
}

void simple_dot_moving(void)
{
    while(1) 
    {
        int  i;

        for(i = 0; i < 50; i++)
        {
            xmas_set_color(
                i,
                0xCC,
                XMAS_COLOR_RED
            );
            _delay_ms(10);
            xmas_set_color(
                i,
                0,
                XMAS_COLOR_RED
            );
        }
    }
}

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

int
main(void)
{
    // On board status LED
    DDRD |= (1<<PD4);
    DDRB |= (1<<PB0);
    flash_led(3);

    // Enumerate the bulbs in the string
    // for individual addressing.
    xmas_fill_color(
        0,
        XMAS_LIGHT_COUNT,
        0,
        XMAS_COLOR_BLACK
    );
    while(1) 
    {
        int  i, c, col, t;

        for(i = 0; i < 50; i++)
        {
            for(c = 0; c < 2; c++)
            {    
                for(t = 0; t < XMAS_DEFAULT_INTENSITY; t++)
                {    
                    xmas_set_color(i, t, XMAS_COLOR_BLUE);
                    _delay_us(200);
                }
                for(t = XMAS_DEFAULT_INTENSITY; t > 0; t--)
                {    
                    xmas_set_color(i, t, XMAS_COLOR_BLUE);
                    _delay_us(200);
                }
            }
            xmas_set_color(i, 0xCC, XMAS_COLOR((random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 
                                               (random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 
                                               (random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0)));
            _delay_ms(100);
        }
    }
}
