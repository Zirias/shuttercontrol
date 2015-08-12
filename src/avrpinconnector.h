#ifndef SCTL_AVRPINCONNECTOR_H
#define SCTL_AVRPINCONNECTOR_H

#include <QObject>
#include <sim_irq.h>

struct avr_t;

class AvrPinConnector: public QObject
{
    Q_OBJECT
    Q_ENUMS(SimIrq)

    public:
	enum SimIrq
	{
	    IRQ_PINS_OUT0,
	    IRQ_PINS_OUT1,
	    IRQ_PINS_IN0,
	    IRQ_PINS_IN1,
	    IRQ_DDR_0,
	    IRQ_DDR_1,
	    IRQ_COUNT
	};
	AvrPinConnector(avr_t *avr);

    private:
	avr_t *avr;
	avr_irq_t *irq;

	void handleIrq(avr_irq_t *irq, uint32_t value);
	static void handleIrqCallback(
		avr_irq_t *irq, uint32_t value, void *param);
};

#endif
