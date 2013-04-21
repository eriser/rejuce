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
	_synthArray.add(gs);

	// add to graph
	for (int i=0;i<_synthArray.size();i++)
	{
		_graph.addNode(_synthArray[i],i);
	}
}

Host::~Host()
{
	_graph.clear();


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

	_adm.addMidiInputCallback(midiInterface,this);
	_midiInterfaceName = midiInterface;

	return true;
}

void Host::start()
{
	_app.setProcessor(&_graph);
	_adm.addAudioCallback(&_app);
}

void Host::stop()
{
	_app.setProcessor(nullptr);
	_adm.removeAudioCallback(&_app);
	_adm.removeMidiInputCallback(_midiInterfaceName,this);
}

bool Host::command(HostCommand c)
{
	if (c.name != HC_INVALID)
	{
		_commandSection.enter();
		_commandCollector.add(c);
		_commandSection.exit();

		return true;
	}
	return false;
}

void Host::handleIncomingMidiMessage (MidiInput* source,const MidiMessage& message)
{
	DBG("have midi");
}
