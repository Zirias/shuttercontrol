#include "controlleraction.h"

ControllerAction::Step::Step()
    : _type(ControllerAction::Step::Done), _value(0)
{}

ControllerAction::Step::Step(ControllerAction::Step::Type type, int value)
    : _type(type), _value(value)
{}

ControllerAction::Step::Type ControllerAction::Step::type() const
{
    return _type;
}

int ControllerAction::Step::value() const
{
    return _value;
}

static const int cmd_status = 0x00;
static const int cmd_up = 0x10;
static const int cmd_down = 0x20;
static const int cmd_stop = 0x30;

ControllerAction::ControllerAction(ControllerAction::Type type, int address)
    : _pos(-1), _address(address)
{
    _steps.append(Step(Step::Direction, 0xff));
    switch (type)
    {
	case ControllerAction::Up:
	    _steps.append(Step(Step::Write, 0x80 | cmd_up | address));
	    _steps.append(Step(Step::WaitClock));
	    _steps.append(Step(Step::Write, 0xc0 | cmd_up | address));
	    break;
	case ControllerAction::Down:
	    _steps.append(Step(Step::Write, 0x80 | cmd_down | address));
	    _steps.append(Step(Step::WaitClock));
	    _steps.append(Step(Step::Write, 0xc0 | cmd_down | address));
	    break;
	case ControllerAction::Stop:
	    _steps.append(Step(Step::Write, 0x80 | cmd_stop | address));
	    _steps.append(Step(Step::WaitClock));
	    _steps.append(Step(Step::Write, 0xc0 | cmd_stop | address));
	    break;
	case ControllerAction::Status:
	    _steps.append(Step(Step::Write, 0x80 | cmd_status | address));
	    _steps.append(Step(Step::WaitClock));
	    _steps.append(Step(Step::Write, 0xc0 | cmd_status | address));
	    break;
    }
    _steps.append(Step(Step::WaitClock));
    _steps.append(Step(Step::Write, 0xbf));
    _steps.append(Step(Step::Direction, 0xcf));

    if (type == ControllerAction::Status)
    {
	_steps.append(Step(Step::WaitClock));
	_steps.append(Step(Step::Write, 0xff));
	_steps.append(Step(Step::WaitClock));
	_steps.append(Step(Step::Write, 0xbf));
	_steps.append(Step(Step::Read));
    }

    _steps.append(Step(Step::WaitClock));
    _steps.append(Step(Step::Write, 0xff));
    _steps.append(Step(Step::Done));
}

const ControllerAction::Step &ControllerAction::nextStep()
{
    if (_pos < _steps.size()-1) ++_pos;
    return _steps.at(_pos);
}

int ControllerAction::address() const
{
    return _address;
}
