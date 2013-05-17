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
#include "GrooveEvent.h"


class Groovebox {
public:
	Groovebox(Host* pHost,GrooveboxInterface* pInterface);
	virtual ~Groovebox();

	void start();
	void stop();

	// called by interface to do a grooveevent
	void event(GrooveEvent& event);

private:
	// called by us to send an grooveevent out
	void out(GrooveEvent &event);

	GrooveboxInterface* _webIterface;
	Host* _host;
};

#endif /* GROOVEBOX_H_ */
