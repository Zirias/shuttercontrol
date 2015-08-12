#include "avrcontainer.h"

#include <QThread>
#include <QCoreApplication>
#include <sim_avr.h>
#include <sim_elf.h>

AvrContainer::AvrContainer(const QString &fwpath)
    : QObject(), fwpath(fwpath)
{
    QThread *thread = new QThread();
    moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(run()));
    connect(this, SIGNAL(finished()), thread, SLOT(quit()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void AvrContainer::run()
{
    elf_firmware_t *fw = new elf_firmware_t;
    elf_read_firmware(fwpath.toLocal8Bit(), fw);
    avr = avr_make_mcu_by_name(fw->mmcu);
    if (!avr)
    {
	emit error(QString(tr("AVR '%1' is unknown")).arg(fw->mmcu));
	emit finished();
	return;
    }
    avr_init(avr);
    avr_load_firmware(avr, fw);
    delete fw;

    int oldstate = avr->state;
    int state = oldstate;
    emit stateChanged(stateName(state));

    while (state != cpu_Done && state != cpu_Crashed)
    {
	QCoreApplication::processEvents();
	state = avr_run(avr);
	if (state != oldstate)
	{
	    emit stateChanged(stateName(state));
	    oldstate = state;
	}
    }

    emit finished();
}

void AvrContainer::stop()
{
    emit finished();
}

const char *AvrContainer::stateName(int state)
{
    switch (state)
    {
	case cpu_Limbo: return "limbo";
	case cpu_Stopped: return "stopped";
	case cpu_Running: return "running";
	case cpu_Sleeping: return "sleeping";
	case cpu_Step: return "step";
	case cpu_StepDone: return "step done";
	case cpu_Done: return "done";
	case cpu_Crashed: return "crashed";
    }
}

