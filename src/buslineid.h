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
	    D0 = 0,
	    D1,
	    D2,
	    D3,
	    D4,
	    AT,
	    BC,
	    RS
	};

	static const QString &name(ID id);
	static const QString &name(int id);

    private:
	BusLineId();
};

#endif
