#include "buslinewidget.h"

BusLineWidget::BusLineWidget(QWidget *parent)
    : QWidget(parent), isSet(false)
{}

void BusLineWidget::set(bool state)
{
    if (state != isSet)
    {
	isSet = state;
	update();
    }
}

void BusLineWidget::paintEvent(QPaintEvent *ev)
{
    QSize sz = size();
    QPainter painter(this);
    painter.setPen(
	    QPen(isSet?Qt::green:Qt::gray, 7, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(6, sz.height()/2, sz.width()-6, sz.height()/2);
}
