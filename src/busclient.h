#ifndef SCTL_BUSCLIENT_H
#define SCTL_BUSCLIENT_H

#include <QObject>

class Bus;
class BusConnector;

class BusClient: public QObject
{
    Q_OBJECT

    public:
	BusClient();
	BusConnector *connector();

    public slots:
	void connectBus(Bus *bus);
	virtual void readBus(int lines) = 0;
	
    protected:
	virtual void connected() = 0;

    signals:
	void writeBus(int lines);
	void setDirections(int lines);

    private:
	BusConnector *conn;
};

#endif
