#include <stdio.h>
#include <avr/io.h>
#include <avr/avr_mcu_section.h>

#include "debug.h"
#include "eepdata.h"

AVR_MCU(8000000, "attiny84");
AVR_MCU_VOLTAGES(3300, 3300, 3300);
AVR_MCU_SIMAVR_CONSOLE(&GPIOR0);

int main (void)
{
    eepdata_init();
    debugf("address: %d.\r", eep_address);
    while(1);
    return 0;
}

