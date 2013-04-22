/*
 * SequencerCommand.cpp
 *
 *  Created on: 14 Apr 2013
 *      Author: gboxdev
 */

#include "HostEvent.h"

HostEvent HostEventFactory::command(HostEvent* c)
{
	HostEvent sc;
	memcpy(&sc,c,sizeof(HostEvent));
	return sc;
}

HostEvent HostEventFactory::command(HostEventName name)
{
	HostEvent sc;
	sc.name = name;
	sc.argc = 0;
	validate(&sc);
	return sc;
}

HostEvent HostEventFactory::command(HostEventName name,int arg0)
{
	HostEvent sc;
	sc.name = name;
	sc.argc = 1;
	sc.argv[0]=arg0;
	validate(&sc);
	return sc;
}

HostEvent HostEventFactory::command(HostEventName name,int arg0,int arg1)
{
	HostEvent sc;
	sc.name = name;
	sc.argc = 2;
	sc.argv[0]=arg0;
	sc.argv[1]=arg1;
	validate(&sc);
	return sc;
}

HostEvent HostEventFactory::command(HostEventName name,int arg0,int arg1,int arg2)
{
	HostEvent sc;
	sc.name = name;
	sc.argc = 2;
	sc.argv[0]=arg0;
	sc.argv[1]=arg1;
	sc.argv[2]=arg2;
	validate(&sc);
	return sc;
}


void HostEventFactory::validate(HostEvent* c)
{
	switch (c->argc)
	{
		case 0:
		if (c->name < HOSTEVENT_0_ARGS || c->name>= HOSTEVENT_1_ARGS)
			c->name = HC_INVALID;
		break;

		case 1:
		if (c->name < HOSTEVENT_1_ARGS || c->name>= HOSTEVENT_2_ARGS)
			c->name = HC_INVALID;
		break;

		case 2:
		if (c->name < HOSTEVENT_2_ARGS || c->name>= HOSTEVENT_3_ARGS)
			c->name = HC_INVALID;
		break;

		default:
		c->name = HC_INVALID;
		break;
	}
}




