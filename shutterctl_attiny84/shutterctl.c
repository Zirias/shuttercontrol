#include <stdio.h>
#include <avr/io.h>
#include <avr/avr_mcu_section.h>

#include "debug.h"

AVR_MCU(8000000, "attiny84");
AVR_MCU_VOLTAGES(3300, 3300, 3300);
AVR_MCU_SIMAVR_CONSOLE(&GPIOR0);

int main (void)
{
    debug("starting...\r");
    while(1);
    return 0;
}

