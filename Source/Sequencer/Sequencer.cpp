/*
 * gbseq.cpp
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#include "Sequencer.h"

Sequencer* g_sequencer = nullptr;

Sequencer* getSequencerInstance(MidiMessageCollector* collector)
{
	if (!g_sequencer)
	{
		g_sequencer = new Sequencer();
		g_sequencer->init(collector);
		g_sequencer->start();
	}

	return g_sequencer;
}

void freeSequencer()
{
	g_sequencer->stop();
	delete g_sequencer;
	g_sequencer=nullptr;
}


Sequencer::Sequencer() : Thread ("Sequencer")
{
	init (nullptr);
}

Sequencer::~Sequencer()
{
	stop();
}

void Sequencer::init(MidiMessageCollector* collector)
{
	_bpm = 120.0f;
	_pMessageCollector = collector;

	_transportState = TRANSPORT_STOPPED;

	_song.init();
}

void Sequencer::run()
{
	while (!threadShouldExit())
	{
		double start = Time::getMillisecondCounterHiRes();

		// execute all commands
		executeEvents();

		// tick
		tick();

		// wait
		Time::waitForMillisecondCounter(start+PHRASE_DELAY_MSEC(_bpm));
	}

	DBG("sequencer thread exiting");
}

int Sequencer::tick()
{
	int t = 0;

	t = _song.tick(_pMessageCollector);

	return t;
}

void Sequencer::start()
{
	startThread();
}

void Sequencer::stop()
{
	signalThreadShouldExit();
	waitForThreadToExit(-1);
}

void Sequencer::midiEvent(MidiMessage m)
{
	if (_transportState==TRANSPORT_RECORDING)
	{
		_song.addEvent(m);
	}
}

bool Sequencer::event(HostEvent c)
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

void Sequencer::executeEvents()
{
	_commandSection.enter();

	int s = _commandCollector.size();
	for (int i=0;i<s;i++)
	{
		HostEvent* pCommand = &_commandCollector.getReference(i);

		executeEvent(pCommand);
	}
	_commandCollector.clear();

	_commandSection.exit();
}

void Sequencer::executeEvent(HostEvent* c)
{
	switch (c->name)
	{
	case HC_TRANSPORT_PAUSE:
	case HC_TRANSPORT_PLAY:
	case HC_TRANSPORT_RECORD:
	case HC_TRANSPORT_REWIND:
	case HC_TRANSPORT_STOP:
		commandTransport(c);
		break;

	case HC_PATTERN_SET_NEXT:
		_song.setNextPattern(c->argv[0]);
		break;

	case HC_MIDI_EVENT:
		_song.addEvent(HostEventFactory::midiMessageFromEvent(c));
		break;

	default:
		break;
	}
}

void Sequencer::commandTransport(HostEvent* c)
{
	switch (c->name)
	{
	case HC_TRANSPORT_PAUSE:
		_transportState = TRANSPORT_PAUSED;
		_song.pause();
		break;

	case HC_TRANSPORT_PLAY:
		_transportState = TRANSPORT_PLAYING;
		_song.play();
		break;

	case HC_TRANSPORT_RECORD:
		_transportState = TRANSPORT_RECORDING;
		_song.play();
		break;

	case HC_TRANSPORT_REWIND:
		_song.stop();
		_song.play();
		break;

	case HC_TRANSPORT_STOP:
		_song.stop();
		break;

	default:
		break;
	}
}

