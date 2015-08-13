#include "timer.h"

#include <string.h>

#define MAX_TIMERS 4

typedef struct timer_record
{
    uint8_t ticks;
    ev_handler timeout;
    void *data;
} timer_record;

static timer_record timers[MAX_TIMERS];

void timer_init(void)
{
    memset(timers, 0, sizeof(timers));
}

timer timer_create(ev_handler timeout, void *data)
{
    for (uint8_t i = 0; i < MAX_TIMERS; ++i)
    {
	if (timers[i].timeout) continue;
	timers[i].timeout = timeout;
	timers[i].data = data;
	return i;
    }
    return INVALID_TIMER;
}

static void timerTick(const event *ev, void *data)
{
    timer t = (timer)(int)data;
    if (ev->data.ticks > timers[t].ticks)
    {
	event_unregister(timerTick, data);
	timers[t].ticks = 0;
	event *timerEvent = event_create();
	timerEvent->type = EV_TIMER;
	timerEvent->data.timer = t;
	timers[t].timeout(timerEvent, timers[t].data);
    }
    timers[t].ticks -= ev->data.ticks;
}

void timer_start(timer timer, uint8_t ticks)
{
    timers[timer].ticks = clockticks + ticks;
    event_onTick(timerTick, (void *)(int)timer);
}

uint8_t timer_ticks(timer timer)
{
    return timers[timer].ticks;
}

void timer_stop(timer timer)
{
    event_unregister(timerTick, (void *)(int)timer);
    timers[timer].ticks = 0;
}

void timer_destroy(timer timer)
{
    timer_stop(timer);
    timers[timer].timeout = 0;
}

