#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "xmas_lib.h"

#define sbi(x,y)    x|=(1<<y)
#define cbi(x,y)    x&=~(1<<y)

static uint8_t i;   // Used as a general purpose loop counter
static uint16_t c;
//static uint8_t xmas_program;

static void
update_simple_color_cycle()
{
    xmas_fill_color(
        i,
        XMAS_LIGHT_COUNT,
        XMAS_DEFAULT_INTENSITY,
        xmas_color_hue((c/8)%(XMAS_HUE_MAX+1))
    );
    c++;
}

static void
update_rainbow()
{
    for(i=0;i<XMAS_LIGHT_COUNT;i++) {
        xmas_set_color(
            i,
            XMAS_DEFAULT_INTENSITY,
            xmas_color_hue((i+c)%(XMAS_HUE_MAX+1))
        );
    }
    c++;
}

/*  Bulbs must be enumerated for individual addressing
**  for this program to work properly.
*/
static void
update_double_rainbow()
{
    for(i=0;i<XMAS_LIGHT_COUNT/2;i++) {
        xmas_color_t color = xmas_color_hue((i+c)%(XMAS_HUE_MAX+1));
        xmas_set_color(
            i,
            XMAS_DEFAULT_INTENSITY,
            color
        );
        xmas_set_color(
            XMAS_LIGHT_COUNT-1-i,
            XMAS_DEFAULT_INTENSITY,
            color
        );
    }
    c++;
}

/*  This program is supposed to look like the
**  old chaser lights that were around the borders
**  of signs. It requires that the bulbs not
**  already be enumerated to work properly.
*/
void
update_now_playing()
{
    static const xmas_color_t color = XMAS_COLOR(15,7,2);
    static const uint8_t increment = 2;
    static const uint8_t upper_limit = XMAS_DEFAULT_INTENSITY/2/increment*increment;
    
    for(c=0;c<upper_limit;c+=increment) {
        xmas_set_color(0,XMAS_DEFAULT_INTENSITY/2+c,color);
        xmas_set_color(1,XMAS_DEFAULT_INTENSITY/2-c,color);
    }
    for(c=upper_limit;c!=0;c-=increment) {
        xmas_set_color(0,XMAS_DEFAULT_INTENSITY/2+c,color);
        xmas_set_color(1,XMAS_DEFAULT_INTENSITY/2-c,color);
    }
    for(c=0;c<upper_limit;c+=increment) {
        xmas_set_color(1,XMAS_DEFAULT_INTENSITY/2+c,color);
        xmas_set_color(0,XMAS_DEFAULT_INTENSITY/2-c,color);
    }
    for(c=upper_limit;c!=0;c-=increment) {
        xmas_set_color(1,XMAS_DEFAULT_INTENSITY/2+c,color);
        xmas_set_color(0,XMAS_DEFAULT_INTENSITY/2-c,color);
    }
}

void random_with_blue_cursor(void)
{
    while(!stop()) 
    {
        int  i, c, t;

        for(i = 0; i < 50 && !stop(); i++)
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
