#ifndef SCTL_BUSLINEWIDGET_H
#define SCTL_BUSLINEWIDGET_H

#include <QWidget>

class BusLineWidget: public QWidget
{
    Q_OBJECT

    public:
	BusLineWidget(QWidget *parent = 0);

    public slots:
	void set(bool state);

    protected:
	virtual void paintEvent(QPaintEvent *ev);

    private:
	bool isSet;
};

#endif
