/*
 * phrase.h
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#ifndef PHRASE_H_
#define PHRASE_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#define PHRASE_CLOCKS 24	/* resolution in beat */
#define PHRASE_DELAY_MSEC(bpm) (1000 / ((bpm*PHRASE_CLOCKS)/60) )

enum PhraseState
{
	PHRASE_STOPPED =0,
	PHRASE_PAUSED,
	PHRASE_PLAYING,
	PHRASE_RECORDING

};

class Phrase {
public:
	Phrase(MidiMessageCollector* pCollector,int channel);
	virtual ~Phrase();

	// operations
	void play();
	void stop();
	void pause();

	void setLengthBars(int bars);
	void setTimeSignature(int numerator,int denominator);

	void addEvent(MidiMessage m); // new events merged on next Play or Stop
	void clear();

	// has to be called on time
	int tick();

	void debug();

private:
	void MergeScratchBuffer();

private:
	int _channel;
	PhraseState _state;

	MidiBuffer _seq;
	MidiBuffer _scratch;
	MidiBuffer::Iterator *_pseqIter;

	MidiMessage _curMessage;
	int _curMessageClock;
	bool _haveEvent;

	MidiMessageCollector* _pCollector;
	int _lengthBars;
    int _timeSigNumerator;
    int _timeSigDenominator;

    volatile int _clock;
    int _lengthClocks;

};

#endif /* PHRASE_H_ */
