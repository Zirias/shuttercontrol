#ifndef SCTL_CONTROLLER_H
#define SCTL_CONTROLLER_H

#include "busclient.h"

class Bus;
class BusClock;
class QTimer;

class Controller: public BusClient
{
    Q_OBJECT

    public:
	Controller();
	virtual ~Controller();

    public slots:
	virtual void readBus(int lines);
	void up();
	void down();
	void setClock(const BusClock *clock);
	void setAddr(int addr);

    private slots:
	void step();

    private:
	void startAction(int action);

    protected:
	virtual void connected();
	
    private:
	QTimer *timer;
	bool active;
	const BusClock *clock;
	int addr, addrSet;
};

#endif
