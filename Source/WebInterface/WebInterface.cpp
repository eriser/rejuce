/*
 * WebInterface.cpp
 *
 *  Created on: 8 May 2013
 *      Author: gnvbx
 */

#include "WebInterface.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include <arpa/inet.h>
#include "cJSON.h"

WebInterface::WebInterface() : Thread ("WebInterface")
{
	_ctx = nullptr;
	_conn = nullptr;

	populateMaps();
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

struct mg_connection* WebInterface::getConnection()
{
	return _conn;
}

void WebInterface::setConnection(struct mg_connection* conn)
{
	_conn = conn;
}

void WebInterface_websocket_ready_handler(struct mg_connection *conn)
{
	WebInterface* pThis = (WebInterface*)mg_get_user_info(conn);

	unsigned char buf[40];
	buf[0] = 0x81;
	buf[1] = snprintf((char *) buf + 2, sizeof(buf) - 2, "%s", "GNUVEb0x");
	mg_write(conn, buf, 2 + buf[1]);
}

int WebInterface_websocket_data_handler(struct mg_connection *conn)
{
	WebInterface* pThis = (WebInterface*)mg_get_user_info(conn);

	// only allow 1 connection..
	if (conn!=pThis->getConnection())
	{
		if (pThis->getConnection())
		{
			mg_close_connection(pThis->getConnection());
		}

		pThis->setConnection(conn);
	}

	// Read the message from the client
	// For now we will assume that this is a string of text
	// Read in the header and size
	unsigned char header[10];
	int totalRead = 0;
	while ( totalRead < 2 )
	{
	  int bytesRead = mg_read(conn, header+totalRead, 2-totalRead);
	  if ( bytesRead <=0 )
	    return nullptr;
	  totalRead += bytesRead;
	}
	// Check the data received
	if ( header[0] != 0x81 )
	  return nullptr;

	long long messageLength = header[1] & 127;
	int maskLength = (header[1] & 128) ? 4 : 0;
	if ( messageLength == 126 ) // Large message
	{
	  totalRead = 0;
	  while ( totalRead < 2 )
	  {
	    int bytesRead = mg_read(conn, header+totalRead, 2-totalRead);
	    if ( bytesRead <=0 )
	      return nullptr;
	    totalRead += bytesRead;
	  }
	  messageLength = (((int) header[0]) << 8) + header[1];
	}
	else if ( messageLength > 126 ) // Very large message
	{
	  totalRead = 0;
	  while ( totalRead < 8 )
	  {
	    int bytesRead = mg_read(conn, header+totalRead, 8-totalRead);
	    if ( bytesRead <=0 )
	      return nullptr;
	    totalRead += bytesRead;
	  }
	  messageLength =  (((long long) htonl(*(int*)&header[0])) << 32) | htonl(*(int*)&header[4]);
	}

	// Now read the data
	unsigned char* buf = new unsigned char[messageLength+maskLength];
	totalRead = 0;
	while ( totalRead < messageLength+maskLength )
	{
	  int bytesRead = mg_read(conn, buf+totalRead, messageLength+maskLength-totalRead);
	  if ( bytesRead <=0 )
	  {
	    delete[] buf;
	    return nullptr;
	  }
	  totalRead += bytesRead;
	}
	char* message = new char[messageLength+1];
	for ( int i=0; i<messageLength; ++i )
	{
	  int x0r = (maskLength==0 ? 0 : buf[i%4]);
	  message[i] = buf[i+maskLength] ^ x0r;
	}

	message[messageLength] = '\0';
	delete[] buf;

	printf("rec: %s\n",message);

	pThis->parseCommand(message);

	delete[] message;

	/////////////////////////////////////////
printf("send ack\n");

	// acknowledge
	unsigned char outbuf[40];
	outbuf[0] = 0x81;
	outbuf[1] = snprintf((char *) outbuf + 2, sizeof(outbuf) - 2, "%s", "ACK");
	mg_write(conn, outbuf, 2 + outbuf[1]);

	printf("ack ok\n");

	return 1; // return 0 close websocket
}

void WebInterface::populateMaps()
{
	int i;

	for (i=0;i<GE_SIZE;i++)
	{
		const char* name = GrooveEvent_getNameString((GrooveEventName)i);
		_eventMap.set(String(name),i);
	}

	for (i=0;i<GL_SIZE;i++)
	{
		const char* name = GrooveLed_getNameString((GrooveLedName)i);
		_ledMap.set(String(name),i);
	}

	for (i=0;i<GC_SIZE;i++)
	{
		const char* name = GrooveControl_getNameString((GrooveControlName)i);
		_controlMap.set(String(name),i);
	}
}

void WebInterface::parseCommand(char* szCommand)
{
	cJSON* pJson = cJSON_Parse(szCommand);

	GrooveEvent event;
	memset(&event,0,sizeof(GrooveEvent));

	// event
	cJSON* pj = cJSON_GetObjectItem(pJson,"event");
	if (pj && pj->type==cJSON_String && pj->valuestring)
	{
		event.event = _eventMap[pj->valuestring];
	}

	// thing
	cJSON* pj = cJSON_GetObjectItem(pJson,"event");
	if (pj && pj->type==cJSON_String && pj->valuestring)
	{
		event.event = _eventMap[pj->valuestring];
	}

	cJSON* argv = cJSON_GetObjectItem(pJson,"argv");
	if (argv && argv->type==cJSON_Array)
	{
		event.argc = cJSON_GetArraySize(argv);
		for (int i=0;i<event.argc;i++)
		{
			cJSON* arg = cJSON_GetArrayItem(argv,i);
			if (arg && arg->type==cJSON_Number)
			{
				event.argv[i] = arg->valueint;
			}
		}
	}

	if (event.name!=GC_INVALID)
	{
		_pOutListener->onGrooveEvent(event);
	}
}

void WebInterface::onGrooveEvent(GrooveEvent& event)
{
	// send event to websocket
	unsigned char outbuf[128];

	cJSON* pJson = cJSON_CreateObject();
	const char* eventName = nullptr;
	switch (event.name)
	{
	case GC_BUTTON_DOWN: eventName = "GC_BUTTON_DOWN"; break;
	case GC_BUTTON_UP: eventName = "GC_BUTTON_UP"; break;
	case GC_KNOB: eventName = "GC_KNOB"; break;
	case GC_OUT_LEDPOS: eventName = "GC_OUT_LEDPOS"; break;
	default:
		return;
		break;
	}
	cJSON_AddItemToObject(pJson,"name",cJSON_CreateString(eventName));
	cJSON* pArray = cJSON_CreateIntArray(event.argv,event.argc);
	cJSON_AddItemToObject(pJson,"argv",pArray);

	printf("=====%d\n",event.name);
	char* c = cJSON_PrintUnformatted(pJson);

	printf ("send: %s\n",c);

	outbuf[0] = 0x81;
	outbuf[1] = snprintf((char *) outbuf + 2, sizeof(outbuf) - 2, "%s", c);
	mg_write(_conn, outbuf, 2 + outbuf[1]);

	free(c);
	cJSON_Delete(pJson);
}

