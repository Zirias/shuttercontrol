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

void AvrContainer::init()
{
    elf_firmware_t *fw = new elf_firmware_t;
    elf_read_firmware("shutterctl.afx", fw);
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

    emit initialized();
}

void AvrContainer::run()
{
    int oldstate = avr->state;
    int state = oldstate;
    emit stateChanged(state);

    while (state != cpu_Done && state != cpu_Crashed)
    {
	QCoreApplication::processEvents();
	state = avr_run(avr);
	if (state != oldstate)
	{
	    emit stateChanged(state);
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

