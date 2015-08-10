#ifndef SCTL_CONTROLLERWIDGET_H
#define SCTL_CONTRLLLERWIDGET_H

#include <QtGui>

class ControllerWidget: public QGroupBox
{
    Q_OBJECT

    public:
	ControllerWidget(QWidget *parent = 0);

    private slots:
	void upClicked();
	void downClicked();

    signals:
	void up();
	void down();

    private:
	QPushButton *upButton;
	QPushButton *downButton;
};

#endif
