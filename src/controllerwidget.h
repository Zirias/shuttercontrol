#ifndef SCTL_CONTROLLERWIDGET_H
#define SCTL_CONTRLLLERWIDGET_H

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
	void clockSelected(int index);
	void addrSelected(int index);

    signals:
	void up();
	void down();
	void selectClock(const BusClock *clock);
	void selectAddr(int addr);

    private:
	QPushButton *upButton;
	QPushButton *downButton;
	QComboBox *clockSelect;
	QComboBox *addrSelect;
};

#endif
