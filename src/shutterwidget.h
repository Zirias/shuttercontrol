#ifndef SCTL_SHUTTERWIDGET_H
#define SCTL_SHUTTERWIDGET_H

#include <QtGui>

class ShutterWidget: public QGroupBox
{
    Q_OBJECT

    public:
	ShutterWidget(int i, QWidget *connector, QWidget *parent = 0);

    private:
	QPushButton *up;
	QPushButton *down;
};

#endif
