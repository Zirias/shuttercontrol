#ifndef SCTL_SHUTTERWIDGET_H
#define SCTL_SHUTTERWIDGET_H

#include <QGroupBox>

class QPushButton;
class AvrWidget;

class ShutterWidget: public QGroupBox
{
    Q_OBJECT

    public:
	ShutterWidget(int i, QWidget *connector, QWidget *parent = 0);

    public slots:
	void stateChanged(int state);
	void up(bool active);
	void down(bool active);

    private slots:
	void upPressed();
	void upReleased();
	void downPressed();
	void downReleased();

    signals:
	void p_up();
	void r_up();
	void p_down();
	void r_down();

    private:
	AvrWidget *avr;
	QPushButton *upBt;
	QPushButton *downBt;
};

#endif
