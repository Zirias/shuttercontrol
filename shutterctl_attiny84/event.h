#ifndef EVENT_H
#define EVENT_H

#include "common.h"

/* Maximum queued up events, if more events stay unhandled, the buffer is
 * cleared and events are lost */
#define EVQUEUESIZE 16

/* Number of event types. Must be increased when using EV_CUSTOM or defining
 * own EV_* constants in order to reserve space for registered handlers */
#define NUMEVTYPES 3

/* Maximum number of handlers registered per event type */
#define MAXEVHANDLERS 4

typedef struct event event;
typedef enum ev_type
{
    EV_TICK	    = 0,
    EV_PINCHANGE    = 1,
    EV_TIMER	    = 2,
    EV_CUSTOM	    = 3
} ev_type;

struct event
{
    ev_type type;
    uint8_t data;
};

typedef void (*ev_handler)(const event *ev, void *data);
typedef BOOL (*ev_filter)(const event *ev, ev_handler handler, void *data);

void event_init(void);

void event_raise(ev_type type, uint8_t ev_data);

BOOL event_register(ev_type type, ev_handler handler,
	ev_filter filter, void *data);
BOOL event_unregister(ev_type type, ev_handler handler, void *data);

void event_loop(void) __attribute__((__noreturn__));

#endif
