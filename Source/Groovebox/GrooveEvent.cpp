/*
 * SequencerCommand.cpp
 *
 *  Created on: 14 Apr 2013
 *      Author: gboxdev
 */

#include "GrooveEvent.h"

GrooveEvent GrooveEventFactory::event(GrooveEvent* c)
{
	GrooveEvent sc;
	memcpy(&sc,c,sizeof(GrooveEvent));
	return sc;
}

GrooveEvent GrooveEventFactory::event(GrooveEventName event,GrooveControlName control,int argv)
{
	GrooveEvent sc;
	sc.event = event;
	sc.control = control;
	sc.argv = argv;
	return sc;
}

const char* GrooveEvent_getNameString(GrooveEventName name)
{
	// Use Preprocessor to expand data into switch statement cases
	switch(name)
	{
		#define X(Enum, String) case Enum:  return String; break;
			X_GROOVEEVENTS
		#undef X

	default: return nullptr; break;
	}
	return nullptr;
}

const char* GrooveControl_getNameString(GrooveControlName name)
{
	// Use Preprocessor to expand data into switch statement cases
	switch(name)
	{
		#define X(Enum, String) case Enum:  return String; break;
			X_GROOVECONTROLS
		#undef X

	default: return nullptr; break;
	}
	return nullptr;
}


