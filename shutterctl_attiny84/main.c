#include <stdio.h>
#include <avr/avr_mcu_section.h>

#include "eepdata.h"
#include "event.h"
#include "shutterctl.h"

AVR_MCU(8000000, "attiny84");
AVR_MCU_VOLTAGES(3300, 3300, 3300);
AVR_MCU_SIMAVR_CONSOLE(&GPIOR0);

static timer shutterTimer;

static void shutterTimeout(const event *ev, void *data)
{
    shutter_stop();
}

static void pinChanged(const event *ev, void *data)
{
    if ((ev->data.pinchange & EV_PC_UP_HI
	    || ev->data.pinchange & EV_PC_DOWN_HI)
	    && timer_ticks(shutterTimer) < 98)
    {
	timer_stop(shutterTimer);
	shutter_stop();
    }
    else if (ev->data.pinchange & EV_PC_UP_LO)
    {
	if (timer_ticks(shutterTimer))
	{
	    timer_stop(shutterTimer);
	    shutter_stop();
	}
	else
	{
	    timer_start(shutterTimer, 100);
	    shutter_up();
	}
    }
    else if (ev->data.pinchange & EV_PC_DOWN_LO)
    {
	if (timer_ticks(shutterTimer))
	{
	    timer_stop(shutterTimer);
	    shutter_stop();
	}
	else
	{
	    timer_start(shutterTimer, 100);
	    shutter_down();
	}
    }
}

int main(void)
{
    DDRA = 0xc0;
    PORTA = 0x3f;
    DDRB = 0x00;
    PORTB = 0x0f;

    eepdata_init();
    event_init();
    timer_init();

    shutterTimer = timer_create(shutterTimeout, 0);
    event_onPinchange(pinChanged, 0);

    event_loop();
}

