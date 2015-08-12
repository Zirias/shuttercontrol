#ifndef SCTL_AVRCONTAINER_H
#define SCTL_AVRCONTAINER_H

#include <QObject>

class AvrPinConnector;
struct avr_t;

class AvrContainer: public QObject
{
    Q_OBJECT

    public:
	AvrContainer(int address);
	AvrPinConnector *getConnector();

    public slots:
	void run();
	void stop();

    signals:
	void started();
	void finished();
	void error(const QString &message);
	void stateChanged(const char *state);

    private:
	static const char *stateName(int state);
	int address;
	avr_t *avr;
	AvrPinConnector *cn;
};

#endif
