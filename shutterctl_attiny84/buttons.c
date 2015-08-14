#include "buttons.h"

#include "event.h"
#include "timer.h"
#include "shutterctl.h"

static timer buttonTimer;

#define UP_PIN	    0x01
#define DOWN_PIN    0x02
#define UP_REQ	    0x10
#define DOWN_REQ    0x20
#define PINMASK	    0x03

static uint8_t state = 0;

static void buttonTimeout(const event *ev, void *data)
{
    if ((state & UP_REQ) && (~state & UP_PIN))
    {
	state &= ~UP_REQ;
	shutterctl_up(PRIO_MANUAL, FALSE);
    }
    else if ((state & DOWN_REQ) && (~state & DOWN_PIN))
    {
	state &= ~DOWN_REQ;
	shutterctl_down(PRIO_MANUAL, FALSE);
    }
}

static void pinChanged(const event *ev, void *data)
{
    uint8_t ticks = timer_ticks(buttonTimer);

    if ((ev->data & UP_PIN) && !(state & UP_PIN))
    {
	state |= UP_PIN;
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
    else if ((ev->data & DOWN_PIN) && !(state & DOWN_PIN))
    {
	state |= DOWN_PIN;
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
    else if (!(ev->data & UP_PIN) && (state & UP_PIN))
    {
	state &= ~UP_PIN;
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
    else if (!(ev->data & DOWN_PIN) && (state & DOWN_PIN))
    {
	state &= ~DOWN_PIN;
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

static BOOL pinChangedFilter(const event *ev, ev_handler handler, void *data)
{
    return ((ev->data & PINMASK) != (state & PINMASK));
}

void buttons_init(void)
{
    event_register(EV_PINCHANGE, pinChanged, pinChangedFilter, 0);
    buttonTimer = timer_create(buttonTimeout, 0);
}

