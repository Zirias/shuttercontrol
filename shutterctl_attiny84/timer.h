#ifndef TIMER_H
#define TIMER_H

#include "common.h"

typedef uint8_t timer;
#define INVALID_TIMER 0xff

#include "event.h"

void timer_init(void);
timer timer_create(ev_handler timeout, void *data);
void timer_start(timer timer, uint8_t ticks);
uint8_t timer_ticks(timer timer);
void timer_stop(timer timer);
void timer_destroy(timer timer);

#endif
