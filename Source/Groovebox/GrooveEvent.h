
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

typedef enum
{
	GE_LED_OFF=0,
	GE_LED_ON,
	GE_LED_PULSE,	// once only (speed determined by interface not BPM or anything clever)
	GE_LED_FLASH,	// flash continuously (as above)

	GE_LED_SIZE
} GrooveLedArgValues;


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
   X(GC_BUTTON_OCTDOWN,		"octDown") \
   X(GC_BUTTON_OCTUP,		"octUp") \
   X(GC_BUTTON_TRACK,		"track") \
   X(GC_BUTTON_TRANSPOSE,	"transpose") \
   X(GC_BUTTON_MUTE,		"mute") \
   X(GC_BUTTON_SECTION,		"section") \
   \
   X(GC_BUTTON_WHITE0,	"white0") \
   X(GC_BUTTON_WHITE1,	"white1") \
		   X(GC_BUTTON_BLACK0,	"black0") \
   X(GC_BUTTON_WHITE2,	"white2") \
   	   	   X(GC_BUTTON_BLACK1,	"black1") \
   X(GC_BUTTON_WHITE3,	"white3") \
   	   	   X(GC_BUTTON_BLACK2,	"black2") \
   X(GC_BUTTON_WHITE4,	"white4") \
   X(GC_BUTTON_WHITE5,	"white5") \
   	   	   X(GC_BUTTON_BLACK3,	"black3") \
   X(GC_BUTTON_WHITE6,	"white6") \
   	   	   X(GC_BUTTON_BLACK4,	"black4") \
   X(GC_BUTTON_WHITE7,	"white7") \
   X(GC_BUTTON_WHITE8,	"white8") \
  	   	   X(GC_BUTTON_BLACK5,	"black5") \
   X(GC_BUTTON_WHITE9,	"white9") \
   	   	   X(GC_BUTTON_BLACK6,	"black6") \
   X(GC_BUTTON_WHITE10,	"white10") \
   	   	   X(GC_BUTTON_BLACK7,	"black7") \
   X(GC_BUTTON_WHITE11,	"white11") \
   X(GC_BUTTON_WHITE12,	"white12") \
   	   	   X(GC_BUTTON_BLACK8,	"black8") \
   X(GC_BUTTON_WHITE13,	"white13") \
   	   	   X(GC_BUTTON_BLACK9,	"black9") \
   X(GC_BUTTON_WHITE14,	"white14") \
   X(GC_BUTTON_WHITE15,	"white15") \
   \
   X(GCL_PLAYING, 		"playing") \
   X(GCL_RECORDING, 	"recording") \
   \
   X(GCL_MUTE, 			"muteLed") \
   X(GCL_SECTION, 		"sectionLed") \
   \
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
   \
   X(GCK_FILTER_TYPE,		"knobFilterType") \
   X(GCK_FILTER_CUTOFF, 	"knobFilterCutoff") \
   X(GCK_FILTER_RES, 		"knobFilterRes") \
   X(GCK_FILTER_KEYFOLLOW,	"knobFilterKeyFollow") \
   X(GCK_FILTER_CONTOUR,	"knobFilterContour") \
   X(GCK_FILTER_A,			"knobFilterA") \
   X(GCK_FILTER_D,			"knobFilterD") \
   X(GCK_FILTER_S,			"knobFilterS") \
   X(GCK_FILTER_R,			"knobFilterR") \
   \
   X(GC_SIZE,		 	"size")

typedef enum {
	#define X(Enum, String)       Enum,
	X_GROOVECONTROLS
	#undef X
} GrooveControlName;


const char* GrooveEvent_getNameString(GrooveEventName name);
const char* GrooveControl_getNameString(GrooveControlName name);

class GrooveEvent
{
public:
	GrooveEvent();
	GrooveEvent(GrooveEventName event,GrooveControlName control,int value);
	GrooveEvent(GrooveEventName event,GrooveControlName control,char* value);
	GrooveEvent(GrooveEventName event,GrooveControlName control,String &value);
	~GrooveEvent();

	GrooveEventName getEvent();
	GrooveControlName getControl();

	bool isString();
	bool isInt();

	int getAsInt();
	char* getAsString();

private:
	GrooveEventName _event;
	GrooveControlName _control;
	bool _isString;
	char _value[32];
};

#endif /* HOSTCOMMAND_H_ */
