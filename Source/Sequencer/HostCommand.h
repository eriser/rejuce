
#ifndef HOSTCOMMAND_H_
#define HOSTCOMMAND_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#define HOSTCOMMAND_INVALID 0
#define HOSTCOMMAND_0_ARGS 1
#define HOSTCOMMAND_1_ARGS 101
#define HOSTCOMMAND_2_ARGS 201
#define HOSTCOMMAND_3_ARGS 201

enum HostCommandName
{
	HC_INVALID = HOSTCOMMAND_INVALID,

	// 0 args, 0 - 100

	HC_TRANSPORT_PLAY =HOSTCOMMAND_0_ARGS,
	HC_TRANSPORT_STOP,
	HC_TRANSPORT_PAUSE,
	HC_TRANSPORT_REWIND,
	HC_TRANSPORT_RECORD,

	// 1 args, 101-200
	HC_MIDI_EVENT = HOSTCOMMAND_1_ARGS,
	HC_PATTERN_SET_NEXT
};

struct HostCommand
{
	HostCommandName name;
	int argc;
	int argv[8];
};

class HostCommandFactory {
public:
	static HostCommand command(HostCommand* c);
	static HostCommand command(HostCommandName name);
	static HostCommand command(HostCommandName name,int arg0);
	static HostCommand command(HostCommandName name,int arg0,int arg1);
	static HostCommand command(HostCommandName name,int arg0,int arg1,int arg2);

private:
	static void validate(HostCommand* c);

};

#endif /* HOSTCOMMAND_H_ */
