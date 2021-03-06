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
	    D0 = 1 << BusLineId::D0,
	    D1 = 1 << BusLineId::D1,
	    D2 = 1 << BusLineId::D2,
	    D3 = 1 << BusLineId::D3,
	    D4 = 1 << BusLineId::D4,
	    AT = 1 << BusLineId::AT,
	    BC = 1 << BusLineId::BC,
	    RS = 1 << BusLineId::RS
	};
	Q_DECLARE_FLAGS(Lines, Line)
	
	Bus(int lines = 0xff);

	bool isLineSet(BusLineId::ID id) const;
	bool isLineSet(int id) const;
	operator Lines() const;
	BusConnector *connector();

    public slots:
	void set(int lines, int mask);

    signals:
	void changed(const Bus *bus);

    private:
	Lines lines;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Bus::Lines)

#endif
