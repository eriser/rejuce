/*
 * Pattern.h
 *
 *  Created on: 12 Apr 2013
 *      Author: gnuvebox
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include "Phrase.h"
class Phrase;

enum PatternState
{
	PATTERN_STOPPED =0,
	PATTERN_PAUSED,
	PATTERN_PLAYING,
	PATTERN_RECORDING
};

class Pattern {
public:
	Pattern(MidiMessageCollector* collector);
	virtual ~Pattern();

	void play();
	void stop();
	void pause();

	void getLengthBars(int bars);

	void addEvent(MidiMessage m); // new events merged on next Play or Stop
	void clear();

	// has to be called on time
	int tick();

	void setActivePhrase(int i);

	// checkout active phrase to do phrase operations, only allowed when stopped
	Phrase* checkoutActivePhrase();
	void checkinActivePhrase();

private:
	Phrase _phrases[16];
	int _activePhrase;
	int _checkedOutPhrase;

	int _lengthBars;
	int _lengthClocks;

	PatternState _state;

	MidiMessageCollector* _pMessageCollector;
};

#endif /* PATTERN_H_ */
