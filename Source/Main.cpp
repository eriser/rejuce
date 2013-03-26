/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

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

	// midi setup
	adm.setMidiInputEnabled("4- PCR MIDI IN",true);

	adm.setAudioDeviceSetup(ds, true);
	adm.playTestSound();


	////////////////////////////////////////////////////////////////

	AudioProcessorPlayer app;




	// wait
	std::cout <<"enter..\n";
	char s[10];
	std::cin >> s;

	CoUninitialize();
    return 0;
}
