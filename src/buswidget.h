#ifndef SCTL_BUSWIDGET_H
#define SCRL_BUSWIDGET_H

#include <QtGui>

#include "buslinewidget.h"
#include "bus.h"

class BusWidget: public QWidget
{
    Q_OBJECT

    public:
	BusWidget(QWidget *parent = 0);

    public slots:
	void show(const Bus *bus);

    private:
	BusLineWidget *lines[9];
};

#endif
