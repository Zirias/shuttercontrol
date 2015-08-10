#include "controller.h"

#include "bus.h"
#include <QThread>
#include <QTimer>

Controller::Controller()
    : BusClient()
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
    timer->setSingleShot(500);
    timer->start(500);
}

void Controller::down()
{
    emit writeBus(Bus::IRQ | 0xf | Bus::D1);
    timer->start(500);
}

void Controller::resetBus()
{
    emit writeBus(0);
}
