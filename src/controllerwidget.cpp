#include "controllerwidget.h"
#include "busclock.h"

ControllerWidget::ControllerWidget(QWidget *connector, QWidget *parent)
    : QGroupBox(QString(tr("Controller")), parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();

    upButton = new QPushButton(tr("up"), this);
    downButton = new QPushButton(tr("down"), this);
    clockSelect = new QComboBox(this);

    for (int i = 0; i < BusClock::nClocks; ++i)
    {
	clockSelect->addItem(tr(BusClock::byId(i).name()));
    }
    clockSelect->setCurrentIndex(BusClock::defClock);

    hbox->addWidget(new QLabel(tr("clock:"), this));
    hbox->addWidget(clockSelect);

    vbox->addItem(new QSpacerItem(0, 0,
		QSizePolicy::Minimum, QSizePolicy::Expanding));
    vbox->addLayout(hbox);
    vbox->addWidget(upButton);
    vbox->addWidget(downButton);
    vbox->addWidget(connector);

    setLayout(vbox);

    connect(upButton, SIGNAL(clicked()), this, SLOT(upClicked()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(downClicked()));
    connect(clockSelect, SIGNAL(currentIndexChanged(int)),
	    this, SLOT(clockSelected(int)));
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
