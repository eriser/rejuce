
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

	HC_PHASE_QUANTISE,		// <-- TODO!

	HC_OUT_BEAT,											//	fired once per beat

	// 1 args, 101-200
	HC_SECTION_SET_NEXT	= HOSTEVENT_1_ARGS,					//	[int next pattern]
	HC_OUT_SECTION_CHANGE,
	HC_PHRASE_MUTE_TOGGLE,

	HC_REC_METSTATE,
	HC_REC_METRONOME,
	HC_PHRASE_LENGTH,
	HC_SONG_QUANTISENUMERATOR,								// [int numerator, default 4]
	HC_SONG_QUANTISEDIVISOR,								// [int divisor, default 8]
	HC_SONG_AUTOQUANTISE,									// [int 0 = false 1 = true]

	HC_CURRENT_PHRASE,										// [int phrase]

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
	static HostEvent event(HostEvent* c);
	static HostEvent event(HostEventName name);
	static HostEvent event(HostEventName name,int argc,int* args);
	static HostEvent event(HostEventName name,int arg0);
	static HostEvent event(HostEventName name,int arg0,int arg1);
	static HostEvent event(HostEventName name,int arg0,int arg1,int arg2);

	static HostEvent event(const MidiMessage& message);
	static MidiMessage midiMessageFromEvent(HostEvent* c);

private:
	static void validate(HostEvent* c);

};

#endif /* HOSTCOMMAND_H_ */
