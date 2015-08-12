#include "shutterwidget.h"

#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include "avrwidget.h"

ShutterWidget::ShutterWidget(int i, QWidget *controller, QWidget *parent)
    : QGroupBox(QString(tr("Shutter %1")).arg(i), parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    avr = new AvrWidget(this);
    upBt = new QPushButton(tr("up"), this);
    downBt = new QPushButton(tr("down"), this);

    vbox->addItem(new QSpacerItem(0, 0,
		QSizePolicy::Minimum, QSizePolicy::Expanding));
    vbox->addWidget(avr);
    vbox->addWidget(upBt);
    vbox->addWidget(downBt);
    vbox->addWidget(controller);

    connect(upBt, SIGNAL(pressed()), this, SLOT(upPressed()));
    connect(upBt, SIGNAL(released()), this, SLOT(upReleased()));
    connect(downBt, SIGNAL(pressed()), this, SLOT(downPressed()));
    connect(downBt, SIGNAL(released()), this, SLOT(downReleased()));

    setLayout(vbox);
}

void ShutterWidget::upPressed()
{
    emit p_up();
}

void ShutterWidget::upReleased()
{
    emit r_up();
}

void ShutterWidget::downPressed()
{
    emit p_down();
}

void ShutterWidget::downReleased()
{
    emit r_down();
}

void ShutterWidget::up(bool active)
{
    avr->up(active);
}

void ShutterWidget::down(bool active)
{
    avr->down(active);
}

void ShutterWidget::stateChanged(int state)
{
    avr->stateChanged(state);
}
