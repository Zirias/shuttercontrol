#include "controller.h"

#include "bus.h"
#include "busclock.h"
#include <QTimer>

Controller::Controller()
    : BusClient(), clock(&BusClock::byId(BusClock::defClock))
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(resetBus()));
}

Controller::~Controller()
{
    timer->stop();
    delete timer;
}

void Controller::connected()
{
    emit setDirections(0x1ff);
}

void Controller::readBus(int lines)
{
}

void Controller::up()
{
    emit writeBus(Bus::IRQ | 0xf | Bus::D0);
    timer->start(clock->period());
}

void Controller::down()
{
    emit writeBus(Bus::IRQ | 0xf | Bus::D1);
    timer->start(clock->period());
}

void Controller::setClock(const BusClock *clock)
{
    this->clock = clock;
}

void Controller::resetBus()
{
    emit writeBus(0);
}
