#ifndef SCTL_CONTROLLERACTION_H
#define SCTL_CONTROLLERACTION_H

#include <QVector>

class ControllerAction
{
    public:
	class Step
	{
	    friend class ControllerAction;
	    friend class QVector<Step>;

	    public:
		enum Type
		{
		    Direction,
		    Write,
		    Read,
		    WaitClock,
		    Done
		};

		Type type() const;
		int value() const;

	    private:
		Step(Type type, int value = 0);
		Step();
		Type _type;
		int _value;
	};

	enum Type
	{
	    Up,
	    Down,
	    Stop,
	    Status
	};

	const Step &nextStep();
	int address() const;

	ControllerAction(Type type, int address);

    private:
	int _pos;
	int _address;
	QVector<Step> _steps;
};

#endif
