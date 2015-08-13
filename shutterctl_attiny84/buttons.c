#include "buttons.h"

#include "event.h"
#include "timer.h"
#include "shutterctl.h"

static timer buttonTimer;
static uint8_t up;

static void buttonTimeout(const event *ev, void *data)
{
    if (up) shutterctl_up(PRIO_MANUAL, FALSE);
    else shutterctl_down(PRIO_MANUAL, FALSE);
}

static void pinChanged(const event *ev, void *data)
{
    uint8_t ticks = timer_ticks(buttonTimer);

    if (ev->data.pinchange & EV_PC_UP_HI)
    {
	if (ticks)
	{
	    if (ticks < 2) /* minimum 1 tick lo */
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
	if (ticks)
	{
	    if (ticks < 2) /* minimum 1 tick lo */
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
	if (shutterctl_isactive())
	{
	    shutterctl_stop(PRIO_MANUAL);
	}
	else
	{
	    up = 1;
	    if (!ticks) timer_start(buttonTimer, 2);
	}
    }
    else if (ev->data.pinchange & EV_PC_DOWN_LO)
    {
	if (shutterctl_isactive())
	{
	    shutterctl_stop(PRIO_MANUAL);
	}
	else
	{
	    up = 0;
	    if (!ticks) timer_start(buttonTimer, 2);
	}
    }
}

void buttons_init(void)
{
    event_onPinchange(pinChanged, 0);
    buttonTimer = timer_create(buttonTimeout, 0);
}
