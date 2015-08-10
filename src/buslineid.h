#ifndef SCTL_BUSLINEID_H
#define SCTL_BUSLINEID_H

#include <QObject>
#include <QString>

class BusLineId: public QObject
{
    Q_OBJECT
    Q_ENUMS(ID)

    public:
	enum ID
	{
	    A0 = 0,
	    A1,
	    A2,
	    A3,
	    D0,
	    D1,
	    D2,
	    BC,
	    IRQ
	};

	static const QString &name(ID id);
	static const QString &name(int id);

    private:
	BusLineId();
};

#endif
