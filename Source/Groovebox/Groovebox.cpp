/*
 * Groovebox.cpp
 *
 *  Created on: 16 May 2013
 *      Author: gnvbx
 */

#include "Groovebox.h"

Groovebox::Groovebox(Host* pHost,GrooveboxInterface* pInterface) :GrooveEventListener(), HostEventListener()
{
	_host = pHost;
	_interface = pInterface;
	_host->setHostEventListener(this);
}

Groovebox::~Groovebox()
{
	delete _interface;
}

void Groovebox::start()
{
	_interface->start();
}

void Groovebox::stop()
{
	_interface->stop();
}

void Groovebox::onGrooveEvent(GrooveEvent& event)
{
	// events coming in from interface. update local state or fire host event
	switch (event.name)
	{
	case GC_BUTTON_DOWN:
	{
		switch (event.argv[0])
		{
			case GT_STOP:
				_host->event(HostEventFactory::event(HC_TRANSPORT_STOP));
				break;
			case GT_PLAY:
				_host->event(HostEventFactory::event(HC_TRANSPORT_PLAY));
				break;
			case GT_PAUSE:
				_host->event(HostEventFactory::event(HC_TRANSPORT_PAUSE));
				break;
			case GT_REWIND:
				_host->event(HostEventFactory::event(HC_TRANSPORT_REWIND));
				break;
			case GT_RECORD:
				_host->event(HostEventFactory::event(HC_TRANSPORT_RECORD));
				break;

			default:
				// TODO: something else
				break;
		}
		break;
	}
	case GC_BUTTON_UP:

		break;
	case GC_KNOB:

		break;
	}
}

void Groovebox::onHostEvent(HostEvent& event)
{
	switch (event.name)
	{
		case HC_OUT_LEDPOS:
		{
			GrooveEvent ge = GrooveEventFactory::event(GC_OUT_LEDPOS,event.argv[0]);
			_interface->onGrooveEvent(ge);
			break;
		}
		default:
			break;
	}
}

void Groovebox::out(GrooveEvent &event)
{
	// feeds the event to the interface
	// at the moment a webinterface

	_interface->onGrooveEvent(event);
}

