#ifndef SCTL_BUSCONNECTORWIDGET_H
#define SCTL_BUSCONNECTORWIDGET_H

#include <QGroupBox>

class BusConnectorPinWidget;

class BusConnectorWidget: public QGroupBox
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
