/*
 * gbseq.cpp
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#include "Sequencer.h"

Sequencer::Sequencer()
{

}

Sequencer::~Sequencer()
{
}

void Sequencer::init(MidiMessageCollector* collector)
{
	_pMessageCollector = collector;
}

void Sequencer::run()
{
	while (!threadShouldExit())
	{
		// wait then command then tick
	}
}

int Sequencer::tick()
{


	return 0;
}

void Sequencer::start()
{

}

void Sequencer::stop()
{

}

bool Sequencer::command(SequencerCommand c)
{

}



