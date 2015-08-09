#include <QtGui>
#include "controllerwidget.h"
#include "shutterwidget.h"
#include "buswidget.h"

#define NUM_SHUTTERS 6

class MainWindow: public QWidget
{
    Q_OBJECT

    public:
	MainWindow();

    private:
	ControllerWidget *controller;
	ShutterWidget *shutter[NUM_SHUTTERS];
	BusWidget *bus;
};

