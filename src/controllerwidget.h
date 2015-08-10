#ifndef SCTL_CONTROLLERWIDGET_H
#define SCTL_CONTRLLLERWIDGET_H

#include <QtGui>

class BusClock;

class ControllerWidget: public QGroupBox
{
    Q_OBJECT

    public:
	ControllerWidget(QWidget *connector, QWidget *parent = 0);

    private slots:
	void upClicked();
	void downClicked();
	void clockSelected(int index);

    signals:
	void up();
	void down();
	void selectClock(const BusClock *clock);

    private:
	QPushButton *upButton;
	QPushButton *downButton;
	QComboBox *clockSelect;
};

#endif
