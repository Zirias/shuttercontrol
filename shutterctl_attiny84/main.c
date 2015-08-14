#include <stdio.h>
#include <avr/io.h>
#include <avr/avr_mcu_section.h>

#include "eepdata.h"
#include "event.h"
#include "timer.h"
#include "shutterctl.h"
#include "buttons.h"
#include "bus.h"

AVR_MCU(1000000, "attiny84");
AVR_MCU_VOLTAGES(5000, 5000, 5000);
AVR_MCU_SIMAVR_CONSOLE(&GPIOR0);

void __attribute__((__naked__,__noreturn__)) main(void)
{
    /* initialize all modules */
    eepdata_init();
    event_init();
    timer_init();
    shutterctl_init();
    buttons_init();
    bus_init();

    /* never come back */
    event_loop();
}

