#include "shutterwidget.h"

#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>

ShutterWidget::ShutterWidget(int i, QWidget *controller, QWidget *parent)
    : QGroupBox(QString(tr("Shutter %1")).arg(i), parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    up = new QPushButton(tr("up"));
    down = new QPushButton(tr("down"));

    vbox->addItem(new QSpacerItem(0, 0,
		QSizePolicy::Minimum, QSizePolicy::Expanding));
    vbox->addWidget(up);
    vbox->addWidget(down);
    vbox->addWidget(controller);

    setLayout(vbox);
}

