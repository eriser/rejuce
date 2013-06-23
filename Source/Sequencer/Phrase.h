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

class Section;

enum PhraseState
{
	PHRASE_STOPPED =0,
	PHRASE_PAUSED,
	PHRASE_PLAYING
};

class Phrase
{
	friend class Section;

public:
	Phrase();
	virtual ~Phrase();
	void init(int channel);

	void setLengthBars(int bars);
	int getLengthBars();
	int getLengthClocks();

	void setTimeSignature(int numerator,int denominator);

	void clear();
	void debug();

	void Quantise(int divs);

protected:
	void play();
	void stop();
	void pause();

	void addEvent(MidiMessage m); // new events merged on next Play or Stop

	// has to be called on time
	int tick(MidiMessageCollector* pCollector);

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

	int _lengthBars;
    int _timeSigNumerator;
    int _timeSigDenominator;

    volatile int _clock;
    int _lengthClocks;

};

#endif /* PHRASE_H_ */
