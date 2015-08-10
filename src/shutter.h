#ifndef SCTL_SHUTTER_H
#define SCTL_SHUTTER_H

#include "busclient.h"

class Shutter: public BusClient
{
    Q_OBJECT

    public:
	Shutter(int address);

    public slots:
	virtual void readBus(int lines);
	void p_up();
	void r_up();
	void p_down();
	void r_down();
    
    protected:
	virtual void connected();
};

#endif
