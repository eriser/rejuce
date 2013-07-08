/*
 * WebInterface.cpp
 *
 *  Created on: 8 May 2013
 *      Author: gnvbx
 */

#include "WebInterface.h"
#include "../../JuceLibraryCode/JuceHeader.h"
#include <arpa/inet.h>
#include "../Groovebox/GrooveEvent.h"
#include "cJSON.h"
#include "mongoose.h"

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
	char outbuf[128];
	outbuf[0] = 0;
	mg_write(_conn, outbuf, 1);

	signalThreadShouldExit();
	waitForThreadToExit(-1);
}

void WebInterface::run()
{
	// start webserver
	initialiseWebserver();

	DBG("webserver running\n");

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
	callbacks.open_file = WebInterface_open_file;

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

int WebInterface::getScreenJsonSize()
{
	return _screenJson.length();
}

char* WebInterface::getScreenJson()
{
	return (char*)_screenJson.getCharPointer().getAddress();
}

void WebInterface_websocket_ready_handler(struct mg_connection *conn)
{
	WebInterface* pThis = (WebInterface*)mg_get_user_info(conn);

//	unsigned char buf[40];
//	buf[0] = 0x81;
//	buf[1] = snprintf((char *) buf + 2, sizeof(buf) - 2, "%s", "GNUVEb0x");
//	mg_write(conn, buf, 2 + buf[1]);
}

static const char* WebInterface_open_file(const struct mg_connection *conn,const char *path, size_t *size)
{
	WebInterface* pThis = (WebInterface*) mg_get_user_info((struct mg_connection *)conn);

	if (!strcmp(path, "./screens.json"))
	{
		*size = pThis->getScreenJsonSize();
		return pThis->getScreenJson();
	}
	return NULL;
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
	/*printf("send ack\n");

	// acknowledge
	unsigned char outbuf[40];
	outbuf[0] = 0x81;
	outbuf[1] = snprintf((char *) outbuf + 2, sizeof(outbuf) - 2, "%s", "ACK");
	mg_write(conn, outbuf, 2 + outbuf[1]);

	printf("ack ok\n");*/

	return 1; // return 0 close websocket
}

void WebInterface::populateMaps()
{
	int i;

	for (i=0;i<GE_SIZE;i++)
	{
		const char* name = GrooveEvent_getNameString((GrooveEventName)i);
		_eventMap.set(String(name),(GrooveEventName)i);
	}

	for (i=0;i<GC_SIZE;i++)
	{
		const char* name = GrooveControl_getNameString((GrooveControlName)i);
		_controlMap.set(String(name),(GrooveControlName)i);
	}
}

void WebInterface::parseCommand(char* szCommand)
{
	cJSON* pJson = cJSON_Parse(szCommand);
	cJSON* pj;
	bool bOk = true;

	if (!pJson)
		return;

	GrooveEventName name=GE_INVALID;
	GrooveControlName control;
	int value;

	// event
	pj = cJSON_GetObjectItem(pJson,"event");
	if (bOk && pj && pj->type==cJSON_String && pj->valuestring && _eventMap.contains(pj->valuestring))
	{
		name = _eventMap[pj->valuestring];
	}
	else bOk = false;

	// thing
	pj = cJSON_GetObjectItem(pJson,"control");
	if (bOk && pj && pj->type==cJSON_String && pj->valuestring && _controlMap.contains(pj->valuestring))
	{
		control = _controlMap[pj->valuestring];
	}
	else bOk = false;

	pj = cJSON_GetObjectItem(pJson,"argv");
	if (bOk && pj && pj->type==cJSON_Number)
	{
		value = pj->valueint;
	}

	if (bOk && name!=GE_INVALID)
	{
		GrooveEvent event = GrooveEvent(name,control,value);
		_pOutListener->onGrooveEvent(event);
	}
}

void WebInterface::onGrooveEvent(GrooveEvent& event)
{
	// send event to websocket
	unsigned char outbuf[128];

	cJSON* pJson = cJSON_CreateObject();

	const char* eventName = nullptr;
	eventName = GrooveEvent_getNameString(event.getEvent());
	cJSON_AddItemToObject(pJson,"event",cJSON_CreateString(eventName));

	const char* controlName = nullptr;
	controlName = GrooveControl_getNameString(event.getControl());
	cJSON_AddItemToObject(pJson,"control",cJSON_CreateString(controlName));

	cJSON_AddItemToObject(pJson,"argv",cJSON_CreateNumber(event.getInt()));
	cJSON_AddItemToObject(pJson,"string",cJSON_CreateString(event.getString()));

	char* c = cJSON_PrintUnformatted(pJson);

	printf ("send: %s\n",c);

	outbuf[0] = 0x81;
	outbuf[1] = snprintf((char *) outbuf + 2, sizeof(outbuf) - 2, "%s", c);
	mg_write(_conn, outbuf, 2 + outbuf[1]);

	free(c);
	cJSON_Delete(pJson);
}

void WebInterface::setScreenManager(ScreenManager* manager)
{
	_screenManager = manager;

	generateScreenJson();
}

void WebInterface::generateScreenJson()
{
	cJSON* jScreens = cJSON_CreateArray();

	for (int nScreen=0;nScreen<_screenManager->getScreenCount();nScreen++)
	{
		// set current screen
		_screenManager->setCurrentScreen(nScreen);

		// create screen object and add it
		cJSON* jScreen = cJSON_CreateObject();
		cJSON_AddItemToArray(jScreens,jScreen);
		cJSON_AddStringToObject(jScreen,"name",_screenManager->getCurrentScreenName().getCharPointer());

		// create pages object and add it to screen
		cJSON* jPages = cJSON_CreateArray();
		cJSON_AddItemToObject(jScreen,"pages",jPages);

		for (int nPage=0;nPage<_screenManager->getPageCount();nPage++)
		{
			// set current page
			_screenManager->setCurrentPage(nPage);

			// create the page object and add it to pages
			cJSON* jPage = cJSON_CreateArray();
			cJSON_AddItemToObject(jPages,"pages",jPage);

			for (int nControl=0;nControl<_screenManager->getControlCount();nControl++)
			{
				// create the control object and add it to page
				cJSON* jControl = cJSON_CreateObject();
				cJSON_AddStringToObject(jControl,"name",GrooveControl_getNameString(_screenManager->getControl(nControl)));
				cJSON_AddNumberToObject(jControl,"id",_screenManager->getControl(nControl));
				cJSON_AddItemToArray(jPage,jControl);
			}
		}

	}

	_screenManager->setCurrentScreen(0);

	_screenJson = cJSON_PrintUnformatted(jScreens);
	cJSON_Delete(jScreens);
}
