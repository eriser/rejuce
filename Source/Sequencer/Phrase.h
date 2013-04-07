/*
 * phrase.h
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#ifndef PHRASE_H_
#define PHRASE_H_

#include "../../JuceLibraryCode/JuceHeader.h"

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
	void Play(double time);
	void Stop();
	void Pause();

	void AddEvent(MidiMessage m); // new events merged on next Play or Stop

	void Tick(double time);

	void Debug();

private:
	void MergeScratchBuffer();

private:
	int _channel;
	PhraseState _state;

	MidiBuffer _seq;
	MidiBuffer _scratch;
	MidiBuffer::Iterator* _pseqIter;
	MidiMessage _curMessage;
	int _curMessagePos;
	bool _haveEvent;

	MidiMessageCollector* _pCollector;
	int _lengthBars;
    int _timeSigNumerator;
    int _timeSigDenominator;

    double _origin;	// last time loop started playing
};

#endif /* PHRASE_H_ */
