#include "mainwindow.h"

#include "busconnector.h"
#include "shutter.h"
#include "controllerwidget.h"
#include "shutterwidget.h"
#include "busconnectorwidget.h"
#include "buswidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

class BusClock;

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
       connect(shutters[i], SIGNAL(stateChanged(const char *)),
	       shutterwidgets[i], SLOT(stateChanged(const char *)));
       connect(shutterwidgets[i], SIGNAL(p_up()), shutters[i], SLOT(p_up()));
       connect(shutterwidgets[i], SIGNAL(r_up()), shutters[i], SLOT(r_up()));
       connect(shutterwidgets[i], SIGNAL(p_down()),
	       shutters[i], SLOT(p_down()));
       connect(shutterwidgets[i], SIGNAL(r_down()),
	       shutters[i], SLOT(r_down()));
   }

   vbox->addLayout(hbox);

   buswidget = new BusWidget(this);
   vbox->addWidget(buswidget);

   setLayout(vbox);
   setWindowTitle(tr("Shutter control simulation"));

   connect(&bus, SIGNAL(changed(const Bus *)),
	   buswidget, SLOT(show(const Bus *)));
   buswidget->show(&bus);

   controller.connectBus(&bus);
   BusConnector *busconn = controller.connector();
   connect(busconn, SIGNAL(changed(int)), controllerConnector, SLOT(show(int)));
   controllerConnector->show(busconn->getDirections());

   connect(controllerwidget, SIGNAL(up()), &controller, SLOT(up()));
   connect(controllerwidget, SIGNAL(down()), &controller, SLOT(down()));
   connect(controllerwidget, SIGNAL(selectClock(const BusClock *)),
	   &controller, SLOT(setClock(const BusClock *)));
   connect(controllerwidget, SIGNAL(selectAddr(int)),
	   &controller, SLOT(setAddr(int)));
}

