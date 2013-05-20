
#ifndef GROOVECOMMAND_H_
#define GROOVECOMMAND_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#define GROOVEEVENT_INVALID 0

enum GrooveEventName
{
	GC_INVALID = GROOVEEVENT_INVALID,

	GC_BUTTON_DOWN = 1,
	GC_BUTTON_UP = 2,

	GC_KNOB = 3,

	GC_LED = 4,					// 2 params, [which led] [on/off]

	GC_OUT_LEDPOS = 100,		// one param, -1 for all off, 0-16 normally
};

enum GrooveLedName
{
	GT_LED_PLAYING =0,
	GT_LED_RECORDING,

	GT_LED_POS_0 = 100,
	GT_LED_POS_1 = 101,
	GT_LED_POS_2 = 102,
	GT_LED_POS_3 = 103,
	GT_LED_POS_4 = 104,
	GT_LED_POS_5 = 105,
	GT_LED_POS_6 = 106,
	GT_LED_POS_7 = 107,
	GT_LED_POS_8 = 108,
	GT_LED_POS_9 = 109,
	GT_LED_POS_10 = 110,
	GT_LED_POS_11 = 111,
	GT_LED_POS_12 = 112,
	GT_LED_POS_13 = 113,
	GT_LED_POS_14 = 114,
	GT_LED_POS_15 = 115,
};

enum GrooveControlName
{
	GT_STOP =0,
	GT_PLAY,
	GT_PAUSE,
	GT_REWIND,
	GT_RECORD,
};

struct GrooveEvent
{
	GrooveEventName name;
	int argc;
	int argv[8];
};

class GrooveEventFactory {
public:
	static GrooveEvent event(GrooveEvent* c);
	static GrooveEvent event(GrooveEventName name);
	static GrooveEvent event(GrooveEventName name,int argc,int* args);
	static GrooveEvent event(GrooveEventName name,int arg0);
	static GrooveEvent event(GrooveEventName name,int arg0,int arg1);
	static GrooveEvent event(GrooveEventName name,int arg0,int arg1,int arg2);

	static GrooveEvent event(const MidiMessage& message);

private:
	static void validate(GrooveEvent* c);

};

#endif /* HOSTCOMMAND_H_ */
