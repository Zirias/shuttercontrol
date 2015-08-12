#ifndef SCTL_AVRCONTAINER_H
#define SCTL_AVRCONTAINER_H

#include <QObject>

class QString;
struct avr_t;

class AvrContainer: public QObject
{
    Q_OBJECT

    public:
	AvrContainer(const QString &fwpath);

    public slots:
	void run();
	void stop();

    signals:
	void finished();
	void error(const QString &message);
	void stateChanged(const char *state);

    private:
	static const char *stateName(int state);
	const QString &fwpath;
	avr_t *avr;
	int lines;
};

#endif
