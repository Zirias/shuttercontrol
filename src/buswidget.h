#include <QtGui>

#include "buslinewidget.h"

class BusWidget: public QWidget
{
    Q_OBJECT

    public:
	BusWidget(QWidget *parent = 0);

    private:
	BusLineWidget *a0;
	BusLineWidget *a1;
	BusLineWidget *a2;
	BusLineWidget *a3;
	BusLineWidget *d0;
	BusLineWidget *d1;
	BusLineWidget *d2;
	BusLineWidget *bc;
	BusLineWidget *irq;
};

