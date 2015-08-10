#ifndef SCTL_BUS_H
#define SCTL_BUS_H

#include <QObject>
#include <QFlags>

#include "buslineid.h"

class BusConnector;

class Bus: public QObject
{
    Q_OBJECT
    Q_FLAGS(Line Lines)

    public:
	enum Line
	{
	    A0 = 1 << BusLineId::A0,
	    A1 = 1 << BusLineId::A1,
	    A2 = 1 << BusLineId::A2,
	    A3 = 1 << BusLineId::A3,
	    D0 = 1 << BusLineId::D0,
	    D1 = 1 << BusLineId::D1,
	    D2 = 1 << BusLineId::D2,
	    BC = 1 << BusLineId::BC,
	    IRQ = 1 << BusLineId::IRQ
	};
	Q_DECLARE_FLAGS(Lines, Line)
	
	Bus(int lines = 0);

	bool isLineSet(BusLineId::ID id) const;
	bool isLineSet(int id) const;
	operator Lines() const;
	BusConnector *connector();

    public slots:
	void set(int lines);

    signals:
	void changed(const Bus *bus);

    private:
	Lines lines;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Bus::Lines)

#endif
