
#ifndef GROOVECOMMAND_H_
#define GROOVECOMMAND_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#define GROOVEEVENT_INVALID 0

enum GrooveEventName
{
	GC_INVALID = GROOVEEVENT_INVALID,


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
