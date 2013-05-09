/*
 * WebInterface.cpp
 *
 *  Created on: 8 May 2013
 *      Author: gnvbx
 */

#include "WebInterface.h"
#include "../../JuceLibraryCode/JuceHeader.h"

WebInterface::WebInterface(Host* host) : Thread ("WebInterface")
{
	_ctx = nullptr;
	_host = host;
}

WebInterface::~WebInterface()
{


}

void WebInterface::start()
{
	startThread();
}

void WebInterface::stop()
{
	signalThreadShouldExit();
	waitForThreadToExit(-1);
}

void WebInterface::run()
{
	// start webserver
	initialiseWebserver();

	// block here until stop running
	while (!this->threadShouldExit())
		Thread::sleep(1000);

	// close webserver
	mg_stop(_ctx);

	DBG("dropping out of WebInterface thread");
}

void WebInterface::initialiseWebserver()
{
	struct mg_callbacks callbacks;

	const char *options[] = {
		"listening_ports", "8080",
		"document_root", "www",
		NULL
	};

	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.websocket_ready = WebInterface_websocket_ready_handler;
	callbacks.websocket_data = WebInterface_websocket_data_handler;
	_ctx = mg_start(&callbacks,this, options);
}

void WebInterface_websocket_ready_handler(struct mg_connection *conn)
{
	WebInterface* pThis = (WebInterface*)mg_get_user_info(conn);

	unsigned char buf[40];
	buf[0] = 0x81;
	buf[1] = snprintf((char *) buf + 2, sizeof(buf) - 2, "%s", "server ready");
	mg_write(conn, buf, 2 + buf[1]);
}

int WebInterface_websocket_data_handler(struct mg_connection *conn)
{
	WebInterface* pThis = (WebInterface*)mg_get_user_info(conn);

	// read the data sent to us
	MemoryBlock data;
	char temp[1024];
	int tempSize =0;

	while ( (tempSize = mg_read(conn,temp,1024)) )
	{
		data.append(temp,tempSize);

		if (data.getSize() > 10000)
			break;
	}

	printf("rcv: [%.*s]\n", (int)data.getSize(), (char*)data.getData());

	// make up some crap response
	unsigned char buf[40];
	buf[0] = 0x81;
	buf[1] = snprintf((char *) buf + 2, sizeof(buf) - 2, "%s", "stuff and nonsense");
	mg_write(conn, buf, 2 + buf[1]);

	return 1; // return 0 close websocket
}

