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

void Host::setHostEventListener(HostEventListener* pHostEventListener)
{
	_hostEventListener = pHostEventListener;
}

void Host::setOutListener(GrooveEventListener* pOutListener)
{
	_grooveEventListener = pOutListener;
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

String Host::getFirstOutputDeviceName(String audioDeviceType)
{
	for (int n=0;n<_adm.getAvailableDeviceTypes().size();n++)
	{
		AudioIODeviceType* dt = _adm.getAvailableDeviceTypes().getUnchecked(n);

		if (dt->getTypeName()==audioDeviceType)
		{
			for (int t=0;t<dt->getDeviceNames().size();t++)
			{
				return dt->getDeviceNames()[t];
			}
		}
	}

	return String::empty;
}

// empty string for audio interface uses first one found in specified type
bool Host::init(String audioDeviceType,String audioInterface,String midiInterface,int sampleRate)
{
	listInterfaces();

	_adm.setCurrentAudioDeviceType(audioDeviceType,true);
	AudioDeviceManager::AudioDeviceSetup ds;

	if (audioInterface.isEmpty())
	{
		ds.outputDeviceName = getFirstOutputDeviceName(audioDeviceType);
		DBG("using default "+ds.outputDeviceName);
	}
	ds.outputDeviceName = audioInterface;

	String error = _adm.setAudioDeviceSetup(ds, true);
	if (error.isNotEmpty())
	{
		DBG(error);
		return false;
	}

	_adm.playTestSound();

	// midi setup
	if (!midiInterface.isEmpty())
	{
		_adm.setMidiInputEnabled(midiInterface,true);
		if (!_adm.isMidiInputEnabled(midiInterface))
		{
			DBG("no midi input");
		}
		else
		{
			_midiInterfaceName = midiInterface;
			_adm.addMidiInputCallback(_midiInterfaceName,this);
		}
	}
	else
	{
		DBG("midi input disabled");
	}

	// start things up
	_app.setProcessor(_hostProcessor);
	_app.getMidiMessageCollector().reset(sampleRate);
	_adm.addAudioCallback(&_app);

	// sequencer initialisation
	_sequencer.init(&_app.getMidiMessageCollector(),_hostEventListener);
	_sequencer.start();

	return true;
}

bool Host::event(HostEvent c)
{
	bool ret = false;
	bool bConsumed = false;

	// if this is a midi message we want to pass it straight to the
	// correct synth.
	if (!bConsumed && c.name == HC_MIDI_EVENT)
	{
		// convert to midi message and check channel
		MidiMessage message = HostEventFactory::midiMessageFromEvent(&c);

		// add the message to the graph player queue
		_app.getMidiMessageCollector().addMessageToQueue(message);

		// DO NOT consume message, as it must go to the sequencer as well
	}

	if (!bConsumed && c.name == HC_OUT_BEAT)
	{
		GrooveEvent g = GrooveEventFactory::event(GE_LEDSET,GCL_PLAYING,2);
		_grooveEventListener->onGrooveEvent(g);
	}

	{
		// TODO: some events will be system events that we need to handle here, immediately.
		// load and save kinda stuff probably.

		// TODO: set bHandled to true if we handled it
	}

	// if its a transport stop, always call panic to kill any stray notes
	if (!bConsumed && c.name == HC_TRANSPORT_STOP)
	{
		panic();
		bConsumed = true;
	}

	// give the event to the sequencer if its not been handled already
	if (!bConsumed && c.name != HC_INVALID)
	{
		ret = _sequencer.event(c);
	}

	return ret;
}

void Host::handleIncomingMidiMessage (MidiInput* source,const MidiMessage& message)
{
	printf("c timecode %f\n",message.getTimeStamp());

	// fire it
	event(HostEventFactory::event(message));
}

void Host::panic()
{
	for (int c=1;c<=16;c++)
	{
		MidiMessage m = MidiMessage::allNotesOff(c);
		m.setTimeStamp(Time::getMillisecondCounterHiRes()/1000.0f);
		event (HostEventFactory::event(m));
	}
}

