#include "controllerwidget.h"

ControllerWidget::ControllerWidget(QWidget *connector, QWidget *parent)
    : QGroupBox(QString(tr("Controller")), parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    upButton = new QPushButton(tr("up"));
    downButton = new QPushButton(tr("down"));

    vbox->addWidget(upButton);
    vbox->addWidget(downButton);
    vbox->addWidget(connector);

    setLayout(vbox);

    connect(upButton, SIGNAL(clicked()), this, SLOT(upClicked()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(downClicked()));
}

void ControllerWidget::upClicked()
{
    emit up();
}

void ControllerWidget::downClicked()
{
    emit down();
}

