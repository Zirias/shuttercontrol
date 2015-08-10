#include "mainwindow.h"
#include "busconnectorwidget.h"
#include "busconnector.h"
#include "shutter.h"

MainWindow::MainWindow()
{
   QVBoxLayout *vbox = new QVBoxLayout();
   QHBoxLayout *hbox = new QHBoxLayout();

   BusConnectorWidget *controllerConnector = new BusConnectorWidget(this);
   controllerwidget = new ControllerWidget(controllerConnector, this);
   hbox->addWidget(controllerwidget);

   for (int i = 0; i < NUM_SHUTTERS; ++i)
   {
       shutters[i] = new Shutter(i+1);
       BusConnectorWidget *cw = new BusConnectorWidget(this);
       shutterwidgets[i] = new ShutterWidget(i+1, cw, this);
       hbox->addWidget(shutterwidgets[i]);
       shutters[i]->connectBus(&bus);
       BusConnector *c = shutters[i]->connector();
       connect(c, SIGNAL(changed(int)), cw, SLOT(show(int)));
       cw->show(c->getDirections());
   }

   vbox->addLayout(hbox);

   buswidget = new BusWidget(this);
   vbox->addWidget(buswidget);

   setLayout(vbox);
   setWindowTitle(tr("Shutter control simulation"));

   connect(&bus, SIGNAL(changed(const Bus *)),
	   buswidget, SLOT(show(const Bus *)));

   controller.connectBus(&bus);
   BusConnector *busconn = controller.connector();
   connect(busconn, SIGNAL(changed(int)), controllerConnector, SLOT(show(int)));
   controllerConnector->show(busconn->getDirections());

   connect(controllerwidget, SIGNAL(up()), &controller, SLOT(up()));
   connect(controllerwidget, SIGNAL(down()), &controller, SLOT(down()));
}

