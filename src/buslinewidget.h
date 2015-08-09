#include <QtGui>

class BusLineWidget: public QWidget
{
    Q_OBJECT

    public:
	BusLineWidget(QWidget *parent = 0);

    protected:
	virtual void paintEvent(QPaintEvent *ev);
};

