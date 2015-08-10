#ifndef SCTL_BUSCONNECTOR_H
#define SCTL_BUSCONNECTOR_H

#include <QObject>

#include "bus.h"

class BusConnector: public QObject
{
    Q_OBJECT

    public:
	BusConnector();
	int getDirections() const;
	int read() const;

    public slots:
	void setDirections(int dir);
	void write(int lines);
	void readBus(const Bus *bus);

    signals:
	void changed(int dir);
	void readChanged(int lines);
	void writeBus(int lines);

    private:
	Bus::Lines directions;
	Bus::Lines readLines;
};

#endif
