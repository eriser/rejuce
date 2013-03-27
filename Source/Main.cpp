/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "GenericSynth.h"

//==============================================================================


#include <Windows.h>
#include <iostream>

//==============================================================================
int main (int argc, char* argv[])
{
	CoInitialize(NULL);

	// setup audio and midi in
	AudioDeviceManager adm;	
	for (int n=0;n<adm.getAvailableDeviceTypes().size();n++)
	{
		AudioIODeviceType* dt = adm.getAvailableDeviceTypes().getUnchecked(n);
		DBG("audio IO device type "<<dt->getTypeName());

		for (int t=0;t<dt->getDeviceNames().size();t++)
		{
			DBG("  device name "<<dt->getDeviceNames()[t]);
		}
	}

	for (int n=0;n<MidiInput::getDevices().size();n++)
	{
		DBG("midi IO device name "<<MidiInput::getDevices()[n]);
	}

	// audio setup (ASIO, M-Audio Delta ASIO)
	adm.setCurrentAudioDeviceType("ASIO",true);
	AudioDeviceManager::AudioDeviceSetup ds;
	ds.outputDeviceName = "M-Audio Delta ASIO";

	String error = adm.setAudioDeviceSetup(ds, true);
	if (error.isNotEmpty())
	{
		DBG(error);
	}
	adm.playTestSound();

	// midi setup
	adm.setMidiInputEnabled("4- PCR MIDI IN",true);
	if (!adm.isMidiInputEnabled("4- PCR MIDI IN"))
	{
		DBG("no midi input");
	}

	////////////////////////////////////////////////////////////////

	// create player and processor
	ScopedPointer<AudioProcessor> gs = (AudioProcessor*)new GenericSynth();
	AudioProcessorPlayer app;
	app.setProcessor(gs);

	// hookup to callbacks
	adm.addMidiInputCallback("4- PCR MIDI IN",&app);
	adm.addAudioCallback(&app);

	// wait
	std::cout <<"q to quit..\n";
	char s[10];s[0]='\0';
	while (s[0]!='q')
	{
		std::cin >> s;

		if (s[0]=='a')
		{
			MidiMessage m(0x80,0x60,0x80,Time::getMillisecondCounterHiRes());
			app.getMidiMessageCollector().addMessageToQueue(m);
		}
		if (s[0]=='b')
		{
			MidiMessage m(0x90,0x60,0x80,Time::getMillisecondCounterHiRes());
			app.getMidiMessageCollector().addMessageToQueue(m);
		}
	}

	// cleanup
	app.setProcessor(nullptr);
	adm.removeAudioCallback(&app);
	adm.removeMidiInputCallback("4- PCR MIDI IN",&app);

	CoUninitialize();
    return 0;
}
