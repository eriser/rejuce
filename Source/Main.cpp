/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "Noisemaker/TalCore.h"

#include "Sequencer/Sequencer.h"

//==============================================================================


//#include <Windows.h>
#include <iostream>

class GlobalMidi: public  MidiInputCallback
{
public:
	GlobalMidi() {

	}

	 void handleIncomingMidiMessage (MidiInput* source,const MidiMessage& message)
	 {

	 }
};

//==============================================================================
int main (int argc, char* argv[])
{
//	CoInitialize(NULL);

/*	// setup audio and midi in
	AudioDeviceManager adm;	
	DBG("list audio device types");
	for (int n=0;n<adm.getAvailableDeviceTypes().size();n++)
	{
		AudioIODeviceType* dt = adm.getAvailableDeviceTypes().getUnchecked(n);
		DBG("audio IO device type "<<dt->getTypeName());

		for (int t=0;t<dt->getDeviceNames().size();t++)
		{
			DBG("  device name "<<dt->getDeviceNames()[t]);
		}
	}

	DBG("list midi devices");
	for (int n=0;n<MidiInput::getDevices().size();n++)
	{
		DBG("midi IO device name "<<MidiInput::getDevices()[n]);
	}

	// audio setup (ASIO, M-Audio Delta ASIO)
	adm.setCurrentAudioDeviceType("ALSA",true);
	AudioDeviceManager::AudioDeviceSetup ds;
	ds.outputDeviceName = "Intel 82801AA-ICH";

	String error = adm.setAudioDeviceSetup(ds, true);
	if (error.isNotEmpty())
	{
		DBG(error);
	}
	adm.playTestSound();

	// midi setup
	adm.setMidiInputEnabled("VMPK Output",true);
	if (!adm.isMidiInputEnabled("VMPK Output"))
	{
		DBG("no midi input");
	}

	////////////////////////////////////////////////////////////////

	// create player and processor
	ScopedPointer<TalCore> gs (new TalCore());
	AudioProcessorPlayer app;
	app.setProcessor(gs);

	// hookup to callbacks
	adm.addMidiInputCallback("VMPK Output",&app);
	adm.addAudioCallback(&app);

	AudioDeviceManager::AudioDeviceSetup setup;
	adm.getAudioDeviceSetup(setup);

	GlobalMidi gm;
	adm.addMidiInputCallback("VMPK Output",&gm);
	*/

	MidiMessageCollector* collector = nullptr;//&app.getMidiMessageCollector();
	//collector->reset(setup.sampleRate);
	Sequencer* pSeq = getSequencerInstance(collector);
/*
	// wait
	std::cout <<"q to quit..\n";
	char s[10];s[0]='\0';
	while (s[0]!='q')
	{
		std::cin >> s;

		if (s[0]=='a')//start note
		{
			MidiMessage m(0x90,0x60,0x80,Time::getMillisecondCounterHiRes());
			app.getMidiMessageCollector().addMessageToQueue(m);
		}
		if (s[0]=='b')//end note
		{
			MidiMessage m(0x80,0x60,0x80,Time::getMillisecondCounterHiRes());
			app.getMidiMessageCollector().addMessageToQueue(m);
		}
		if (s[0]=='t') //patch
		{
			int a = atoi(&s[1]);
			gs->setCurrentProgram(jlimit(0,127,a));
		}

		if (s[0]=='p')//play
		{
			printf("play...\n");
			SequencerCommand sc();
		}

		if (s[0]=='s')//play
		{
			printf("stop...\n");
		}

	}

*/
	freeSequencer();
/*
	// cleanup
	app.setProcessor(nullptr);
	adm.removeAudioCallback(&app);
	adm.removeMidiInputCallback("4- PCR MIDI IN",&app);
*/
//	CoUninitialize();
    return 0;
}
