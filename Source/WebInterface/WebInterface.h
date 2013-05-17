/*
 * WebInterface.h
 *
 *  Created on: 8 May 2013
 *      Author: gnvbx
 */

#ifndef WEBINTERFACE_H_
#define WEBINTERFACE_H_

#include "mongoose.h"
#include "../Groovebox/Groovebox.h"

class WebInterface : private Thread {

public:
	WebInterface(Groovebox* pBox);
	virtual ~WebInterface();

	void start();
	void stop();

	// called by groovebox class to send a grooveevent to the interface
	void send(GrooveEvent& event);

	// triggered by interface to send grooveevent to groovebox class
	void parseCommand(char* szCommand);

	struct mg_connection* getConnection();
	void setConnection(mg_connection* conn);

private:
	void run();
	void initialiseWebserver();
	void populateCommandMap();

private:
	struct mg_context* _ctx;
	struct mg_connection* _conn;
	Groovebox* _groovebox;

	HashMap <String,HostEventName> _grooveEventMap;
};

void WebInterface_websocket_ready_handler(struct mg_connection *conn);
int WebInterface_websocket_data_handler(struct mg_connection *conn);


#endif /* WEBINTERFACE_H_ */
