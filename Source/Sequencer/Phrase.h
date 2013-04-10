/*
 * phrase.h
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#ifndef PHRASE_H_
#define PHRASE_H_

#include "../../JuceLibraryCode/JuceHeader.h"

#include "MidiSequence.h"

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
	void Play();
	void Stop();
	void Pause();

	void setLengthBars(int bars);
	void setTimeSignature(int numerator,int denominator);

	void AddEvent(MidiMessage m); // new events merged on next Play or Stop

	// has to be called on time
	int Tick();

	void Debug();

private:
	void MergeScratchBuffer();

private:
	int _channel;
	PhraseState _state;

	MidiSequence _seq;
	MidiSequence _scratch;

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
