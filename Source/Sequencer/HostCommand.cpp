/*
 * SequencerCommand.cpp
 *
 *  Created on: 14 Apr 2013
 *      Author: gboxdev
 */

#include "HostCommand.h"

HostCommand HostCommandFactory::command(HostCommand* c)
{
	HostCommand sc;
	memcpy(&sc,c,sizeof(HostCommand));
	return sc;
}

HostCommand HostCommandFactory::command(HostCommandName name)
{
	HostCommand sc;
	sc.name = name;
	sc.argc = 0;
	validate(&sc);
	return sc;
}

HostCommand HostCommandFactory::command(HostCommandName name,int arg0)
{
	HostCommand sc;
	sc.name = name;
	sc.argc = 1;
	sc.argv[0]=arg0;
	validate(&sc);
	return sc;
}

HostCommand HostCommandFactory::command(HostCommandName name,int arg0,int arg1)
{
	HostCommand sc;
	sc.name = name;
	sc.argc = 2;
	sc.argv[0]=arg0;
	sc.argv[1]=arg1;
	validate(&sc);
	return sc;
}

HostCommand HostCommandFactory::command(HostCommandName name,int arg0,int arg1,int arg2)
{
	HostCommand sc;
	sc.name = name;
	sc.argc = 2;
	sc.argv[0]=arg0;
	sc.argv[1]=arg1;
	sc.argv[2]=arg2;
	validate(&sc);
	return sc;
}


void HostCommandFactory::validate(HostCommand* c)
{
	switch (c->argc)
	{
		case 0:
		if (c->name < HOSTCOMMAND_0_ARGS || c->name>= HOSTCOMMAND_1_ARGS)
			c->name = HC_INVALID;
		break;

		case 1:
		if (c->name < HOSTCOMMAND_1_ARGS || c->name>= HOSTCOMMAND_2_ARGS)
			c->name = HC_INVALID;
		break;

		case 2:
		if (c->name < HOSTCOMMAND_2_ARGS || c->name>= HOSTCOMMAND_3_ARGS)
			c->name = HC_INVALID;
		break;

		default:
		c->name = HC_INVALID;
		break;
	}
}




