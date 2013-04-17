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
	_bpm = 120.0f;
	_pMessageCollector = nullptr;
}

Sequencer::~Sequencer()
{
	stop();
}

void Sequencer::init(MidiMessageCollector* collector)
{
	_pMessageCollector = collector;
	_song.init(collector);
}

void Sequencer::run()
{
	while (!threadShouldExit())
	{
		double start = Time::getMillisecondCounterHiRes();

		// execute all commands
		executeCommands();

		// tick
		tick();

		// wait
		Time::waitForMillisecondCounter(start+PHRASE_DELAY_MSEC(_bpm));
	}
}

int Sequencer::tick()
{


	return 0;
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

bool Sequencer::command(SequencerCommand c)
{
	if (c.name != SC_INVALID)
	{
		_commandSection.enter();
		_commandCollector.add(c);
		_commandSection.exit();

		return true;
	}
	return false;
}

void Sequencer::executeCommands()
{
	_commandSection.enter();

	int s = _commandCollector.size();
	for (int i=0;i<s;i++)
	{
		SequencerCommand* pCommand = &_commandCollector.getReference(i);

		executeCommand(pCommand);
	}
	_commandCollector.clear();

	_commandSection.exit();
}

void Sequencer::executeCommand(SequencerCommand* c)
{
	//TODO
}



