#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/avr_mcu_section.h>

#include "debug.h"
#include "eepdata.h"

AVR_MCU(8000000, "attiny84");
AVR_MCU_VOLTAGES(3300, 3300, 3300);
AVR_MCU_SIMAVR_CONSOLE(&GPIOR0);

ISR(PCINT1_vect)
{
    debugf("[0x%x] interrupted.\r", eep_address);
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
    while(1) {
	__asm__("sleep");
    }
    return 0;
}

