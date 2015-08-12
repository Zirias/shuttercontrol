#include "bus.h"

#include "busconnector.h"

Bus::Bus(int lines)
    : QObject(), lines(lines)
{}

void Bus::set(int lines, int mask)
{
    this->lines = (Bus::Lines)(((int)this->lines & ~mask) | lines & mask);
    emit changed(this);
}

bool Bus::isLineSet(BusLineId::ID id) const
{
    return lines.testFlag(Line(1 << id));
}

bool Bus::isLineSet(int id) const
{
    return isLineSet((BusLineId::ID) id);
}

Bus::operator Bus::Lines() const
{
    return lines;
}

BusConnector *Bus::connector()
{
    BusConnector *conn = new BusConnector();
    conn->readBus(this);

    connect(this, SIGNAL(changed(const Bus *)),
	    conn, SLOT(readBus(const Bus *)));
    connect(conn, SIGNAL(writeBus(int, int)),
	    this, SLOT(set(int, int)));

    return conn;
}
