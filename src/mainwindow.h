#ifndef SCTL_MAINWINDOW_H
#define SCTL_MAINWINDOW_H

#include <QWidget>

#include "bus.h"
#include "controller.h"

class ControllerWidget;
class ShutterWidget;
class BusWidget;
class Shutter;

#define NUM_SHUTTERS 4

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
