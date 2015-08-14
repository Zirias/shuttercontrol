#include "timer.h"

#include <string.h>

#define MAX_TIMERS 4

typedef struct timer_record
{
    uint16_t ticks;
    ev_handler timeout;
    void *data;
} timer_record;

static timer_record timers[MAX_TIMERS];

void timer_init(void)
{
    memset(timers, 0, sizeof(timers));
}

static BOOL timerFilter(const event *ev, ev_handler handler, void *data)
{
    timer t = (timer)(int)ev->data;
    return (timers[t].timeout == handler && timers[t].data == data);
}

timer timer_create(ev_handler timeout, void *data)
{
    for (uint8_t i = 0; i < MAX_TIMERS; ++i)
    {
	if (timers[i].timeout) continue;
	timers[i].timeout = timeout;
	timers[i].data = data;
	event_register(EV_TIMER, timeout, timerFilter, data);
	return i;
    }
    return INVALID_TIMER;
}

static void timerTick(const event *ev, void *data)
{
    timer t = (timer)(int)data;
    if (!--timers[t].ticks)
    {
	event_unregister(EV_TICK, timerTick, data);
	event_raise(EV_TIMER, t);
    }
}

void timer_start(timer timer, uint16_t ticks)
{
    event_register(EV_TICK, timerTick, 0, (void *)(int)timer);
    timers[timer].ticks = ticks;
}

uint16_t timer_ticks(timer timer)
{
    return timers[timer].ticks;
}

void timer_stop(timer timer)
{
    event_unregister(EV_TICK, timerTick, (void *)(int)timer);
    timers[timer].ticks = 0;
}

void timer_destroy(timer timer)
{
    timer_stop(timer);
    event_unregister(EV_TIMER, timers[timer].timeout, timers[timer].data);
    timers[timer].timeout = 0;
}

