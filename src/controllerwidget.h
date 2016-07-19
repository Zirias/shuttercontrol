#ifndef SCTL_CONTROLLERWIDGET_H
#define SCTL_CONTROLLERWIDGET_H

#include <QGroupBox>

class BusClock;
class QPushButton;
class QComboBox;

class ControllerWidget: public QGroupBox
{
    Q_OBJECT

    public:
	ControllerWidget(QWidget *connector, QWidget *parent = 0);

    private slots:
	void upClicked();
	void downClicked();
	void stopClicked();
	void calClicked();
	void clockSelected(int index);
	void addrSelected(int index);

    signals:
	void up();
	void down();
	void stop();
	void cal();
	void selectClock(const BusClock *clock);
	void selectAddr(int addr);

    private:
	QPushButton *upButton;
	QPushButton *downButton;
	QPushButton *stopButton;
	QPushButton *calButton;
	QComboBox *clockSelect;
	QComboBox *addrSelect;
};

#endif
