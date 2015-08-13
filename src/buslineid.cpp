#include "buslineid.h"

static const QString names[] = {
    "D0",
    "D1",
    "D2",
    "D3",
    "D4",
    "AT",
    "BC",
    "RS"
};

static const QString err = "[ERR]";

BusLineId::BusLineId() {};

const QString &BusLineId::name(BusLineId::ID id)
{
    if (id < 0 || id > RS) return err;
    return names[id];
}

const QString &BusLineId::name(int id)
{
    return name((BusLineId::ID) id);
}

