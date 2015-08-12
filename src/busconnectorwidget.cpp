#include "busconnectorwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>

class BusConnectorPinWidget: public QWidget
{
    private:
	bool dir;

    public:
	BusConnectorPinWidget(QWidget *parent = 0)
	    : QWidget(parent), dir(false)
	{
	    setMinimumSize(9, 9);
	}

	void setDirection(bool dir)
	{
	    if (this->dir != dir)
	    {
		this->dir = dir;
		update();
	    }
	}

    protected:
	virtual void paintEvent(QPaintEvent *ev)
	{
	    QSize sz = size();
	    QPainter painter(this);
	    painter.setPen(QPen(dir?Qt::red:Qt::green, 7));
	    painter.drawPoint(sz.width()/2, sz.height()/2);
	}
};

BusConnectorWidget::BusConnectorWidget(QWidget *parent)
    : QGroupBox("Bus Connector", parent)
{
    QGridLayout *layout = new QGridLayout();
    QLabel *lbl;

    for (int i = 0; i < 8; ++i)
    {
	lbl = new QLabel(QString::number(i), this);
	lbl->setAlignment(Qt::AlignHCenter);
	pins[i] = new BusConnectorPinWidget(this);
	layout->addWidget(lbl, 0, i);
	layout->addWidget(pins[i], 1, i);
    }

    setLayout(layout);
}

void BusConnectorWidget::show(int lines)
{
    for (int i = 0; i < 8; ++i)
    {
	pins[i]->setDirection(lines & (1 << i));
    }
}

