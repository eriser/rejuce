/*
 * gbseq.h
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#ifndef GBSEQ_H_
#define GBSEQ_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#include "Phrase.h"
#include "Pattern.h"
#include "Song.h"
#include "SequencerCommand.h"

class Sequencer: private Thread {
public:
	Sequencer();
	virtual ~Sequencer();

	void init(MidiMessageCollector* collector);

	void start();
	void stop();

	bool command(SequencerCommand& c);

private:
	void run();
	int tick();
	void executeCommands();
	void executeCommand(SequencerCommand* c);

private:
	MidiMessageCollector* _pMessageCollector;
	float _bpm;
	Song _song;

	Array <SequencerCommand> _commandCollector;
	CriticalSection _commandSection;	// TODO: remove this, use a lockless FIFO command queue
};

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


#endif /* GBSEQ_H*_ */
