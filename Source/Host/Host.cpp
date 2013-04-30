/*
 * Host.cpp
 *
 *  Created on: 21 Apr 2013
 *      Author: gboxdev
 */

#include "Host.h"

#include "../Noisemaker/TalCore.h"

Host::Host()
{
	// TODO: at the moment there is only one synth!!!
	AudioProcessor* gs = new TalCore();

	_hostProcessor = new HostProcessor();

	_hostProcessor->hostAddSynth(gs);

}

Host::~Host()
{
	_sequencer.stop();

	_app.setProcessor(nullptr);
	delete _hostProcessor;
	_adm.removeAudioCallback(&_app);
	_adm.removeMidiInputCallback(_midiInterfaceName,this);
}

void Host::listInterfaces()
{
	DBG("list audio device types");
	for (int n=0;n<_adm.getAvailableDeviceTypes().size();n++)
	{
		AudioIODeviceType* dt = _adm.getAvailableDeviceTypes().getUnchecked(n);
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
}

bool Host::init(String audioDeviceType,String audioInterface,String midiInterface,int sampleRate)
{
	listInterfaces();

	_adm.setCurrentAudioDeviceType(audioDeviceType,true);
	AudioDeviceManager::AudioDeviceSetup ds;
	ds.outputDeviceName = audioInterface;

	String error = _adm.setAudioDeviceSetup(ds, true);
	if (error.isNotEmpty())
	{
		DBG(error);
		return false;
	}

	_adm.playTestSound();

	// midi setup
	_adm.setMidiInputEnabled(midiInterface,true);
	if (!_adm.isMidiInputEnabled(midiInterface))
	{
		DBG("no midi input");
		return false;
	}

	_midiInterfaceName = midiInterface;

	// start things up
	_adm.addMidiInputCallback(_midiInterfaceName,this);
	_app.setProcessor(_hostProcessor);
	_adm.addAudioCallback(&_app);

	// sequencer initialisation
	_sequencer.init(&_app.getMidiMessageCollector());
	_sequencer.start();

	return true;
}

bool Host::event(HostEvent c)
{
	bool ret = false;
	bool bHandled = false;

	// if this is a midi message we want to pass it straight to the
	// correct synth.
	if (c.name == HC_MIDI_EVENT)
	{
		// convert to midi message and check channel
		MidiMessage message = HostEventFactory::midiMessageFromEvent(&c);

		// add the message to the graph player queue
		_app.getMidiMessageCollector().addMessageToQueue(message);
	}
	else
	{
		// TODO: some events will be system events that we need to handle here, immediately.
		// load and save kinda stuff probably.

		// TODO: set bHandled to true if we handled it
	}

	// give the event to the sequencer if its not been handled already
	if (c.name != HC_INVALID && !bHandled)
	{
		ret = _sequencer.event(c);
	}

	return ret;
}

void Host::handleIncomingMidiMessage (MidiInput* source,const MidiMessage& message)
{
	DBG("have midi");

	// fire it
	event(HostEventFactory::event(message));
}

