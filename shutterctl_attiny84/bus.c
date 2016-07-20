#include "bus.h"

#include <avr/io.h>
#include "event.h"
#include "shutterctl.h"
#include "eepdata.h"

#define ATN	    0x20
#define DATAMASK    0x1f
#define CMD_STATUS  0x00
#define CMD_UP	    0x01
#define CMD_DOWN    0x02
#define CMD_STOP    0x03
#define CMD_CAL	    0x04

#define input() do { \
    PORTA |= _BV(PA0)|_BV(PA1)|_BV(PA2)|_BV(PA3)|_BV(PA4); \
    DDRA &= ~(_BV(PA0)|_BV(PA1)|_BV(PA2)|_BV(PA3)|_BV(PA4)); } while (0)
#define output() do { \
    DDRA |= _BV(PA0)|_BV(PA1)|_BV(PA2)|_BV(PA3)|_BV(PA4); \
    PORTA &= ~(_BV(PA0)|_BV(PA1)|_BV(PA2)|_BV(PA3)|_BV(PA4)); } while (0)

static uint8_t status = 0;

static void pinChanged(const event *ev, void *data)
{
    uint8_t busdata = PINA & 0x3f;

    if (status & ATN)
    {
	if ((status & DATAMASK) == DATAMASK)
	{
	    if (!(busdata & ATN))
	    {
		/* protocol error, ATN set while expecting command */
		status = 0;
		return;
	    }
	    busdata &= DATAMASK;
	    switch (busdata)
	    {
		case CMD_UP:
		    shutterctl_up(PRIO_BUS, TRUE);
		    status = 0;
		    return;
		case CMD_DOWN:
		    shutterctl_down(PRIO_BUS, TRUE);
		    status = 0;
		    return;
		case CMD_STOP:
		    shutterctl_stop(PRIO_BUS);
		    status = 0;
		    return;
		case CMD_STATUS:
		    status = ATN | CMD_STATUS;
		    return;
		case CMD_CAL:
		    shutterctl_calibrate();
		    status = 0;
		    return;
		default:
		    status = 0;
		    return;
	    }
	}
	else
	{
	    switch (status & DATAMASK)
	    {
		case CMD_STATUS:
		    output();
		    PORTA |= shutterctl_pos();
		    return;
	    }
	}
    }
    else
    {
	if (busdata & ATN) return;
	if (!busdata || busdata == eep_address) status = ATN | DATAMASK;
    }
}

static BOOL pinChangedFilter(const event *ev, ev_handler handler, void *data)
{
    static uint8_t pin = 1;

    if (!(ev->data & 0x4))
    {
	input();
	pin = 0;
    }
    else if (!pin)
    {
	pin = 1;
	return TRUE;
    }
    return FALSE;
}

void bus_init(void)
{
    event_register(EV_PINCHANGE, pinChanged, pinChangedFilter, 0);
}
