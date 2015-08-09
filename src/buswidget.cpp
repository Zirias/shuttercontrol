#include "buswidget.h"

BusWidget::BusWidget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout();
    QLabel *lbl;

    lbl = new QLabel("A0");
    a0 = new BusLineWidget(this);
    layout->addWidget(lbl, 0, 0);
    layout->addWidget(a0, 0, 1);
    lbl = new QLabel("A1");
    a1 = new BusLineWidget(this);
    layout->addWidget(lbl, 1, 0);
    layout->addWidget(a1, 1, 1);
    lbl = new QLabel("A2");
    a2 = new BusLineWidget(this);
    layout->addWidget(lbl, 2, 0);
    layout->addWidget(a2, 2, 1);
    lbl = new QLabel("A3");
    a3 = new BusLineWidget(this);
    layout->addWidget(lbl, 3, 0);
    layout->addWidget(a3, 3, 1);
    lbl = new QLabel("D0");
    d0 = new BusLineWidget(this);
    layout->addWidget(lbl, 4, 0);
    layout->addWidget(d0, 4, 1);
    lbl = new QLabel("D1");
    d1 = new BusLineWidget(this);
    layout->addWidget(lbl, 5, 0);
    layout->addWidget(d1, 5, 1);
    lbl = new QLabel("D2");
    d2 = new BusLineWidget(this);
    layout->addWidget(lbl, 6, 0);
    layout->addWidget(d2, 6, 1);
    lbl = new QLabel("BC");
    bc = new BusLineWidget(this);
    layout->addWidget(lbl, 7, 0);
    layout->addWidget(bc, 7, 1);
    lbl = new QLabel("IRQ");
    irq = new BusLineWidget(this);
    layout->addWidget(lbl, 8, 0);
    layout->addWidget(irq, 8, 1);
    
    layout->setColumnStretch(1, 1);

    setLayout(layout);
}

