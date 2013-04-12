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

class Sequencer {
public:
	Sequencer(MidiMessageCollector* collector);
	virtual ~Sequencer();

	int tick();

private:
	MidiMessageCollector* _pMessageCollector;
	float _bpm;




};

#endif /* GBSEQ_H*_ */
