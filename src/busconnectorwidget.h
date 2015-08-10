#ifndef SCTL_BUSCONNECTORWIDGET_H
#define SCTL_BUSCONNECTORWIDGET_H

#include <QtGui>

class BusConnectorPinWidget;

class BusConnectorWidget: public QWidget
{
    Q_OBJECT

    public:
	BusConnectorWidget(QWidget *parent = 0);

    public slots:
	void show(int lines);
    
    private:
	BusConnectorPinWidget *pins[9];
};

#endif
