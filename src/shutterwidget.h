#include <QtGui>

class ShutterWidget: public QGroupBox
{
    Q_OBJECT

    public:
	ShutterWidget(int i, QWidget *parent = 0);

    private:
	QPushButton *up;
	QPushButton *down;
};

