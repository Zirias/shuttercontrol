#include <stdio.h>
#include <avr/io.h>
#include <avr/avr_mcu_section.h>

#include "eepdata.h"
#include "event.h"
#include "timer.h"
#include "shutterctl.h"
#include "buttons.h"

AVR_MCU(8000000, "attiny84");
AVR_MCU_VOLTAGES(3300, 3300, 3300);
AVR_MCU_SIMAVR_CONSOLE(&GPIOR0);

int main(void)
{
    /* initial pin configuration */
    DDRA = 0xc0;
    PORTA = 0x3f;
    DDRB = 0x00;
    PORTB = 0x0f;

    /* initialize all modules */
    eepdata_init();
    event_init();
    timer_init();
    shutterctl_init();
    buttons_init();

    /* never come back */
    event_loop();
}

