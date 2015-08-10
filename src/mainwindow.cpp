#include "mainwindow.h"

MainWindow::MainWindow()
{
   QVBoxLayout *vbox = new QVBoxLayout();
   QHBoxLayout *hbox = new QHBoxLayout();

   controllerwidget = new ControllerWidget(this);
   hbox->addWidget(controllerwidget);

   for (int i = 0; i < NUM_SHUTTERS; ++i)
   {
       shutterwidgets[i] = new ShutterWidget(i, this);
       hbox->addWidget(shutterwidgets[i]);
   }

   vbox->addLayout(hbox);

   buswidget = new BusWidget(this);
   vbox->addWidget(buswidget);

   setLayout(vbox);
   setWindowTitle(tr("Shutter control simulation"));

   connect(&bus, SIGNAL(changed(const Bus *)),
	   buswidget, SLOT(show(const Bus *)));

   connect(&controller, SIGNAL(writeBus(int)),
	   &bus, SLOT(set(int)));
   connect(&bus, SIGNAL(changed(const Bus *)),
	   &controller, SLOT(readBus(const Bus *)));

   connect(controllerwidget, SIGNAL(up()), &controller, SLOT(up()));
   connect(controllerwidget, SIGNAL(down()), &controller, SLOT(down()));
}

