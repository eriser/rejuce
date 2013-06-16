/*
 * SequencerCommand.cpp
 *
 *  Created on: 14 Apr 2013
 *      Author: gboxdev
 */

#include "GrooveEvent.h"

GrooveEvent::GrooveEvent()
{
	_event=GE_INVALID;
	_control=GC_INVALID;
	_value=0;
	_string[0]=0;
}
GrooveEvent::GrooveEvent(GrooveEventName event,GrooveControlName control,int value)
{
	_event=event;
	_control=control;
	_value=value;
	_string[0]=0;
}
GrooveEvent::GrooveEvent(GrooveEventName event,GrooveControlName control,int value,char* string)
{
	_event=event;
	_control=control;
	_value=value;
	strncpy(_string,string,31);
}
GrooveEvent::~GrooveEvent()
{

}
 GrooveEventName GrooveEvent::getEvent()
{
	return _event;
}
 GrooveControlName GrooveEvent::getControl()
{
	return _control;
}
int GrooveEvent::getInt()
{
	return _value;
}
 char* GrooveEvent::getString()
{
	return _string;
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


