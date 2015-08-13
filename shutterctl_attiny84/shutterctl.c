#include "shutterctl.h"

#include "timer.h"
#include <avr/io.h>

#define shutter_stop() do { PORTA &= ~(_BV(PA6) | _BV(PA7)); } while (0)
#define shutter_up() do { shutter_stop(); PORTA |= _BV(PA6); } while (0)
#define shutter_down() do { shutter_stop(); PORTA |= _BV(PA7); } while (0)

static timer shutterTimer;
static uint8_t maxpos = 100;
static uint8_t pos = 0xff;

#define STOPPED	    0x00
#define UP	    0x10
#define DOWN	    0x20
#define AUTOSTOP    0x80

static uint8_t state = 0;
static uint8_t initticks = 0;

static void shutterTimeout(const event *ev, void *data)
{
    if (state & UP)
    {
	pos = maxpos;
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
}

void shutterctl_stop(shutterctl_prio prio)
{
    uint8_t ticks;
    uint8_t elapsed;

    if (!(state & 0xf0)) return;
    if (prio >= (state & 0xf))
    {
	shutter_stop();
	ticks = timer_ticks(shutterTimer);
	elapsed = initticks - ticks;
	if (state & UP)
	{
	    if (ticks)
	    {
		timer_stop(shutterTimer);
		pos += (elapsed > maxpos ? maxpos : elapsed);
	    }
	    else pos = maxpos;
	}
	else if (state & DOWN)
	{
	    if (ticks)
	    {
		timer_stop(shutterTimer);
		pos -= (elapsed > maxpos ? maxpos : elapsed);
	    }
	    else pos = 0;
	}
	state = 0;
    }
}

void shutterctl_up(shutterctl_prio prio, BOOL autostop)
{
    if (prio >= (state & 0xf))
    {
	if (pos == INVALID_POS) pos = 0;
	initticks = maxpos - pos + 10;
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
	if (pos == INVALID_POS) pos = maxpos;
	initticks = pos + 10;
	shutter_down();
	state = prio | DOWN;
	if (autostop) state |= AUTOSTOP;
	timer_start(shutterTimer, initticks);
    }
}

BOOL shutterctl_isactive(void)
{
    return !!(state & 0xf0);
}

uint8_t shutterctl_pos(void)
{
    return pos;
}

uint8_t shutterctl_maxpos(void)
{
    return maxpos;
}

