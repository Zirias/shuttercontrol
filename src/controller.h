#ifndef SCTL_CONTROLLER_H
#define SCTL_CONTROLLER_H

#include <QObject>

class Bus;
class QTimer;

class Controller: public QObject
{
    Q_OBJECT

    public:
	Controller();

    public slots:
	void readBus(const Bus *bus);
	void up();
	void down();

    private slots:
	void run();
	void resetBus();

    signals:
	void writeBus(int value);
	void finished();

    private:
	QTimer *timer;
};

#endif
