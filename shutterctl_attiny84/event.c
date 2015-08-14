#include "event.h"

#include <string.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/io.h>

static event equeue[EVQUEUESIZE];
volatile static uint8_t eqhead = 0;
static uint8_t eqtail = 0;

static uint8_t ticksEnabled = 0;

typedef struct evhdl_record
{
    ev_handler handler;
    ev_filter filter;
    void *data;
} evhdl_record;

typedef struct evhdl_vector
{
    uint8_t nhandlers;
    evhdl_record handlers[MAXEVHANDLERS];
} evhdl_vector;

static evhdl_vector handlervector[NUMEVTYPES];

void event_init(void)
{
    /* configure interrupts: */
    GIMSK |= _BV(PCIE1);
    PCMSK1 |= _BV(PCINT8) | _BV(PCINT9) | _BV(PCINT10);
    TIMSK1 |= _BV(OCIE1A);

    /* power management, disable unused stuff: */
    PRR |= _BV(PRTIM1) | _BV(PRTIM0) | _BV(PRUSI) | _BV(PRADC);
    MCUCR &= ~(_BV(BODS) | _BV(BODSE));
    MCUCR |= _BV(BODS);
    MCUCR &= ~_BV(BODSE);
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
}

void event_raise(ev_type type, uint8_t ev_data)
{
    cli();
    equeue[eqhead].type = type;
    equeue[eqhead].data = ev_data;
    if (++eqhead == EVQUEUESIZE) eqhead = 0;
    sei();
}

static void enableTicks(void)
{
    if (ticksEnabled++) return;
    PRR &= ~_BV(PRTIM1);
    GTCCR |= _BV(TSM) | _BV(PSR10);
//  TCCR1B = _BV(WGM12) | _BV(CS10) | _BV(CS11); /* CTC, 8MHz / 64 */
    TCCR1B = _BV(WGM12) | _BV(CS11); /* CTC, 1MHz / 8 */
    OCR1A = 12500; /* 100 ms */
    TCNT1 = 0;
    GTCCR &= ~_BV(TSM);
}

static void disableTicks(void)
{
    if (ticksEnabled && --ticksEnabled) return;
    GTCCR |= _BV(TSM) | _BV(PSR10);
    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    GTCCR &= ~_BV(TSM);
    PRR |= _BV(PRTIM1);
}

BOOL event_register(ev_type type, ev_handler handler,
	ev_filter filter, void *data)
{
    for (uint8_t i = 0; i < MAXEVHANDLERS; ++i)
    {
	if (handlervector[type].handlers[i].handler)
	{
	    if (handlervector[type].handlers[i].handler == handler
		    && handlervector[type].handlers[i].data == data)
	    {
		handlervector[type].handlers[i].filter = filter;
		return TRUE;
	    }
	    continue;
	}
	handlervector[type].handlers[i].handler = handler;
	handlervector[type].handlers[i].filter = filter;
	handlervector[type].handlers[i].data = data;
	++handlervector[type].nhandlers;
	if (type == EV_TICK) enableTicks();
	return TRUE;
    }
    return FALSE;
}

BOOL event_unregister(ev_type type, ev_handler handler, void *data)
{
    for (uint8_t i = 0; i < MAXEVHANDLERS; ++i)
    {
	if (handlervector[type].handlers[i].handler != handler
		|| handlervector[type].handlers[i].data != data)
	{
	    continue;
	}
	handlervector[type].handlers[i].handler = 0;
	--handlervector[type].nhandlers;
	if (type == EV_TICK) disableTicks();
	return TRUE;
    }
    return FALSE;
}

ISR(PCINT1_vect)
{
    uint8_t pins = PINB;
    equeue[eqhead].type = EV_PINCHANGE;
    equeue[eqhead].data = pins;
    if (++eqhead == EVQUEUESIZE) eqhead = 0;
}

ISR(TIM1_COMPA_vect)
{
    equeue[eqhead].type = EV_TICK;
    if (++eqhead == EVQUEUESIZE) eqhead = 0;
}

void event_loop(void)
{
    sei();
    while (1)
    {
	/* run event queue */
	while (eqtail != eqhead)
	{
	    event *ev = &(equeue[eqtail]);
	    evhdl_vector *v = &(handlervector[ev->type]);
	    if (v->nhandlers)
	    {
		for (uint8_t i = 0; i < MAXEVHANDLERS; ++i)
		{
		    if (!(v->handlers[i].handler)) continue;
		    if (!(v->handlers[i].filter) ||
			    v->handlers[i].filter(ev,
				v->handlers[i].handler, v->handlers[i].data))
		    {
			v->handlers[i].handler(ev, v->handlers[i].data);
		    }
		}
	    }
	    if (++eqtail == EVQUEUESIZE) eqtail = 0;
	}

	/* sleep if no more events are pending and no timer tick enabled */
	if (!ticksEnabled) __asm__ volatile("sleep");
    }
    UNREACHABLE();
}

