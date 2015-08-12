#include "controllerwidget.h"

#include "busclock.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

static QString addrlabel(const QString &a, int addr)
{
    switch (addr)
    {
	case 0x0: return a.arg(ControllerWidget::tr("BC"));
	case 0xf: return a.arg(ControllerWidget::tr("NA"));
	default: return a.arg(addr,2,10,QChar('0'));
    }
}

ControllerWidget::ControllerWidget(QWidget *connector, QWidget *parent)
    : QGroupBox(QString(tr("Controller")), parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();

    vbox->addItem(new QSpacerItem(0, 0,
		QSizePolicy::Minimum, QSizePolicy::Expanding));

    clockSelect = new QComboBox(this);

    for (int i = 0; i < BusClock::nClocks; ++i)
    {
	clockSelect->addItem(tr(BusClock::byId(i).name()));
    }
    clockSelect->setCurrentIndex(BusClock::defClock);

    hbox->addWidget(new QLabel(tr("clock:"), this));
    hbox->addWidget(clockSelect);

    vbox->addLayout(hbox);

    hbox = new QHBoxLayout();
    addrSelect = new QComboBox(this);
    for (int i = 0x0; i < 0x10; ++i)
    {
	addrSelect->addItem(addrlabel(QString("0x%1: [%2]").arg(i,0,16), i));
    }

    hbox->addWidget(new QLabel(tr("addr:"), this));
    hbox->addWidget(addrSelect);

    vbox->addLayout(hbox);

    upButton = new QPushButton(tr("up"), this);
    downButton = new QPushButton(tr("down"), this);

    vbox->addWidget(upButton);
    vbox->addWidget(downButton);
    vbox->addWidget(connector);

    setLayout(vbox);

    connect(upButton, SIGNAL(clicked()), this, SLOT(upClicked()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(downClicked()));
    connect(clockSelect, SIGNAL(currentIndexChanged(int)),
	    this, SLOT(clockSelected(int)));
    connect(addrSelect, SIGNAL(currentIndexChanged(int)),
	    this, SLOT(addrSelected(int)));
}

void ControllerWidget::upClicked()
{
    emit up();
}

void ControllerWidget::downClicked()
{
    emit down();
}

void ControllerWidget::clockSelected(int index)
{
    emit selectClock(&BusClock::byId(index));
}

void ControllerWidget::addrSelected(int index)
{
    emit selectAddr(index);
}
