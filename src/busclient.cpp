#include "busclient.h"

#include "busconnector.h"

BusClient::BusClient()
    : QObject(), conn(0)
{
}

BusConnector *BusClient::connector()
{
    return conn;
}

void BusClient::connectBus(Bus *bus)
{
    conn = bus->connector();
    
    connect(conn, SIGNAL(readChanged(int)),
	    this, SLOT(readBus(int)));
    connect(this, SIGNAL(writeBus(int)),
	    conn, SLOT(write(int)));
    connect(this, SIGNAL(setDirections(int)),
	    conn, SLOT(setDirections(int)));

    connected();
}
