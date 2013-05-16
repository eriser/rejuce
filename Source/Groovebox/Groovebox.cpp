/*
 * Groovebox.cpp
 *
 *  Created on: 16 May 2013
 *      Author: gnvbx
 */

#include "Groovebox.h"

Groovebox::Groovebox(Host* pHost)
{
	_host = pHost;
	_webIterface = new WebInterface(pHost);
}

Groovebox::~Groovebox()
{
	delete _webIterface;
}

void Groovebox::start()
{
	_webIterface->start();
}

void Groovebox::stop()
{
	_webIterface->stop();
}

void Groovebox::out(GrooveEvent &event)
{
	// feeds the event to the interface
	// at the moment a webinterface

	_webIterface->send(event);
}

