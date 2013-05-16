/*
 * Groovebox.h
 *
 *  Created on: 16 May 2013
 *      Author: gnvbx
 */

#ifndef GROOVEBOX_H_
#define GROOVEBOX_H_

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../WebInterface/WebInterface.h"
#include "../Host/Host.h"
#include "GrooveEvent.h"

class Groovebox {
public:
	Groovebox(Host* pHost);
	virtual ~Groovebox();

	void start();
	void stop();

private:
	void out(GrooveEvent &event);

	WebInterface* _webIterface;
	Host* _host;
};

#endif /* GROOVEBOX_H_ */
