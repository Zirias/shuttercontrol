#ifndef SCTL_BUSWIDGET_H
#define SCTL_BUSWIDGET_H

#include <QWidget>

class Bus;
class BusLineWidget;

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
