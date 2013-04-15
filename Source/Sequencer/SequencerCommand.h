/*
 * SequencerCommand.h
 *
 *  Created on: 14 Apr 2013
 *      Author: gboxdev
 */

#ifndef SEQUENCERCOMMAND_H_
#define SEQUENCERCOMMAND_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#define SEQUENCERCOMMAND_INVALID 0
#define SEQUENCERCOMMAND_0_ARGS 1
#define SEQUENCERCOMMAND_1_ARGS 101
#define SEQUENCERCOMMAND_2_ARGS 201
#define SEQUENCERCOMMAND_3_ARGS 201

enum SequencerCommandName
{
	SC_INVALID = SEQUENCERCOMMAND_INVALID,

	// 0 args, 0 - 100

	SC_TRANSPORT_PLAY =SEQUENCERCOMMAND_0_ARGS,
	SC_TRANSPORT_STOP,
	SC_TRANSPORT_PAUSE,
	SC_TRANSPORT_REWIND,
	SC_TRANSPORT_RECORD,

	// 1 args, 101-200
	SC_PATTERN_SET_NEXT = SEQUENCERCOMMAND_1_ARGS


};

class SequencerCommand {
public:
	SequencerCommand(SequencerCommand &c);
	SequencerCommand(char* raw);
	SequencerCommand(SequencerCommandName name,int arg0);
	SequencerCommand(SequencerCommandName name,int arg0,int arg1);
	SequencerCommand(SequencerCommandName name,int arg0,int arg1,int arg2);
	virtual ~SequencerCommand();

	void init(SequencerCommand &c);

	SequencerCommandName getName();
	int getArg(int i);
	int getArgCount();

	const char* getRaw();

private:
	void validate();

private:
	SequencerCommandName _name;
	Array <int> _args;
	char _raw[16];
};

#endif /* SEQUENCERCOMMAND_H_ */
