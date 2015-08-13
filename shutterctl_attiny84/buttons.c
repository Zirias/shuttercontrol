#include "buttons.h"

#include "event.h"
#include "timer.h"
#include "shutterctl.h"

static timer buttonTimer;

#define UP_REQ	    1<<0
#define DOWN_REQ    1<<1
#define STOP_REQ    1<<3
#define UP_LO	    1<<4
#define DOWN_LO	    1<<5

static uint8_t state = 0;

static void buttonTimeout(const event *ev, void *data)
{
    if ((state & UP_REQ) && (state & UP_LO))
    {
	state &= ~UP_REQ;
	shutterctl_up(PRIO_MANUAL, FALSE);
    }
    else if ((state & DOWN_REQ) && (state & DOWN_LO))
    {
	state &= ~DOWN_REQ;
	shutterctl_down(PRIO_MANUAL, FALSE);
    }
}

static void pinChanged(const event *ev, void *data)
{
    uint8_t ticks = timer_ticks(buttonTimer);

    if (ev->data.pinchange & EV_PC_UP_HI)
    {
	state &= ~UP_LO;
	if (ticks)
	{
	    if (ticks < 3) /* minimum 1 tick lo */
	    {
		timer_stop(buttonTimer);
		shutterctl_up(PRIO_MANUAL, TRUE);
	    }
	}
	else
	{
	    shutterctl_stop(PRIO_MANUAL);
	}
    }
    else if (ev->data.pinchange & EV_PC_DOWN_HI)
    {
	state &= ~DOWN_LO;
	if (ticks)
	{
	    if (ticks < 3) /* minimum 1 tick lo */
	    {
		timer_stop(buttonTimer);
		shutterctl_down(PRIO_MANUAL, TRUE);
	    }
	}
	else
	{
	    shutterctl_stop(PRIO_MANUAL);
	}
    }
    else if (ev->data.pinchange & EV_PC_UP_LO)
    {
	state |= UP_LO;
	if (shutterctl_isactive())
	{
	    shutterctl_stop(PRIO_MANUAL);
	}
	else
	{
	    state |= UP_REQ;
	    if (!ticks) timer_start(buttonTimer, 3);
	}
    }
    else if (ev->data.pinchange & EV_PC_DOWN_LO)
    {
	state |= DOWN_LO;
	if (shutterctl_isactive())
	{
	    shutterctl_stop(PRIO_MANUAL);
	}
	else
	{
	    state |= DOWN_REQ;
	    if (!ticks) timer_start(buttonTimer, 3);
	}
    }
}

static BOOL pinChangedFilter(const event *ev)
{
    return (ev->type == EV_PINCHANGE &&
	    (ev->data.pinchange &
	     (EV_PC_UP_HI|EV_PC_UP_LO|EV_PC_DOWN_HI|EV_PC_DOWN_LO)));
}

void buttons_init(void)
{
    event_register(pinChangedFilter, pinChanged, 0);
    buttonTimer = timer_create(buttonTimeout, 0);
}
