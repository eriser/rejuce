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
#include "../Host/HostEvent.h"

enum TransportState
{
	TRANSPORT_STOPPED = 0,
	TRANSPORT_PAUSED,
	TRANSPORT_PLAYING,
	TRANSPORT_RECORDING
};

class Sequencer: private Thread {
public:
	Sequencer();
	virtual ~Sequencer();

	void init(MidiMessageCollector* collector);

	void start();
	void stop();

	bool event(HostEvent c);

private:
	void run();
	int tick();
	void midiEvent(MidiMessage m);
	void executeEvents();
	void executeEvent(HostEvent* c);

private:
	void commandTransport(HostEvent* c);

private:
	MidiMessageCollector* _pMessageCollector;
	float _bpm;
	Song _song;

	TransportState _transportState;

	Array <HostEvent> _commandCollector;
	CriticalSection _commandSection;	// TODO: remove this, use a lockless FIFO command queue
};

Sequencer* getSequencerInstance(MidiMessageCollector* collector);
void freeSequencer();

#endif /* GBSEQ_H*_ */
