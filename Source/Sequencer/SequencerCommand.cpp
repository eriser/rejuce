/*
 * SequencerCommand.cpp
 *
 *  Created on: 14 Apr 2013
 *      Author: gboxdev
 */

#include "SequencerCommand.h"

SequencerCommand SequencerCommandFactory::command(SequencerCommand* c)
{
	SequencerCommand sc;
	memcpy(&sc,c,sizeof(SequencerCommand));
	return sc;
}

SequencerCommand SequencerCommandFactory::command(SequencerCommandName name)
{
	SequencerCommand sc;
	sc.name = name;
	sc.argc = 0;
	validate(&sc);
	return sc;
}

SequencerCommand SequencerCommandFactory::command(SequencerCommandName name,int arg0)
{
	SequencerCommand sc;
	sc.name = name;
	sc.argc = 1;
	sc.argv[0]=arg0;
	validate(&sc);
	return sc;
}

SequencerCommand SequencerCommandFactory::command(SequencerCommandName name,int arg0,int arg1)
{
	SequencerCommand sc;
	sc.name = name;
	sc.argc = 2;
	sc.argv[0]=arg0;
	sc.argv[1]=arg1;
	validate(&sc);
	return sc;
}

SequencerCommand SequencerCommandFactory::command(SequencerCommandName name,int arg0,int arg1,int arg2)
{
	SequencerCommand sc;
	sc.name = name;
	sc.argc = 2;
	sc.argv[0]=arg0;
	sc.argv[1]=arg1;
	sc.argv[2]=arg2;
	validate(&sc);
	return sc;
}


void SequencerCommandFactory::validate(SequencerCommand* c)
{
	switch (c->argc)
	{
		case 0:
		if (c->name < SEQUENCERCOMMAND_0_ARGS || c->name>= SEQUENCERCOMMAND_1_ARGS)
			c->name = SC_INVALID;
		break;

		case 1:
		if (c->name < SEQUENCERCOMMAND_1_ARGS || c->name>= SEQUENCERCOMMAND_2_ARGS)
			c->name = SC_INVALID;
		break;

		case 2:
		if (c->name < SEQUENCERCOMMAND_2_ARGS || c->name>= SEQUENCERCOMMAND_3_ARGS)
			c->name = SC_INVALID;
		break;

		default:
		c->name = SC_INVALID;
		break;
	}
}




