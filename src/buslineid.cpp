#include "buslineid.h"

static const QString names[] = {
    "A0",
    "A1",
    "A2",
    "A3",
    "D0",
    "D1",
    "D2",
    "BC",
    "IRQ"
};

static const QString err = "[ERR]";

BusLineId::BusLineId() {};

const QString &BusLineId::name(BusLineId::ID id)
{
    if (id < 0 || id > IRQ) return err;
    return names[id];
}

const QString &BusLineId::name(int id)
{
    return name((BusLineId::ID) id);
}
