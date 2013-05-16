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

void WebInterface::populateCommandMap()
{
	_commandNameMap.set("play",HC_TRANSPORT_PLAY);
	_commandNameMap.set("stop",HC_TRANSPORT_STOP);
	_commandNameMap.set("pause",HC_TRANSPORT_PAUSE);
	_commandNameMap.set("rewind",HC_TRANSPORT_REWIND);
	_commandNameMap.set("record",HC_TRANSPORT_RECORD);

	_commandNameMap.set("setnext",HC_PATTERN_SET_NEXT);
	_commandNameMap.set("midievent",HC_MIDI_EVENT);

}

WebInterface::WebInterface(Host* host) : Thread ("WebInterface")
{
	_ctx = nullptr;
	_conn = nullptr;
	_host = host;

	populateCommandMap();
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

void WebInterface::parseCommand(char* szCommand)
{
	cJSON* pJson = cJSON_Parse(szCommand);
	bool bOk = true;

	cJSON* args = nullptr;
	int argarray[8];
	cJSON* event = nullptr;

	if (pJson)
	{
		event = cJSON_GetObjectItem(pJson,"event");
		if (!event || event->type!=cJSON_String)
		{
			DBG("event null or not string");
			bOk = false;
		}

		args = cJSON_GetObjectItem(pJson,"args");
		if (!args || args->type!=cJSON_Array)
		{
			DBG("args null or not array");
			bOk = false;
		}

		if (bOk)
		{
			for (int i=0;i<cJSON_GetArraySize(args);i++)
			{
				cJSON* arg = cJSON_GetArrayItem(args,i);
				if (!arg || arg->type!=cJSON_Number)
				{
					DBG("at least one argument is NULL or NaN");
					bOk = false;
				}
				else
				{
					argarray[i]=arg->valueint;
				}
			}
		}

		// TODO
		// get event
		// get args, which must all be ints
	}
	else
	{
		DBG("cannot parse JSON");
		bOk = false;
	}

	if (bOk)
	{
		String name = event->valuestring;

		if (!_commandNameMap.contains(name))
		{
			DBG("event not known");
			bOk = false;
		}
		else
		{
			HostEventName hen = _commandNameMap[name];
			HostEvent event = HostEventFactory::event(hen,
					cJSON_GetArraySize(args),argarray);

			_host->event(event);
		}
	}
}

void WebInterface::send(GrooveEvent& event)
{
	// send event to websocket
}
