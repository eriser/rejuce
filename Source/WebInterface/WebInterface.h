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
#include "../Groovebox/GrooveEvent.h"

class WebInterface : private Thread, public GrooveboxInterface {

public:
	WebInterface();
	virtual ~WebInterface();

	void start();
	void stop();

	// called by groovebox class to send a grooveevent to the interface
	void onGrooveEvent(GrooveEvent& event);

	// triggered by interface to send grooveevent to groovebox class
	void parseCommand(char* szCommand);

	struct mg_connection* getConnection();
	void setConnection(mg_connection* conn);

	void setScreenManager(ScreenManager* manager);

	int getScreenJsonSize();
	char* getScreenJson();

private:
	void run();
	void initialiseWebserver();
	void populateMaps();
	void generateScreenJson();

private:
	struct mg_context* _ctx;
	struct mg_connection* _conn;

	HashMap<String,GrooveEventName> _eventMap;
	HashMap<String,GrooveControlName> _controlMap;

	String _screenJson;

};

void WebInterface_websocket_ready_handler(struct mg_connection *conn);
int WebInterface_websocket_data_handler(struct mg_connection *conn);
static const char *WebInterface_open_file(const struct mg_connection *conn,const char *path, size_t *size);


#endif /* WEBINTERFACE_H_ */
