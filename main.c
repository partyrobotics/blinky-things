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

volatile short ticks = 0;
ISR (TIMER0_OVF_vect) 
{
    ticks--; 
}

int stop(void)
{
    if (ticks <= 0)
    {
        sbi(PORTD, 4);
        _delay_ms(50);
        cbi(PORTD, 4);
    }
    return (ticks <= 0);
}

// This is the init sequence that executes right after boot to indicate
// that the controller has booted.
void flying_logo(void)
{
    buffer_t buf[XMAS_LIGHT_COUNT];
    int      i;

    blit_init();
    memset(&buf, 0, sizeof(buf));
    for(i = 0; i < XMAS_LIGHT_COUNT; i+= 2)
         buf[i] = xcolor(0xF, 0, 0xF, 0xF);
    for(i = 1; i < XMAS_LIGHT_COUNT; i+= 2)
         buf[i] = xcolor(0xF, 7, 0x0, 0xE);

    for(i = 0; i < 3; i++)
    {
        sbi(PORTD, 4);
        blit_buffer(buf);
        _delay_ms(100);

        shift_buffer_down(buf);
        cbi(PORTD, 4);
        blit_buffer(buf);
        _delay_ms(100);
    }
}

void sweep_erase(void)
{
    int i;

    for(i = 0; i < 50; i++)
    {
       xmas_set_color(i, 0, XMAS_COLOR(0,0,0));
       _delay_ms(5);
    }
}

void chase(void)
{
    buffer_t buf[XMAS_LIGHT_COUNT];
    int      i;

    blit_init();
    memset(&buf, 0, sizeof(buf));
    for(i = 0; i < XMAS_LIGHT_COUNT; i+= 2)
         buf[i] = xcolor((random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 
                         (random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 
                         (random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 0xF);

    for(; !stop();)
    {
        blit_buffer(buf);
        shift_buffer_down(buf);
        _delay_ms(50);
    }
}

void drop_in(void)
{
    int i;
    buffer_t a[XMAS_LIGHT_COUNT];
    buffer_t b[XMAS_LIGHT_COUNT];

    blit_init();
    memset(&a, 0, sizeof(a));
    memset(&b, 0, sizeof(b));

    for(i = 0; i < XMAS_LIGHT_COUNT; i++)
    {
        a[i] = xcolor(((i / 5) % 2) ? 15 : 0, 0, 0, 15);
        b[i] = xcolor(0, ((i / 2) % 2) ? 15 : 0, ((i / 3) % 2) ? 15 : 0, 15);
    }

    for(;!stop();)
    {
        merge_blit(a, b);
        shift_buffer_down(a);
        shift_buffer_up(b);
        _delay_ms(100);
    }
}

void color_fill_and_sweep(void)
{
    int i;
    buffer_t a[XMAS_LIGHT_COUNT];

    blit_init();
    memset(&a, 0, sizeof(a));

    sweep_erase();

    blit_buffer(a);
    for(i = 0; i < 10; i++)
        _delay_ms(100);

    for(; !stop();)
    {
        for(i = 0; i < XMAS_LIGHT_COUNT; i++)
        {
            a[0] = xcolor((rand() % 15) * (rand() % 3 ? 1 : 0), 
                          (rand() % 15) * (rand() % 3 ? 1 : 0), 
                          (rand() % 15) * (rand() % 3 ? 1 : 0), 15);
            blit_buffer(a);
            shift_buffer_up(a);
            _delay_ms(75);
        }
        sweep_erase();
        memset(&a, 0, sizeof(a));
    }
}

// THIS CODE IS FROM THE ORIGINAL XMAS LIGHT HACKER. LICENSE NEEDS TO BE CLEARED
static uint8_t i;   // Used as a general purpose loop counter
static uint16_t c;

void double_rainbow()
{
    for(;!stop();)
    {
        for(i=0;i<XMAS_LIGHT_COUNT/2 && !stop(); i++) 
        {
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
}
// END OF CODE THAT IS FROM THE ORIGINAL XMAS LIGHT HACKER.

void random_colors(void)
{
    sweep_erase();
    while(!stop()) 
    {
        int  i;

        for(i = 0; i < 50 && !stop(); i++)
        {
            xmas_set_color(i, 0xCC, XMAS_COLOR((random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 
                                               (random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 
                                               (random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0)));
            _delay_ms(10);
        }
    }
}

static xmas_color_t colors[15] = { XMAS_COLOR(15, 4, 4), 
                                 XMAS_COLOR(15, 7, 7), 
                                 XMAS_COLOR(12, 3, 3), 
                                 XMAS_COLOR(15, 0, 0), 
                                 XMAS_COLOR(10, 0, 0), 
                                 XMAS_COLOR(11, 7, 3), 
                                 XMAS_COLOR(10, 4, 0), 
                                 XMAS_COLOR(15, 9, 4), 
                                 XMAS_COLOR(15, 11, 7), 
                                 XMAS_COLOR(9, 2, 6), 
                                 XMAS_COLOR(10, 0, 6), 
                                 XMAS_COLOR(13, 0, 7), 
                                 XMAS_COLOR(14, 3, 9), 
                                 XMAS_COLOR(14, 6, 10) 
};

void complementary_colors(void)
{
    for(;!stop();)
    {
        xmas_set_color(random() % XMAS_LIGHT_COUNT, 0xCC, colors[random() % 15]);
        _delay_ms(30);
    }
}

void fill_up(void)
{
    int i, j;
    xmas_color_t fill_colors[2];
    for(; !stop();)
    {
        for(j = 0; j < 2; j++)
            fill_colors[j] = XMAS_COLOR((random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 
                                        (random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0), 
                                        (random() % XMAS_CHANNEL_MAX) * (random() % 3 ? 1 : 0));
        for(i = XMAS_LIGHT_COUNT; i >= 0; i--)
        {
            for(j = 0; j < i; j++)
            {
                xmas_set_color(j, 0xCC, fill_colors[i % 2]);
                _delay_ms(10);
                xmas_set_color(j, 0xCC, XMAS_COLOR(0,0,0));
            }
            xmas_set_color(i, 0xCC, fill_colors[i % 2]);
        }
    }
}

int
main(void)
{
    int index = 0;

    // On board status LED
    DDRD |= (1<<PD4);
    DDRB |= (1<<PB0);

    TCCR0B |= (_BV(CS00) | _BV(CS02));
    TIMSK0 |= _BV(TOIE0);

    // Enumerate the bulbs in the string
    // for individual addressing.
    xmas_fill_color(
        0,
        XMAS_LIGHT_COUNT,
        0,
        XMAS_COLOR_BLACK
    );
    flying_logo();
    sei();

    for(;;)
    {
        if (index == 0)
        {
           ticks = 300;
           fill_up();
           index++;
        }
        else
        if (index == 1)
        {
           ticks = 600;
           double_rainbow();
           index++;
        }
        else
        if (index == 2)
        {
           ticks = 300;
           random_colors();
           index++;
        }
        else
        if (index == 3)
        {
           ticks = 300;
           drop_in();
           index++;
        }
        else
        if (index == 4)
        {
           ticks = 300;
           complementary_colors();
           index++;
        }
        else
        if (index == 5)
        {
           ticks = 300;
           color_fill_and_sweep();
           index++;
        }
        else
           index = 0;
    }
    return 0;
}
