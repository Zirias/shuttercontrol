#include "bus.h"

Bus::Bus(int lines)
    : QObject(), lines(lines)
{}

void Bus::set(Bus::Lines lines)
{
    this->lines = lines;
    emit changed(this);
}

void Bus::set(int lines)
{
    set((Bus::Lines) lines);
}

void Bus::setLine(BusLineId::ID id)
{
    lines |= Line(1 << id);
    emit changed(this);
}

void Bus::setLine(int id)
{
    setLine((BusLineId::ID) id);
}

void Bus::clearLine(BusLineId::ID id)
{
    lines &= ~(Line(1 << id));
    emit changed(this);
}

void Bus::clearLine(int id)
{
    clearLine((BusLineId::ID) id);
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
