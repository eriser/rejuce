/*
 * Groovebox.h
 *
 *  Created on: 16 May 2013
 *      Author: gnvbx
 */

#ifndef GROOVEBOX_H_
#define GROOVEBOX_H_

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Host/Host.h"
#include "../Host/HostEventListener.h"
#include "GrooveEventListener.h"
#include "GrooveboxInterface.h"
#include "../Sequencer/Sequencer.h"


class Groovebox : public GrooveEventListener, public HostEventListener {
public:
	Groovebox(Host* pHost,GrooveboxInterface* pInterface);
	virtual ~Groovebox();

	void start();
	void stop();

	// how we get events in from the interface
	void onGrooveEvent(GrooveEvent& event);

	// how we get events from the host
	void onHostEvent(HostEvent& event);

private:
	void setLedPos(HostEvent& event);
	void setSemiLedsOff();
	void setTransportLeds();
	void handleKeyboardButton(bool bDown,GrooveControlName control);

	GrooveboxInterface* _interface;
	Host* _host;

	TransportState _transportState;

	int _controlState[GC_SIZE];

	int _currentChannel=0;
	int _transposeOffset=60;
};

#endif /* GROOVEBOX_H_ */
