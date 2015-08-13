#include "event.h"

#include <string.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define EVBUFSIZE 8
#define MAXEVHANDLERS 8

volatile static uint8_t pinchanged = 0;
volatile uint8_t clockticks = 0;

static event events[EVBUFSIZE];
static uint8_t nextevent = 0;

static uint8_t enableTicks = 0;

typedef struct evhdl_record
{
    ev_handler handler;
    ev_filter filter;
    void *data;
} evhdl_record;

static evhdl_record handlers[MAXEVHANDLERS];

static uint8_t pins = 0xf;

ISR(PCINT1_vect)
{
    pinchanged = 1;
}

ISR(TIM1_COMPA_vect)
{
    ++clockticks;
}

static BOOL filterPinchange(const event *ev)
{
    return ev->type == EV_PINCHANGE;
}

static BOOL filterTick(const event *ev)
{
    return ev->type == EV_TICK;
}

void event_init(void)
{
    /* clean state: */
    memset(events, 0, sizeof(events));
    memset(handlers, 0, sizeof(handlers));

    /* configure interrupts: */
    GIMSK |= _BV(PCIE1);
    PCMSK1 |= _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10);
    TIMSK1 |= _BV(OCIE1A);

    /* disable all integrated peripherals: */
    PRR |= _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRUSI) | _BV(PRADC);
}

void event_enableTicks(void)
{
    if (enableTicks++) return;
    PRR &= ~_BV(PRTIM1);
    GTCCR |= _BV(TSM) | _BV(PSR10);
    TCCR1B |= _BV(WGM12) | _BV(CS10) | _BV(CS11); /* CTC, 8MHz / 64 */
    OCR1A = 12500; /* 100 ms */
    set_sleep_mode(SLEEP_MODE_IDLE);
    TCNT1 = 0;
    GTCCR &= ~_BV(TSM);
}

void event_disableTicks(void)
{
    if (--enableTicks) return;
    GTCCR |= _BV(TSM) | _BV(PSR10);
    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    GTCCR &= ~_BV(TSM);
    PRR |= _BV(PRTIM1);
}

event *event_create(void)
{
    event *ev = &(events[nextevent]);
    ++nextevent;
    if (nextevent == EVBUFSIZE) nextevent = 0;
    return ev;
}

BOOL event_register(ev_filter filter, ev_handler handler, void *data)
{
    uint8_t ssave = SREG;
    cli();
    for (uint8_t i = 0; i < MAXEVHANDLERS; ++i)
    {
	if (handlers[i].handler)
	{
	    if (handlers[i].handler == handler && handlers[i].data == data)
	    {
		SREG = ssave;
		return TRUE;
	    }
	    continue;
	}
	handlers[i].handler = handler;
	handlers[i].filter = filter;
	handlers[i].data = data;
	if (filter == filterTick) event_enableTicks();
	SREG = ssave;
	return TRUE;
    }
    SREG = ssave;
    return FALSE;
}

BOOL event_unregister(ev_handler handler, void *data)
{
    uint8_t ssave = SREG;
    cli();
    for (uint8_t i = 0; i < MAXEVHANDLERS; ++i)
    {
	if (handlers[i].handler != handler || handlers[i].data != data)
	{
	    continue;
	}
	handlers[i].handler = 0;
	if (handlers[i].filter == filterTick) event_disableTicks();
	SREG = ssave;
	return TRUE;
    }
    SREG = ssave;
    return FALSE;
}

BOOL event_onPinchange(ev_handler handler, void *data)
{
    return event_register(filterPinchange, handler, data);
}

BOOL event_onTick(ev_handler handler, void *data)
{
    return event_register(filterTick, handler, data);
}

void event_dispatch(event *ev)
{
    for (uint8_t i = 0; i < MAXEVHANDLERS; ++i)
    {
	if (!handlers[i].handler) continue;
	if (handlers[i].filter(ev))
	{
	    handlers[i].handler(ev, handlers[i].data);
	}
    }
}

static event *createPinchangeEvent(uint8_t newpins)
{
    event *ev = event_create();
    ev->type = EV_PINCHANGE;
    ev->data.pinchange = 0;
    if ((pins & _BV(0)) != (newpins & _BV(0)))
    {
	if (newpins & _BV(0)) ev->data.pinchange |= EV_PC_UP_HI;
	else ev->data.pinchange |= EV_PC_UP_LO;
    }
    if ((pins & _BV(1)) != (newpins & _BV(1)))
    {
	if (newpins & _BV(1)) ev->data.pinchange |= EV_PC_DOWN_HI;
	else ev->data.pinchange |= EV_PC_DOWN_LO;
    }
    if ((pins & _BV(2)) != (newpins & _BV(2)))
    {
	if (newpins & _BV(2)) ev->data.pinchange |= EV_PC_BUSCLK_HI;
	else ev->data.pinchange |= EV_PC_BUSCLK_LO;
    }
    return ev;
}

static event *createTickEvent(uint8_t oldticks)
{
    event *ev = event_create();
    ev->type = EV_TICK;
    ev->data.ticks = clockticks - oldticks;
    return ev;
}

void event_loop(void)
{
    uint8_t newpins = 0;
    uint8_t ticks = clockticks;
    event *ev;

    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sei();
    while (1)
    {
	while (pinchanged || clockticks != ticks)
	{
	    if (pinchanged)
	    {
		newpins = PINB;
		cli();
		ev = createPinchangeEvent(newpins);
		pinchanged = 0;
		pins = newpins;
		sei();
		event_dispatch(ev);
	    }
	    if (clockticks != ticks)
	    {
		cli();
		ev = createTickEvent(ticks);
		sei();
		ticks = clockticks;
		event_dispatch(ev);
	    }
	}
	__asm__ volatile("sleep");
    }
    UNREACHABLE();
}

