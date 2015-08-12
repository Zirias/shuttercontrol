#include "avrcontainer.h"

#include <QCoreApplication>
#include <sim_avr.h>
#include <sim_elf.h>
#include "avrpinconnector.h"

AvrContainer::AvrContainer(int address)
    : QObject(), address(address)
{}

AvrPinConnector *AvrContainer::getConnector()
{
    return cn;
}

void AvrContainer::run()
{
    elf_firmware_t *fw = new elf_firmware_t;
    elf_read_firmware("shutterctl_attiny84/shutterctl.afx", fw);
    avr = avr_make_mcu_by_name(fw->mmcu);
    if (!avr)
    {
	emit error(QString(tr("AVR '%1' is unknown")).arg(fw->mmcu));
	emit finished();
	return;
    }
    fw->eeprom[0] = (fw->eeprom[0] & 0xf0) | (address & 0x0f);
    avr_init(avr);
    avr_load_firmware(avr, fw);
    delete fw;

    cn = new AvrPinConnector(avr);

    emit started();
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
    delete cn;
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

