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

GrooveEvent GrooveEventFactory::event(GrooveEventName name)
{
	GrooveEvent sc;
	sc.name = name;
	sc.argc = 0;
	validate(&sc);
	return sc;
}

GrooveEvent GrooveEventFactory::event(GrooveEventName name,int arg0)
{
	GrooveEvent sc;
	sc.name = name;
	sc.argc = 1;
	sc.argv[0]=arg0;
	validate(&sc);
	return sc;
}

GrooveEvent GrooveEventFactory::event(GrooveEventName name,int arg0,int arg1)
{
	GrooveEvent sc;
	sc.name = name;
	sc.argc = 2;
	sc.argv[0]=arg0;
	sc.argv[1]=arg1;
	validate(&sc);
	return sc;
}

GrooveEvent GrooveEventFactory::event(GrooveEventName name,int argc,int* args)
{
	GrooveEvent sc;
	sc.name = name;
	sc.argc = argc;
	for (int i=0;i<argc;i++)
	{
		sc.argv[i]=args[i];
	}
	validate(&sc);
	return sc;
}

GrooveEvent GrooveEventFactory::event(GrooveEventName name,int arg0,int arg1,int arg2)
{
	GrooveEvent sc;
	sc.name = name;
	sc.argc = 2;
	sc.argv[0]=arg0;
	sc.argv[1]=arg1;
	sc.argv[2]=arg2;
	validate(&sc);
	return sc;
}


void GrooveEventFactory::validate(GrooveEvent* c)
{

}




