#include "buslinewidget.h"

BusLineWidget::BusLineWidget(QWidget *parent)
    : QWidget(parent)
{}

void BusLineWidget::paintEvent(QPaintEvent *ev)
{
    QSize sz = size();
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 7, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(6, sz.height()/2, sz.width()-6, sz.height()/2);
}

