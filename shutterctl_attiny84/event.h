#ifndef EVENT_H
#define EVENT_H

#include "common.h"

typedef struct event event;
typedef void (*ev_handler)(const event *ev, void *data);
typedef BOOL (*ev_filter)(const event *ev);

#include "timer.h"

/* ticks in 100ms since last global timer start */
extern volatile uint8_t clockticks;

typedef enum ev_type
{
    EV_CUSTOM = 0,
    EV_PINCHANGE,
    EV_TICK,
    EV_TIMER
} ev_type;

struct event
{
    ev_type type;
    uint8_t data;
};

void event_init(void);
void event_enableTicks(void);
void event_disableTicks(void);
event *event_create(void);
BOOL event_register(ev_filter filter, ev_handler handler, void *data);
BOOL event_unregister(ev_handler handler, void *data);
BOOL event_onPinchange(ev_handler handler, void *data);
BOOL event_onTick(ev_handler handler, void *data);

void event_loop(void) __attribute__((__noreturn__));

#endif
