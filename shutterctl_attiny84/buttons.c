#include "buttons.h"

#include "event.h"
#include "timer.h"
#include "shutterctl.h"

#define UP_PIN	    0x01
#define DOWN_PIN    0x02
#define MANUAL	    0x08
#define UP_REQ	    0x10
#define DOWN_REQ    0x20
#define STOP_REQ    0x40
#define DEBOUNCE    0x80
#define PINMASK	    0x03
#define REQ_MASK    0x0f

#define DEBOUNCE_TICKS	8
#define MANUAL_TICKS	(30 - DEBOUNCE_TICKS)

static uint8_t state = PINMASK;

static timer buttonTimer;

static void buttonTimeout(const event *ev, void *data)
{
    if (state & DEBOUNCE)
    {
	state &= ~DEBOUNCE;
	if (state & STOP_REQ)
	{
	    state &= ~STOP_REQ;
	}
	else if (state & UP_REQ)
	{
	    if (!(state & UP_PIN))
	    {
		timer_start(buttonTimer, MANUAL_TICKS);
	    }
	    else
	    {
		state &= ~UP_REQ;
	    }
	}
	else if (state & DOWN_REQ)
	{
	    if (!(state & DOWN_PIN))
	    {
		timer_start(buttonTimer, MANUAL_TICKS);
	    }
	    else
	    {
		state &= ~DOWN_REQ;
	    }
	}
    }
    else if (state & UP_REQ && !(state & UP_PIN))
    {
	shutterctl_up(PRIO_MANUAL, FALSE);
	state &= ~UP_REQ;
	state |= MANUAL;
    }
    else if (state & DOWN_REQ && !(state & DOWN_PIN))
    {
	shutterctl_down(PRIO_MANUAL, FALSE);
	state &= ~DOWN_REQ;
	state |= MANUAL;
    }

}

static void pinChanged(const event *ev, void *data)
{
    uint8_t ticks = timer_ticks(buttonTimer);
    BOOL edge = FALSE;

    if ((ev->data & UP_PIN) && !(state & UP_PIN))
    {
	state |= UP_PIN;
    }
    else if ((ev->data & DOWN_PIN) && !(state & DOWN_PIN))
    {
	state |= DOWN_PIN;
    }
    else if (!(ev->data & UP_PIN) && (state & UP_PIN))
    {
	state &= ~UP_PIN;
	edge = TRUE;
    }
    else if (!(ev->data & DOWN_PIN) && (state & DOWN_PIN))
    {
	state &= ~DOWN_PIN;
	edge = TRUE;
    }

    if (state & DEBOUNCE) return;

    timer_stop(buttonTimer);
    state &= REQ_MASK;

    if (edge && !ticks)
    {
	if (shutterctl_calibrating())
	{
	    shutterctl_calibrate();
	}
	else if (shutterctl_isactive())
	{
	    shutterctl_stop(PRIO_MANUAL);
	    state |= STOP_REQ;
	}
	else if (!(state & UP_PIN))
	{
	    shutterctl_up(PRIO_MANUAL, TRUE);
	    state |= UP_REQ;
	}
	else if (!(state & DOWN_PIN))
	{
	    shutterctl_down(PRIO_MANUAL, TRUE);
	    state |= DOWN_REQ;
	}
	state |= DEBOUNCE;
	timer_start(buttonTimer, DEBOUNCE_TICKS);
    }
    else if (!edge && (state & MANUAL))
    {
	shutterctl_stop(PRIO_MANUAL);
	state &= ~MANUAL;
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

