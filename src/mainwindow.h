#ifndef SCTL_MAINWINDOW_H
#define SCTL_MAINWINDOW_H

#include <QtGui>
#include "controllerwidget.h"
#include "shutterwidget.h"
#include "buswidget.h"
#include "bus.h"
#include "controller.h"

#define NUM_SHUTTERS 4

class Shutter;

class MainWindow: public QWidget
{
    Q_OBJECT

    public:
	MainWindow();

    private:
	ControllerWidget *controllerwidget;
	ShutterWidget *shutterwidgets[NUM_SHUTTERS];
	BusWidget *buswidget;
	Bus bus;
	Controller controller;
	Shutter *shutters[NUM_SHUTTERS];
};

#endif
