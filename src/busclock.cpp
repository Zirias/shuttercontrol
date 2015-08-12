#include "busclock.h"

const BusClock BusClock::UltraFast(0, 10, "fastest");
const BusClock BusClock::Fast(1, 50, "fast");
const BusClock BusClock::Medium(2, 200, "medium");
const BusClock BusClock::Slow(3, 500, "slow");
const BusClock BusClock::UltraSlow(4, 2000, "slowest");

const int BusClock::nClocks = 5;
const int BusClock::defClock = 2;

BusClock::BusClock(int id, int period, const char *name)
    : _id(id), _period(period), _name(name)
{}

int BusClock::id() const
{
    return _id;
}

int BusClock::period() const
{
    return _period;
}

const char *BusClock::name() const
{
    return _name;
}

const BusClock &BusClock::byId(int id)
{
    switch(id)
    {
	case 0: return BusClock::UltraFast;
	case 1: return BusClock::Fast;
	case 2: return BusClock::Medium;
	case 3: return BusClock::Slow;
	case 4: return BusClock::UltraSlow;
	default: return byId(BusClock::defClock);
    }
}

