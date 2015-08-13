#include "avrpinconnector.h"

#include <sim_avr.h>
#include <avr_ioport.h>

static const char *irqnames[] =
{
    "PORTA_OUT",
    "PORTB_OUT",
    "PORTA_IN0",
    "PORTA_IN1",
    "PORTA_IN2",
    "PORTA_IN3",
    "PORTA_IN4",
    "PORTA_IN5",
    "PORTA_IN6",
    "PORTA_IN7",
    "PORTB_IN0",
    "PORTB_IN1",
    "PORTB_IN2",
    "PORTB_IN3",
    "PORTB_IN4",
    "PORTB_IN5",
    "PORTB_IN6",
    "PORTB_IN7",
    "DDRA",
    "DDRB"
};

AvrPinConnector::AvrPinConnector(avr_t *avr)
    : avr(avr), lines(0xff), direction(0), _up(false), _down(false)
{
    irq = avr_alloc_irq(&avr->irq_pool, 0, IRQ_COUNT, irqnames);   

    avr_irq_register_notify(irq + AvrPinConnector::PORTA_OUT,
	    &AvrPinConnector::handleIrqCallback, (void *)this);
    avr_irq_register_notify(irq + AvrPinConnector::PORTB_OUT,
	    &AvrPinConnector::handleIrqCallback, (void *)this);
    avr_irq_register_notify(irq + AvrPinConnector::DDRA,
	    &AvrPinConnector::handleIrqCallback, (void *)this);
    avr_irq_register_notify(irq + AvrPinConnector::DDRB,
	    &AvrPinConnector::handleIrqCallback, (void *)this);

    avr_connect_irq(
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN_ALL),
	    irq + AvrPinConnector::PORTA_OUT);
    avr_connect_irq(
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN_ALL),
	    irq + AvrPinConnector::PORTB_OUT);
    avr_connect_irq(
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_DIRECTION_ALL),
	    irq + AvrPinConnector::DDRA);
    avr_connect_irq(
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_DIRECTION_ALL),
	    irq + AvrPinConnector::DDRB);

    avr_connect_irq(irq + AvrPinConnector::PORTA_IN0,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN0));
    avr_connect_irq(irq + AvrPinConnector::PORTA_IN1,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN1));
    avr_connect_irq(irq + AvrPinConnector::PORTA_IN2,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN2));
    avr_connect_irq(irq + AvrPinConnector::PORTA_IN3,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN3));
    avr_connect_irq(irq + AvrPinConnector::PORTA_IN4,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN4));
    avr_connect_irq(irq + AvrPinConnector::PORTA_IN5,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN5));
    avr_connect_irq(irq + AvrPinConnector::PORTA_IN6,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN6));
    avr_connect_irq(irq + AvrPinConnector::PORTA_IN7,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('A'), IOPORT_IRQ_PIN7));

    avr_connect_irq(irq + AvrPinConnector::PORTB_IN0,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN0));
    avr_connect_irq(irq + AvrPinConnector::PORTB_IN1,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN1));
    avr_connect_irq(irq + AvrPinConnector::PORTB_IN2,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN2));
    avr_connect_irq(irq + AvrPinConnector::PORTB_IN3,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN3));
    avr_connect_irq(irq + AvrPinConnector::PORTB_IN4,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN4));
    avr_connect_irq(irq + AvrPinConnector::PORTB_IN5,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN5));
    avr_connect_irq(irq + AvrPinConnector::PORTB_IN6,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN6));
    avr_connect_irq(irq + AvrPinConnector::PORTB_IN7,
	    avr_io_getirq(avr,
		AVR_IOCTL_IOPORT_GETIRQ('B'), IOPORT_IRQ_PIN7));
}

static int port0tobus(int current, uint32_t port0)
{
    current &= 0xc0; /* port0 not connected to bus lines 6 (BC) and 7 (RS) */
    current |= (port0 & 0x3f); /* other pins mapped 1:1 */
    return current;
}

static int port1tobus(int current, uint32_t port1)
{
    current &= 0x3f; /* port1 connected to bus lines 6 and 7 */
    current |= ((port1 & 0xc) << 4);
    return current;
}

void AvrPinConnector::readBus(int lines)
{
    avr_raise_irq(irq + AvrPinConnector::PORTA_IN0, !!(lines & 1<<0));
    avr_raise_irq(irq + AvrPinConnector::PORTA_IN1, !!(lines & 1<<1));
    avr_raise_irq(irq + AvrPinConnector::PORTA_IN2, !!(lines & 1<<2));
    avr_raise_irq(irq + AvrPinConnector::PORTA_IN3, !!(lines & 1<<3));
    avr_raise_irq(irq + AvrPinConnector::PORTA_IN4, !!(lines & 1<<4));
    avr_raise_irq(irq + AvrPinConnector::PORTA_IN5, !!(lines & 1<<5));
    avr_raise_irq(irq + AvrPinConnector::PORTB_IN2, !!(lines & 1<<6));

    // TODO: reset line
}

void AvrPinConnector::p_up()
{
    avr_raise_irq(irq + AvrPinConnector::PORTB_IN0, 0);
}

void AvrPinConnector::r_up()
{
    avr_raise_irq(irq + AvrPinConnector::PORTB_IN0, 1);
}

void AvrPinConnector::p_down()
{
    avr_raise_irq(irq + AvrPinConnector::PORTB_IN1, 0);
}

void AvrPinConnector::r_down()
{
    avr_raise_irq(irq + AvrPinConnector::PORTB_IN1, 1);
}

void AvrPinConnector::handleIrq(avr_irq_t *irq, uint32_t value)
{
    int l;
    switch (irq->irq)
    {
	case AvrPinConnector::PORTA_OUT:
	    l = port0tobus(lines, value);
	    if (l != lines)
	    {
		lines = l;
		emit writeBus(lines);
	    }
	    if (_up && !(value & 0x40))
	    {
		_up = false;
		emit up(_up);
	    }
	    else if (!_up && (value & 0x40))
	    {
		_up = true;
		emit up(_up);
	    }
	    if (_down && !(value & 0x80))
	    {
		_down = false;
		emit down(_down);
	    }
	    else if (!_down && (value & 0x80))
	    {
		_down = true;
		emit down(_down);
	    }
	    break;

	case AvrPinConnector::PORTB_OUT:
	    l = port1tobus(lines, value);
	    if (l != lines)
	    {
		lines = l;
		emit writeBus(lines);
	    }
	    break;
	
	case AvrPinConnector::DDRA:
	    l = port0tobus(direction, value);
	    if (l != direction)
	    {
		direction = l;
		emit setDirections(direction);
	    }
	    break;

	case AvrPinConnector::DDRB:
	    l = port1tobus(direction, value);
	    if (l != direction)
	    {
		direction = l;
		emit setDirections(direction);
	    }
	    break;
    }
}

void AvrPinConnector::handleIrqCallback(
	avr_irq_t *irq, uint32_t value, void *param)
{
    ((AvrPinConnector *)(param))->handleIrq(irq, value);
}
