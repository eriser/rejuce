
#ifndef HOSTCOMMAND_H_
#define HOSTCOMMAND_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#define HOSTEVENT_INVALID 0
#define HOSTEVENT_0_ARGS 1
#define HOSTEVENT_1_ARGS 101
#define HOSTEVENT_2_ARGS 201
#define HOSTEVENT_3_ARGS 301

enum HostEventName
{
	HC_INVALID = HOSTEVENT_INVALID,

	// 0 args, 0 - 100

	HC_TRANSPORT_PLAY =HOSTEVENT_0_ARGS,
	HC_TRANSPORT_STOP,
	HC_TRANSPORT_PAUSE,
	HC_TRANSPORT_REWIND,
	HC_TRANSPORT_RECORD,

	// 1 args, 101-200
	HC_PATTERN_SET_NEXT	= HOSTEVENT_1_ARGS,					//	[int next pattern]

	// 2 args, 201-300
	HC_MIDI_EVENT = HOSTEVENT_2_ARGS,		//	[float timestamp] [int rawmidi]
};

struct HostEvent
{
	HostEventName name;
	int argc;
	int argv[8];
};

class HostEventFactory {
public:
	static HostEvent command(HostEvent* c);
	static HostEvent command(HostEventName name);
	static HostEvent command(HostEventName name,int arg0);
	static HostEvent command(HostEventName name,int arg0,int arg1);
	static HostEvent command(HostEventName name,int arg0,int arg1,int arg2);

private:
	static void validate(HostEvent* c);

};

#endif /* HOSTCOMMAND_H_ */
