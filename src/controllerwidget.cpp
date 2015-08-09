#include "controllerwidget.h"

ControllerWidget::ControllerWidget(QWidget *parent)
    : QGroupBox(QString(tr("Controller")), parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    up = new QPushButton(tr("up"));
    down = new QPushButton(tr("down"));

    vbox->addWidget(up);
    vbox->addWidget(down);

    setLayout(vbox);
}

