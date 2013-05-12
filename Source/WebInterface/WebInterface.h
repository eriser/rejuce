/*
 * WebInterface.h
 *
 *  Created on: 8 May 2013
 *      Author: gnvbx
 */

#ifndef WEBINTERFACE_H_
#define WEBINTERFACE_H_

#include "../Host/Host.h"
#include "mongoose.h"

class WebInterface : private Thread {

public:
	WebInterface(Host* host);
	virtual ~WebInterface();

	void start();
	void stop();

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
	Host* _host;

	HashMap <String,HostEventName> _commandNameMap;
};

void WebInterface_websocket_ready_handler(struct mg_connection *conn);
int WebInterface_websocket_data_handler(struct mg_connection *conn);


#endif /* WEBINTERFACE_H_ */
