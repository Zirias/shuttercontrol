#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/avr_mcu_section.h>

#include "debug.h"
#include "eepdata.h"

AVR_MCU(8000000, "attiny84");
AVR_MCU_VOLTAGES(3300, 3300, 3300);
AVR_MCU_SIMAVR_CONSOLE(&GPIOR0);

void loop(void) __attribute__((__noreturn__));

#define BUS_START 0
#define BUS_READ 1

volatile static uint8_t interrupted = 0;
volatile static uint8_t elapsed = 0;
static uint8_t b = 0xf;

ISR(PCINT1_vect)
{
    ++interrupted;
}

ISR(TIM1_COMPA_vect)
{
    ++elapsed;
}

int main(void)
{
    DDRA = 0xc0;
    PORTA = 0x3f;
    DDRB = 0x00;
    PORTB = 0x0f;

    eepdata_init();

    GIMSK |= _BV(PCIE1);
    PCMSK1 |= _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10);
    TIMSK1 |= _BV(OCIE1A);
    sei();
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    loop();
}

void startTimer(void)
{
    GTCCR |= _BV(TSM) | _BV(PSR10);
    TCCR1B |= _BV(WGM12) | _BV(CS10) | _BV(CS11); /* CTC, 8MHz / 64 */
    OCR1A = 12500; /* 100 ms */
    elapsed = 0;
    set_sleep_mode(SLEEP_MODE_IDLE);
    GTCCR &= ~_BV(TSM);
}

void stopTimer(void)
{
    GTCCR |= _BV(TSM) | _BV(PSR10);
    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    elapsed = 0;
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    GTCCR &= ~_BV(TSM);
}

void loop(void)
{
    uint8_t lastb = b;
    while(1) {
	if (!interrupted) __asm__("sleep");
	if (interrupted)
	{
	    --interrupted;
	    b = PINB;
	    if (lastb == b) continue;
	    if ((b & 1) != (lastb & 1))
	    {
		if (b & 1)
		{
		    if (elapsed > 2)
		    {
			stopTimer();
			PORTA &= ~_BV(PA6);
		    }
		}
		else
		{
		    if (elapsed)
		    {
			stopTimer();
			PORTA &= ~(_BV(PA6) | _BV(PA7));
		    }
		    else
		    {
			PORTA |= _BV(PA6);
			startTimer();
		    }
		}
	    }
	    else if ((b & 2) != (lastb & 2))
	    {
		if (b & 2)
		{
		    if (elapsed > 2)
		    {
			stopTimer();
			PORTA &= ~_BV(PA7);
		    }
		}
		else
		{
		    if (elapsed)
		    {
			stopTimer();
			PORTA &= ~(_BV(PA6) | _BV(PA7));
		    }
		    else
		    {
			PORTA |= _BV(PA7);
			startTimer();
		    }
		}
	    }
	    lastb = b;
	}
	if (elapsed > 100)
	{
	    stopTimer();
	    PORTA &= ~(_BV(PA6) | _BV(PA7));
	}
    }
    __builtin_unreachable();
}
