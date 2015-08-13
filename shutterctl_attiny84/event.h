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

typedef enum ev_pinchange
{
    EV_PC_NONE = (1 << 0),
    EV_PC_BUSCLK_HI = (1 << 1),
    EV_PC_BUSCLK_LO = (1 << 2),
    EV_PC_UP_HI = (1 << 3),
    EV_PC_UP_LO = (1 << 4),
    EV_PC_DOWN_HI = (1 << 5),
    EV_PC_DOWN_LO = (1 << 6)
} ev_pinchange;

typedef uint8_t ev_tick;
typedef timer ev_timer;

struct event
{
    ev_type type;
    union
    {
	uint8_t custom;
	ev_pinchange pinchange;
	ev_tick ticks;
	ev_timer timer;
    } data;
};

void event_init(void);
void event_enableTicks(void);
void event_disableTicks(void);
event *event_create(void);
void event_dispatch(event *ev);
BOOL event_register(ev_filter filter, ev_handler handler, void *data);
BOOL event_unregister(ev_handler handler, void *data);
BOOL event_onPinchange(ev_handler handler, void *data);
BOOL event_onTick(ev_handler handler, void *data);

void event_loop(void) __attribute__((__noreturn__));

#endif
