#ifndef SCTL_BUSCLOCK_H
#define SCTL_BUSCLOCK_H

#include <QtCore>

class BusClock
{
    private:
	int _id;
	int _period;
	const char *_name;

	BusClock(int id, int period, const char *name);

    public:
	int id() const;
	int period() const;
	const char *name() const;

	static const BusClock UltraFast;
	static const BusClock Fast;
	static const BusClock Medium;
	static const BusClock Slow;
	static const BusClock UltraSlow;
	static const BusClock &byId(int id);

	static const int nClocks;
	static const int defClock;
};

#endif
