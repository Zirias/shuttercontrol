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
	    PORTA_OUT,
	    PORTB_OUT,
	    PORTA_IN0,
	    PORTA_IN1,
	    PORTA_IN2,
	    PORTA_IN3,
	    PORTA_IN4,
	    PORTA_IN5,
	    PORTA_IN6,
	    PORTA_IN7,
	    PORTB_IN0,
	    PORTB_IN1,
	    PORTB_IN2,
	    PORTB_IN3,
	    PORTB_IN4,
	    PORTB_IN5,
	    PORTB_IN6,
	    PORTB_IN7,
	    DDRA,
	    DDRB,
	    IRQ_COUNT
	};
	AvrPinConnector(avr_t *avr);

    public slots:
	void readBus(int lines);
	void p_up();
	void r_up();
	void p_down();
	void r_down();

    signals:
	void writeBus(int lines);
	void setDirections(int lines);
	void up(bool active);
	void down(bool active);

    private:
	avr_t *avr;
	avr_irq_t *irq;
	int lines, direction;
	bool _up, _down;

	void handleIrq(avr_irq_t *irq, uint32_t value);
	static void handleIrqCallback(
		avr_irq_t *irq, uint32_t value, void *param);
};

#endif
