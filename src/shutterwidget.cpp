#include "shutterwidget.h"

#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QLabel>

ShutterWidget::ShutterWidget(int i, QWidget *controller, QWidget *parent)
    : QGroupBox(QString(tr("Shutter %1")).arg(i), parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    state = new QLabel("limbo");
    up = new QPushButton(tr("up"));
    down = new QPushButton(tr("down"));

    vbox->addItem(new QSpacerItem(0, 0,
		QSizePolicy::Minimum, QSizePolicy::Expanding));
    vbox->addWidget(state);
    vbox->addWidget(up);
    vbox->addWidget(down);
    vbox->addWidget(controller);

    connect(up, SIGNAL(pressed()), this, SLOT(upPressed()));
    connect(up, SIGNAL(released()), this, SLOT(upReleased()));
    connect(down, SIGNAL(pressed()), this, SLOT(downPressed()));
    connect(down, SIGNAL(released()), this, SLOT(downReleased()));

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

void ShutterWidget::stateChanged(const char *state)
{
    this->state->setText(state);
}
