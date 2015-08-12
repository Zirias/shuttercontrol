#include "avrwidget.h"

#include <QString>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <sim_avr.h>

class AvrOutPinWidget: public QWidget
{
    private:
	bool active;

    public:
	AvrOutPinWidget(QWidget *parent = 0)
	    : QWidget(parent), active(false)
	{
	    setMinimumSize(9, 9);
	}

	void setActive(bool active)
	{
	    if (this->active != active)
	    {
		this->active = active;
		update();
	    }
	}

    protected:
	virtual void paintEvent(QPaintEvent *ev)
	{
	    QSize sz = size();
	    QPainter painter(this);
	    painter.setPen(QPen(active?Qt::cyan:Qt::black, 7));
	    painter.drawPoint(sz.width()/2, sz.height()/2);
	}
};

static QString stateString(int state)
{
    switch (state)
    {
	case cpu_Limbo:
	    return QString("%2: <b><font color='#333333'>%1</font></b>")
		.arg(AvrWidget::tr("limbo"));
	case cpu_Stopped:
	    return QString("%2: <b><font color='#ff8800'>%1</font></b>")
		.arg(AvrWidget::tr("stopped"));
	case cpu_Running:
	    return QString("%2: <b><font color='#00ff00'>%1</font></b>")
		.arg(AvrWidget::tr("running"));
	case cpu_Sleeping:
	    return QString("%2: <b><font color='#008800'>%1</font></b>")
		.arg(AvrWidget::tr("sleeping"));
	case cpu_Step:
	    return QString("%2: <b><font color='#ff8800'>%1</font></b>")
		.arg(AvrWidget::tr("step"));
	case cpu_StepDone:
	    return QString("%2: <b><font color='#ff8800'>%1</font></b>")
		.arg(AvrWidget::tr("step done"));
	case cpu_Done:
	    return QString("%2: <b><font color='#008800'>%1</font></b>")
		.arg(AvrWidget::tr("done"));
	case cpu_Crashed:
	    return QString("%2: <b><font color='#ff0000'>%1</font></b>")
		.arg(AvrWidget::tr("crashed"));
	default:
	    return QString("%2: <b><font color='#333333'>%1</font></b>")
		.arg(AvrWidget::tr("unknown"));
    };
}

AvrWidget::AvrWidget(QWidget *parent)
    : QGroupBox(tr("virtual AVR"), parent)
{
    QGridLayout *grid = new QGridLayout();
    QLabel *l = new QLabel("Up:", this);
    l->setAlignment(Qt::AlignRight);
    grid->addWidget(l, 0, 0);

    l = new QLabel("Down:", this);
    l->setAlignment(Qt::AlignRight);
    grid->addWidget(l, 1, 0);

    upPin = new AvrOutPinWidget(this);
    grid->addWidget(upPin, 0, 1);

    downPin = new AvrOutPinWidget(this);
    grid->addWidget(downPin, 1, 1);

    grid->setColumnStretch(0, 1);

    QVBoxLayout *vbox = new QVBoxLayout();
    state = new QLabel(stateString(0).arg(tr("CPU")));
    state->setAlignment(Qt::AlignHCenter);
    state->setTextFormat(Qt::RichText);

    vbox->addLayout(grid);
    vbox->addWidget(state);

    setLayout(vbox);
}

void AvrWidget::stateChanged(int state)
{
    this->state->setText(stateString(state).arg(tr("CPU")));
}

void AvrWidget::up(bool active)
{
    upPin->setActive(active);
}

void AvrWidget::down(bool active)
{
    downPin->setActive(active);
}

