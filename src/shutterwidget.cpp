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

    setLayout(vbox);
}

void ShutterWidget::stateChanged(const char *state)
{
    this->state->setText(state);
}
