#include <QtGui>
#include "controllerwidget.h"
#include "shutterwidget.h"

#define NUM_SHUTTERS 4

class MainWindow: public QWidget
{
    Q_OBJECT

    public:
	MainWindow();

    private:
	ControllerWidget *controller;
	ShutterWidget *shutter[NUM_SHUTTERS];
};

