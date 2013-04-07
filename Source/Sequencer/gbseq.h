/*
 * gbseq.h
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#ifndef GBSEQ_H_
#define GBSEQ_H_

#include "../../JuceLibraryCode/JuceHeader.h"

class gbseq {
public:
	gbseq(MidiMessageCollector* collector);
	virtual ~gbseq();

	void Tick();

private:
	MidiMessageCollector* _pMessageCollector;
	float _bpm;


};

#endif /* GBSEQ_H*_ */
