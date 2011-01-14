#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#define F_CPU 16000000UL
#include <util/delay.h>

// Bit manipulation macros
#define sbi(a, b) ((a) |= 1 << (b))       //sets bit B in variable A
#define cbi(a, b) ((a) &= ~(1 << (b)))    //clears bit B in variable A
#define tbi(a, b) ((a) ^= 1 << (b))       //toggles bit B in variable A

void pwm_setup(void)
{
	/* Set to Fast PWM */
	TCCR0A |= _BV(WGM01) | _BV(WGM00);
	TCCR2A |= _BV(WGM21) | _BV(WGM20);

	// Set the compare output mode
	TCCR0A |= _BV(COM0A1);
	TCCR0A |= _BV(COM0B1);
	TCCR2A |= _BV(COM2B1);

	// Reset timers and comparators
	OCR0A = 0;
	OCR0B = 0;
	OCR2B = 0;
	TCNT0 = 0;
	TCNT2 = 0;

    // Set the clock source
	TCCR0B |= _BV(CS00);
	TCCR2B |= _BV(CS20);

    // Set PWM pins as outputs
    DDRD |= (1<<PD6)|(1<<PD5)|(1<<PD3);
}

void set_led_color(uint8_t red, uint8_t green, uint8_t blue)
{
    OCR2B = 255 - red;
    OCR0A = 255 - blue;
    OCR0B = 255 - green;
}

void flash_led(void)
{
    uint8_t i;

    for(i = 0; i < 3; i++)
    {
        set_led_color(255, 0, 0);
        _delay_ms(50);
        set_led_color(0, 0, 0);
        _delay_ms(50);

        set_led_color(0, 255, 0);
        _delay_ms(50);
        set_led_color(0, 0, 0);
        _delay_ms(50);

        set_led_color(0, 0, 255);
        _delay_ms(50);
        set_led_color(0, 0, 0);
        _delay_ms(50);
    }
}

int fade_test(void)
{
    uint8_t i;

	while (1)
    {
        for(i = 0; i < 255; i++)
        {
            set_led_color(i, 0, 0);
		    _delay_ms(20); 
        }
        set_led_color(0, 0, 0);
        for(i = 0; i < 255; i++)
        {
            set_led_color(0, i, 0);
		    _delay_ms(20); 
        }
        set_led_color(0, 0, 0);
        for(i = 0; i < 255; i++)
        {
            set_led_color(0, 0, i);
		    _delay_ms(20); 
        }
        set_led_color(0, 0, 0);
	}
	return 0;
}

void fade(uint8_t r1, uint8_t g1, uint8_t b1,
          uint8_t r2, uint8_t g2, uint8_t b2,
          uint8_t steps, uint8_t delay)
{
    int16_t i;

    for(i = 0; i < steps; i++)
    {
        set_led_color((uint8_t)(r1 + (uint8_t)(((int16_t)(r2 - r1) * i) / steps)),
                      (uint8_t)(g1 + (uint8_t)(((int16_t)(g2 - g1) * i) / steps)),
                      (uint8_t)(b1 + (uint8_t)(((int16_t)(b2 - b1) * i) / steps)));
        _delay_ms(delay); 
    }
}

int main(void)
{
    uint8_t i, r1, g1, b1, r2, b2, g2;

    pwm_setup();
    flash_led();

    r1 = g1 = b1 = 0;
	while (1)
    {
        r2 = random() % 255;
        g2 = random() % 255;
        b2 = random() % 255;
        fade(r1, g1, b1, r2, g2, b2, 100, 10);
        r1 = r2;
        g1 = g2;
        b1 = b2;
	}
	return 0;
}
