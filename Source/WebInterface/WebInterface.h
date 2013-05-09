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

private:
	void run();
	void initialiseWebserver();

private:
	struct mg_context *_ctx;
	Host* _host;

};

void WebInterface_websocket_ready_handler(struct mg_connection *conn);
int WebInterface_websocket_data_handler(struct mg_connection *conn);


#endif /* WEBINTERFACE_H_ */
