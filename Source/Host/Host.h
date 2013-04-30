/*
 * Host.h
 *
 *  Created on: 21 Apr 2013
 *      Author: gboxdev
 */

#ifndef HOST_H_
#define HOST_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#include "HostEvent.h"
#include "HostProcessor.h"
#include "../Sequencer/Sequencer.h"

class Host: public  MidiInputCallback {
public:
	Host();
	virtual ~Host();

	void listInterfaces();

	bool init(String audioDeviceType,String audioInterface,String midiInterface,int sampleRate);

	bool event(HostEvent c);

private:
	void handleIncomingMidiMessage (MidiInput* source,const MidiMessage& message);

private:
	AudioDeviceManager _adm;
	HostProcessor* _hostProcessor;
	AudioProcessorPlayer _app;
	String _midiInterfaceName;

	Sequencer _sequencer;
};

#endif /* HOST_H_ */
