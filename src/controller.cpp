#include "controller.h"

#include "bus.h"
#include "busclock.h"
#include "controlleraction.h"
#include <QTimer>

Controller::Controller()
    : BusClient(), clock(&BusClock::byId(BusClock::defClock)),
    addr(0), action(0)
{
    timer = new QTimer(this);
    timer->setInterval(clock->period());
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
}

Controller::~Controller()
{
    timer->stop();
    delete timer;
}

void Controller::connected()
{
    emit setDirections(0xe0);
}

void Controller::readBus(int lines)
{
}

void Controller::up()
{
    startAction(new ControllerAction(ControllerAction::Up, addr));
}

void Controller::down()
{
    startAction(new ControllerAction(ControllerAction::Down, addr));
}

void Controller::stop()
{
    startAction(new ControllerAction(ControllerAction::Stop, addr));
}

void Controller::setClock(const BusClock *clock)
{
    this->clock = clock;
    timer->setInterval(clock->period());
}

void Controller::step()
{
    if (!action)
    {
	if (!pending.isEmpty()) action = pending.dequeue();
	else
	{
	    timer->stop();
	    return;
	}
    }
    bool running = true;
    while (running)
    {
	const ControllerAction::Step &step = action->nextStep();
	switch (step.type())
	{
	    case ControllerAction::Step::Direction:
		emit setDirections(step.value());
		break;

	    case ControllerAction::Step::Write:
		emit writeBus(step.value());
		break;

	    case ControllerAction::Step::Read:
		break;

	    case ControllerAction::Step::WaitClock:
		running = false;
		break;

	    case ControllerAction::Step::Done:
		running = false;
		delete action;
		action = 0;
	}
    }
}

void Controller::startAction(ControllerAction *act)
{
    pending.enqueue(act);
    if (!timer->isActive())
    {
	timer->start();
	step();
    }
}

void Controller::setAddr(int addr)
{
    this->addr = addr;
}

