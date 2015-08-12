#include "avrpinconnector.h"

#include <sim_avr.h>

AvrPinConnector::AvrPinConnector(avr_t *avr)
    : avr(avr)
{
    irq = avr_alloc_irq(&avr->irq_pool, 0, IRQ_COUNT, 0);   

    avr_irq_register_notify(irq + AvrPinConnector::IRQ_PINS_OUT0,
	    &AvrPinConnector::handleIrqCallback, (void *)this);
    avr_irq_register_notify(irq + AvrPinConnector::IRQ_PINS_OUT1,
	    &AvrPinConnector::handleIrqCallback, (void *)this);
    avr_irq_register_notify(irq + AvrPinConnector::IRQ_DDR_0,
	    &AvrPinConnector::handleIrqCallback, (void *)this);
    avr_irq_register_notify(irq + AvrPinConnector::IRQ_DDR_1,
	    &AvrPinConnector::handleIrqCallback, (void *)this);
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

static uint32_t bustoport0(uint32_t current, int lines)
{
    current &= 0xc0;
    current |= (lines & 0x3f);
    return current;
}

static uint32_t bustoport1(uint32_t current, int lines)
{
    current &= 0x3;
    current |= ((lines & 0xc0) >> 4);
}

void AvrPinConnector::handleIrq(avr_irq_t *irq, uint32_t value)
{
}

void AvrPinConnector::handleIrqCallback(
	avr_irq_t *irq, uint32_t value, void *param)
{
    ((AvrPinConnector *)(param))->handleIrq(irq, value);
}
