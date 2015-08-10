#include "busconnector.h"

BusConnector::BusConnector()
    : QObject(), directions(0), readLines(0)
{}

int BusConnector::getDirections() const
{
    return directions;
}

int BusConnector::read() const
{
    return readLines;
}

void BusConnector::setDirections(int dir)
{
    Bus::Lines dirLines = (Bus::Lines)dir;
    if (dirLines != directions)
    {
	directions = dirLines;
	emit changed(directions);
    }
}

void BusConnector::write(int lines)
{
    emit writeBus(lines & (int)directions);
}

void BusConnector::readBus(const Bus *bus)
{
    Bus::Lines filtered = ~directions & (Bus::Lines) *bus;
    if (filtered != readLines)
    {
	readLines = filtered;
	emit readChanged(readLines);
    }
}

