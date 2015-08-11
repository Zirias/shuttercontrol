#ifndef SCTL_CONTROLLER_H
#define SCTL_CONTROLLER_H

#include "busclient.h"
#include <QQueue>

class Bus;
class BusClock;
class QTimer;
class ControllerAction;

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

    protected:
	virtual void connected();
	
    private slots:
	void step();

    private:
	void startAction(ControllerAction *act);

	QTimer *timer;
	ControllerAction *action;
	QQueue<ControllerAction *> pending;
	const BusClock *clock;
	int addr;
};

#endif
