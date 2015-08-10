#include "controller.h"

#include "bus.h"
#include <QThread>
#include <QTimer>

Controller::Controller()
    : QObject()
{
    QThread* thread = new QThread();
    moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(run()));
    connect(this, SIGNAL(finished()), thread, SLOT(quit()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void Controller::readBus(const Bus *bus)
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

void Controller::run()
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(resetBus()));
}

void Controller::resetBus()
{
    emit writeBus(0);
}
