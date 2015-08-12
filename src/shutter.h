#ifndef SCTL_SHUTTER_H
#define SCTL_SHUTTER_H

#include "busclient.h"

class AvrContainer;

class Shutter: public BusClient
{
    Q_OBJECT

    public:
	Shutter(int address);
	virtual ~Shutter();

    public slots:
	virtual void readBus(int lines);
	void p_up();
	void r_up();
	void p_down();
	void r_down();
    
    private slots:
	void proxy_stateChanged(const char *state);
	void proxy_writeBus(int lines);
	void proxy_setDirections(int lines);
	void proxy_up(bool active);
	void proxy_down(bool active);
	void avr_started();

    signals:
	void up(bool active);
	void down(bool active);
	void stateChanged(const char *state);
	void proxy_readBus(int lines);
	void proxy_p_up();
	void proxy_r_up();
	void proxy_p_down();
	void proxy_r_down();

    protected:
	virtual void connected();
	AvrContainer *chip;
};

#endif
