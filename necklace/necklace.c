#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#define F_CPU 4000000UL
#include <util/delay.h>

// Bit manipulation macros
#define sbi(a, b) ((a) |= 1 << (b))       //sets bit B in variable A
#define cbi(a, b) ((a) &= ~(1 << (b)))    //clears bit B in variable A
#define tbi(a, b) ((a) ^= 1 << (b))       //toggles bit B in variable A

char morse1[] PROGMEM = "..   .-.. .. -.- .   --. . . -.- ... .-.-.-   -.-. .- .-.. .-..   -- .   .- -   -.... ..... ----- ..--- ..--- ....- ..... -.... ..--- .....   .. ..-.   -.-- --- ..-   -.. . -.-. --- -.. .   - .... .. ... .-.-.-\0";
char morse2[] PROGMEM = "..   .-.. .. -.- .   --. . . -.- ... .-.-.-   -- .- .. .-..   -- .   .- -   -.-. ... -- .. - .... .---- ----- --...   .- -   --. -- .- .. .-..   -.. --- -   -.-. --- --   .. ..-.   -.-- --- ..-   -.. . -.-. --- -.. .   - .... .. ... .-.-.-\0";
char morse3[] PROGMEM = "-.- . . .--. / - .- .... --- . / -.-. .... .- .-. - .-. . ..- ... .\0";
#include "table.h"

#define SHORT_DUR 40 
#define LONG_DUR  120

void flash_led(unsigned char num)
{
    unsigned char i, j;

    sbi(PORTB, 0);
    for(i = 0; i < LONG_DUR; i++)
       _delay_ms(10);
    for(j = 0; j < num; j++)
    {
        cbi(PORTB, 0);
        for(i = 0; i < 40; i++)
            _delay_ms(10);

        sbi(PORTB, 0);
        for(i = 0; i < 40; i++)
            _delay_ms(10);
    }
    for(i = 0; i < LONG_DUR + LONG_DUR; i++)
       _delay_ms(10);
    cbi(PORTB, 0);
    for(i = 0; i < LONG_DUR + LONG_DUR; i++)
       _delay_ms(10);
}

void morse(int index, char message[] PROGMEM)
{
    char *PROGMEM  ptr;
    char          ch;
    unsigned int  delay;
    unsigned int  led;
    unsigned int  i;

    flash_led(index);

    for(ptr = message;; ptr++)
    {
        if (!(PINB & (1<<PINB4)))
            break;

        ch = pgm_read_byte(ptr);
        if (!ch)
        {
            ptr = message;
            continue;
        }

        if (ch == '-')
           delay = LONG_DUR;
        else
           delay = SHORT_DUR;

        if (ch == ' ')
           led = 0;
        else
           led = 1;

        if (led)
         	sbi(PORTB, 0);

        for(i = 0; i < delay; i++)
           _delay_ms(10);

       	cbi(PORTB, 0);

        for(i = 0; i < SHORT_DUR; i++)
           _delay_ms(10);
    }
}

#define SNORE_PWM_LENGTH 100
void pwm_led(unsigned char dc, unsigned char len, unsigned char loops)
{
    unsigned char i, j;

    for(i = 0; i < loops; i++)
    {
        sbi(PORTB, 0);
        for(j = 0; j < dc; j++)
            _delay_us(1);

        cbi(PORTB, 0);
        for(j = 0; j < len - dc; j++)
            _delay_us(1);
    }
}

void snore_led(void)
{
    unsigned char i = 0;
    char *PROGMEM  ptr;

    while(1)
    {
        if (!(PINB & (1<<PINB4)))
            break;
        for(i = 0, ptr = snore_table; i < SNORE_STEPS; i++, ptr++)
            pwm_led(pgm_read_byte(ptr), SNORE_PWM_LENGTH, 50);
        for(i = 0; i < SNORE_STEPS; i++, ptr++)
            pwm_led(pgm_read_byte(ptr), SNORE_PWM_LENGTH, 50);
    }
}


void transition(void)
{
    unsigned char i;
    while(1)
    {
        if ((PINB & (1<<PINB4)))
            break;

        sbi(PORTB, 0);
        for(i = 0; i < 5; i++)
            _delay_ms(10);

        cbi(PORTB, 0);
        for(i = 0; i < 5; i++)
            _delay_ms(10);
    }
}

int main(void)
{
    DDRB |= (1<<PB0);		//Turn LED pin to output 

    while(1)
    {
        transition();
        snore_led();
        transition();
        morse(1, morse1);
        transition();
        morse(2, morse2);
        transition();
        morse(3, morse3);
        transition();
    }
}
