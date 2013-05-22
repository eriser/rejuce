
#ifndef GROOVECOMMAND_H_
#define GROOVECOMMAND_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#define GROOVEEVENT_INVALID 0


 #define X_GROOVEEVENTS \
    X(GE_INVALID, 		"invalid") \
    X(GE_BUTTON_DOWN,   "buttonDown") \
    X(GE_BUTTON_UP, 	"buttonUp") \
    X(GE_KNOB, 			"knob") \
    X(GE_LEDSET,		"led") \
	X(GE_SIZE,		 	"size")

typedef enum {
	#define X(Enum, String)       Enum,
		X_GROOVEEVENTS
	#undef X
} GrooveEventName;


#define X_GROOVECONTROLS \
   X(GC_INVALID, 		"invalid") \
   \
   X(GC_BUTTON_PLAY,	"play") \
   X(GC_BUTTON_STOP,	"stop") \
   X(GC_BUTTON_PAUSE,	"pause") \
   X(GC_BUTTON_BACK,	"back") \
   X(GC_BUTTON_RECORD,	"record") \
   \
   X(GCL_PLAYING, 		"playing") \
   X(GCL_RECORDING, 	"recording") \
   X(GCL_SEMI_0, 		"semi0") \
   X(GCL_SEMI_1, 		"semi1") \
   X(GCL_SEMI_2, 		"semi2") \
   X(GCL_SEMI_3, 		"semi3") \
   X(GCL_SEMI_4, 		"semi4") \
   X(GCL_SEMI_5, 		"semi5") \
   X(GCL_SEMI_6, 		"semi6") \
   X(GCL_SEMI_7, 		"semi7") \
   X(GCL_SEMI_8, 		"semi8") \
   X(GCL_SEMI_9, 		"semi9") \
   X(GCL_SEMI_10, 		"semi10") \
   X(GCL_SEMI_11, 		"semi11") \
   X(GCL_SEMI_12, 		"semi12") \
   X(GCL_SEMI_13, 		"semi13") \
   X(GCL_SEMI_14, 		"semi14") \
   X(GCL_SEMI_15, 		"semi15") \
   \
   X(GC_SIZE,		 	"size")

typedef enum {
	#define X(Enum, String)       Enum,
	X_GROOVECONTROLS
	#undef X
} GrooveControlName;


const char* GrooveEvent_getNameString(GrooveEventName name);
const char* GrooveControl_getNameString(GrooveControlName name);

struct GrooveEvent
{
	GrooveEventName event;
	GrooveControlName control;
	int argv;
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
