/*
 * SequencerCommand.cpp
 *
 *  Created on: 14 Apr 2013
 *      Author: gboxdev
 */

#include "SequencerCommand.h"

SequencerCommand::SequencerCommand(char* raw)
{
	_name =  (SequencerCommandName) (*((int*)raw));
	int argc = * ((int*)raw+sizeof(int));
	for (int i=0;i<argc;i++)
	{
		int* arg = (int*)(raw+sizeof(int)+sizeof(int)*(i+1));
		_args.add(*arg);
	}
	validate();
}

const char* SequencerCommand::getRaw()
{
	char* raw = _raw;
	memset(raw,0,16);

	memcpy(raw,&_name,sizeof(int));
	raw+=sizeof(int);

	int argc = _args.size();
	memcpy(raw,&argc,sizeof(int));
	raw+=sizeof(int);

	for (int i=0;i<argc;i++)
	{
		int argv= _args[i];
		memcpy(raw,&argv,sizeof(int));
		raw+=sizeof(int);
	}

	return _raw;
}

SequencerCommand::SequencerCommand(SequencerCommandName name,int arg0)
{
	_name = name;
	_args.add(arg0);
	validate();
}

SequencerCommand::SequencerCommand(SequencerCommandName name,int arg0,int arg1)
{
	_name = name;
	_args.add(arg0);
	_args.add(arg1);
	validate();
}

SequencerCommand::SequencerCommand(SequencerCommandName name,int arg0,int arg1,int arg2)
{
	_name = name;
	_args.add(arg0);
	_args.add(arg1);
	_args.add(arg2);
	validate();
}

SequencerCommand::~SequencerCommand() {
}

SequencerCommandName SequencerCommand::getName()
{
	return _name;
}

int SequencerCommand::getArg(int i)
{
	return _args[i];
}

int SequencerCommand::getArgCount()
{
	return _args.size();
}

void SequencerCommand::validate()
{
	switch (getArgCount())
	{
		case 0:
		if (_name < SEQUENCERCOMMAND_0_ARGS || _name>= SEQUENCERCOMMAND_1_ARGS)
			_name = SC_INVALID;
		break;

		case 1:
		if (_name < SEQUENCERCOMMAND_1_ARGS || _name>= SEQUENCERCOMMAND_2_ARGS)
			_name = SC_INVALID;
		break;

		case 2:
		if (_name < SEQUENCERCOMMAND_2_ARGS || _name>= SEQUENCERCOMMAND_3_ARGS)
			_name = SC_INVALID;
		break;

		default:
		_name = SC_INVALID;
		break;
	}
}




