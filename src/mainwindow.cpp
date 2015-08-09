#include "mainwindow.h"

MainWindow::MainWindow()
{
   QVBoxLayout *vbox = new QVBoxLayout();
   QHBoxLayout *hbox = new QHBoxLayout();

   controller = new ControllerWidget(this);
   hbox->addWidget(controller);

   for (int i = 0; i < NUM_SHUTTERS; ++i)
   {
       shutter[i] = new ShutterWidget(i, this);
       hbox->addWidget(shutter[i]);
   }

   vbox->addLayout(hbox);

   bus = new BusWidget(this);
   vbox->addWidget(bus);

   setLayout(vbox);
   setWindowTitle(tr("Shutter control simulation"));
}

