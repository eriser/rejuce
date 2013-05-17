/*
 * WebInterface.h
 *
 *  Created on: 8 May 2013
 *      Author: gnvbx
 */

#ifndef GBINTERFACE_H_
#define GBINTERFACE_H_

#include "GrooveEvent.h"

class GrooveboxInterface {

public:
	virtual void start() =0;
	virtual void stop() =0;

	virtual void send(GrooveEvent& event);

	// triggered by interface to send grooveevent to groovebox class
	virtual void parseCommand(char* szCommand);

	struct mg_connection* getConnection();
	void setConnection(mg_connection* conn);

};


#endif /* WEBINTERFACE_H_ */
