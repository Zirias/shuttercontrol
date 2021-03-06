#include "shutter.h"
#include "avrcontainer.h"
#include "avrpinconnector.h"
#include <QThread>

Shutter::Shutter(int address)
    : BusClient(), chipRunning(false)
{
    chip = new AvrContainer(address);
    connect(chip, SIGNAL(initialized()), this, SLOT(avr_started()));
    connect(this, SIGNAL(chipConnected()), chip, SLOT(run()));
    connect(chip, SIGNAL(stateChanged(int)),
	    this, SLOT(proxy_stateChanged(int)));
}

Shutter::~Shutter()
{
    chip->stop();
}

void Shutter::readBus(int lines)
{
    emit proxy_readBus(lines);
}

void Shutter::p_up()
{
    emit proxy_p_up();
}

void Shutter::r_up()
{
    emit proxy_r_up();
}

void Shutter::p_down()
{
    emit proxy_p_down();
}

void Shutter::r_down()
{
    emit proxy_r_down();
}

void Shutter::startChip()
{
    if (chipRunning) return;
    QThread *thread = new QThread();
    chip->moveToThread(thread);
    connect(thread, SIGNAL(started()), chip, SLOT(init()));
    connect(chip, SIGNAL(finished()), thread, SLOT(quit()));
    connect(chip, SIGNAL(finished()), chip, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    chipRunning = true;
}

void Shutter::proxy_stateChanged(int state)
{
    emit stateChanged(state);
}

void Shutter::proxy_writeBus(int lines)
{
    emit writeBus(lines);
}

void Shutter::proxy_setDirections(int lines)
{
    emit setDirections(lines);
}

void Shutter::proxy_up(bool active)
{
    emit up(active);
}

void Shutter::proxy_down(bool active)
{
    emit down(active);
}

void Shutter::avr_started()
{
    AvrPinConnector *cn = chip->getConnector();
    connect(this, SIGNAL(proxy_p_up()), cn, SLOT(p_up()));
    connect(this, SIGNAL(proxy_r_up()), cn, SLOT(r_up()));
    connect(this, SIGNAL(proxy_p_down()), cn, SLOT(p_down()));
    connect(this, SIGNAL(proxy_r_down()), cn, SLOT(r_down()));
    connect(this, SIGNAL(proxy_readBus(int)), cn, SLOT(readBus(int)));
    connect(cn, SIGNAL(writeBus(int)), this, SLOT(proxy_writeBus(int)));
    connect(cn, SIGNAL(setDirections(int)),
	    this, SLOT(proxy_setDirections(int)));
    connect(cn, SIGNAL(up(bool)), this, SLOT(proxy_up(bool)));
    connect(cn, SIGNAL(down(bool)), this, SLOT(proxy_down(bool)));
    emit chipConnected();
}

void Shutter::connected()
{
}

