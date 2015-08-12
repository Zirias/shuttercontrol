#include <QApplication>
#include "mainwindow.h"

#ifdef WIN32
#include <QWindowsVistaStyle>
#endif

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
#ifdef WIN32
    QApplication::setStyle(new QWindowsVistaStyle());
#endif

    MainWindow w;
    w.show();

    return app.exec();
}
