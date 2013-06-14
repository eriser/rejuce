/*
 * SequencerCommand.cpp
 *
 *  Created on: 14 Apr 2013
 *      Author: gboxdev
 */

#include "GrooveEvent.h"

GrooveEvent::GrooveEvent()
{
	_isString=false;
	_event=GE_INVALID;
	_control=GC_INVALID;
	_value[0]=0;
	_value[1]=0;
	_value[2]=0;
	_value[3]=0;
}
GrooveEvent::GrooveEvent(GrooveEventName event,GrooveControlName control,int value)
{
	_event=event;
	_control=control;
	_isString=false;
	memcpy(_value,&value,4);
}
GrooveEvent::GrooveEvent(GrooveEventName event,GrooveControlName control,char* value)
{
	_event=event;
	_control=control;
	_isString=true;
	strncpy(_value,value,31);
	_value[31]=0;
}
GrooveEvent::GrooveEvent(GrooveEventName event,GrooveControlName control,String &value)
{
	_event=event;
	_control=control;
	_isString=true;
	strncpy(_value,value.getCharPointer(),31);
	_value[31]=0;
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
 bool GrooveEvent::isString()
{
	return _isString;
}
 bool GrooveEvent::isInt()
{
	return !_isString;
}
 int GrooveEvent::getAsInt()
{
	int i;
	memcpy(&i,_value,4);
	return i;
}
 char* GrooveEvent::getAsString()
{
	return _value;
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


