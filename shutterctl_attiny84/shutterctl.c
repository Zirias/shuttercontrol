#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
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
static uint8_t b = 0xf;

ISR(PCINT1_vect)
{
    ++interrupted;
    debugf("[0x%x] interrupted: %d.\r", eep_address, interrupted);
}

int main(void)
{
    DDRA = 0xc0;
    PORTA = 0x3f;
    DDRB = 0x00;
    PORTB = 0x0f;

    eepdata_init();
    debugf("[0x%x] starting.\r", eep_address);

    GIMSK |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10);
    sei();
    loop();
}

void loop(void)
{
    uint8_t lastb = b;
    while(1) {
	if (!interrupted) __asm__("sleep");
	--interrupted;
	b = PINB;
	if (lastb == b) continue;
	debugf("PINB: 0x%02x\r", b);
	if ((b & 1) != (lastb & 1))
	{
	    if (b & 1) PORTA &= ~(_BV(6));
	    else PORTA |= _BV(6);
	}
	else if ((b & 2) != (lastb & 2))
	{
	    if (b & 2) PORTA &= ~(_BV(7));
	    else PORTA |= _BV(7);
	}
	lastb = b;
    }
    __builtin_unreachable();
}
