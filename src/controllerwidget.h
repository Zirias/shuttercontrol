#include <QtGui>

class ControllerWidget: public QGroupBox
{
    Q_OBJECT

    public:
	ControllerWidget(QWidget *parent = 0);

    private:
	QPushButton *up;
	QPushButton *down;
};

