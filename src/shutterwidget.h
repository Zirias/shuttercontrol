#ifndef SCTL_SHUTTERWIDGET_H
#define SCTL_SHUTTERWIDGET_H

#include <QGroupBox>

class QPushButton;
class QLabel;

class ShutterWidget: public QGroupBox
{
    Q_OBJECT

    public:
	ShutterWidget(int i, QWidget *connector, QWidget *parent = 0);

    public slots:
	void stateChanged(const char *state);

    private:
	QLabel *state;
	QPushButton *up;
	QPushButton *down;
};

#endif
