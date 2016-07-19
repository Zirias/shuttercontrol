#include "shutterctl.h"

#include "timer.h"
#include "eepdata.h"
#include <avr/io.h>

#define shutter_stop() do { PORTA &= ~(_BV(PA6) | _BV(PA7)); } while (0)
#define shutter_up() do { PORTA |= _BV(PA6); } while (0)
#define shutter_down() do { PORTA |= _BV(PA7); } while (0)

static timer shutterTimer;
static uint8_t pos = 63;

#define STOPPED	    0x00
#define UP	    0x10
#define DOWN	    0x20
#define AUTOSTOP    0x80

#define CAL_MAXTICKS 4000

typedef enum shutterctl_calstate
{
    CAL_NONE = 0,
    CAL_READY,
    CAL_INIT,
    CAL_DOWN,
    CAL_UP
} shutterctl_calstate;

static uint8_t state = 0;
static uint16_t initticks = 0;
static shutterctl_calstate calstate = CAL_NONE;

static void shutterTimeout(const event *ev, void *data)
{
    if (calstate)
    {
	state = 0;
	calstate = CAL_NONE;
    }
    if (state & UP)
    {
	pos = 63;
    }
    else if (state & DOWN)
    {
	pos = 0;
    }
    if (state & AUTOSTOP)
    {
	shutter_stop();
	state = 0;
    }
}

void shutterctl_init(void)
{
    shutterTimer = timer_create(shutterTimeout, 0);
    shutterctl_down(PRIO_MANUAL, 1);
}

static uint16_t stopAndReadTimer(void)
{
    if (!(state & 0xf0)) return 0;
    shutter_stop();
    uint16_t ticks = timer_ticks(shutterTimer);
    timer_stop(shutterTimer);
    return ticks ? initticks - ticks : MAXTICKS;
}

void shutterctl_stop(shutterctl_prio prio)
{
    uint16_t elapsed;

    if (prio >= (state & 0xf))
    {
	if ((elapsed = stopAndReadTimer()))
	{
	    if (state & UP)
	    {
		if (elapsed >= eep.ticks_up) pos = 63;
		else
		{
		    pos += (elapsed * 63) / eep.ticks_up;
		    if (pos > 63) pos = 63;
		}
	    }
	    else if (state & DOWN)
	    {
		if (elapsed >= eep.ticks_down) pos = 0;
		else
		{
		    pos -= (elapsed * 63) / eep.ticks_down;
		    if (pos > 63) pos = 0;
		}
	    }
	}
	state = 0;
    }
}

void shutterctl_up(shutterctl_prio prio, BOOL autostop)
{
    if (prio >= (state & 0xf))
    {
	shutterctl_stop(prio);
	initticks = ((uint16_t)(63 - pos) << 4) / 63 * eep.ticks_up / 16 + 150;
	shutter_up();
	state = prio | UP;
	if (autostop) state |= AUTOSTOP;
	timer_start(shutterTimer, initticks);
    }
}

void shutterctl_down(shutterctl_prio prio, BOOL autostop)
{
    if (prio >= (state & 0xf))
    {
	shutterctl_stop(prio);
	initticks = ((uint16_t)pos << 4 ) / 63 * eep.ticks_down / 16 + 150;
	shutter_down();
	state = prio | DOWN;
	if (autostop) state |= AUTOSTOP;
	timer_start(shutterTimer, initticks);
    }
}

void shutterctl_calibrate(void)
{
    static uint16_t down_ticks;

    shutterctl_prio prio = PRIO_CAL;
    if (prio >= (state & 0xf))
    {
	switch (calstate)
	{
	    case CAL_NONE:
		shutterctl_stop(prio);
		state = prio;
		calstate = CAL_READY;
		break;
	    case CAL_READY:
		shutter_stop();
		shutter_up();
		state = prio | UP | AUTOSTOP;
		timer_start(shutterTimer, CAL_MAXTICKS);
		calstate = CAL_INIT;
		break;
	    case CAL_INIT:
		timer_stop(shutterTimer);
		shutter_stop();
		shutter_down();
		state = prio | DOWN | AUTOSTOP;
		initticks = CAL_MAXTICKS;
		timer_start(shutterTimer, initticks);
		calstate = CAL_DOWN;
		break;
	    case CAL_DOWN:
		down_ticks = stopAndReadTimer();
		shutter_up();
		state = prio | UP | AUTOSTOP;
		initticks = CAL_MAXTICKS;
		timer_start(shutterTimer, initticks);
		calstate = CAL_UP;
		break;
	    case CAL_UP:
		eep.ticks_up = stopAndReadTimer();
		state = prio;
		eep.ticks_down = down_ticks;
		pos = 63;
		timer_start(shutterTimer, 20);
		eepdata_save();
		break;
	}
    }
}

BOOL shutterctl_isactive(void)
{
    return !!(state & 0xf0);
}

BOOL shutterctl_calibrating(void)
{
    return !!calstate;
}

uint8_t shutterctl_pos(void)
{
    return (pos>>1) & 0x1f;
}

