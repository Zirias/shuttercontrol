#include "shutterwidget.h"

ShutterWidget::ShutterWidget(int i, QWidget *controller, QWidget *parent)
    : QGroupBox(QString(tr("Shutter %1")).arg(i), parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    up = new QPushButton(tr("up"));
    down = new QPushButton(tr("down"));

    vbox->addWidget(up);
    vbox->addWidget(down);
    vbox->addWidget(controller);

    setLayout(vbox);
}

