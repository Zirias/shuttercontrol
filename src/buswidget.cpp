#include "buswidget.h"

#include "bus.h"
#include "buslineid.h"
#include "buslinewidget.h"

#include <QGridLayout>
#include <QLabel>

BusWidget::BusWidget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout();
    QLabel *lbl;

    for (int i = 0; i < 8; ++i)
    {
	lbl = new QLabel(
		QString("[%1] %2").arg(i).arg(BusLineId::name(i)), this);
	lines[i] = new BusLineWidget(this);
	layout->addWidget(lbl, i, 0);
	layout->addWidget(lines[i], i, 1);
    }
    layout->setColumnStretch(1, 1);

    setLayout(layout);
}

void BusWidget::show(const Bus *bus)
{
    for (int i = 0; i < 8; ++i)
    {
	lines[i]->set(bus->isLineSet(i));
    }
}

