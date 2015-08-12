#ifndef SCTL_AVRWIDGET_H
#define SCTL_AVRWIDGET_H

#include <QGroupBox>

class QLabel;
class AvrOutPinWidget;

class AvrWidget: public QGroupBox
{
    Q_OBJECT

    public:
	AvrWidget(QWidget *parent = 0);

    public slots:
	void stateChanged(int state);
	void up(bool active);
	void down(bool active);

    private:
	QLabel *state;
	AvrOutPinWidget *upPin;
	AvrOutPinWidget *downPin;
};

#endif
