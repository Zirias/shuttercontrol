#ifndef SCTL_CONTROLLER_H
#define SCTL_CONTROLLER_H

#include "busclient.h"

class Bus;
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

    private slots:
	void resetBus();

    protected:
	virtual void connected();
	
    private:
	QTimer *timer;
};

#endif
