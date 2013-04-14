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
	PATTERN_PLAYING
};

class Song;

class Pattern {
public:
	Pattern();
	Pattern(MidiMessageCollector* collector);
	void init(MidiMessageCollector* collector);
	virtual ~Pattern();

	// control/transport
	void play();
	void stop();
	void pause();

	void addEvent(MidiMessage m); // new events merged on next Play or Stop
	void clear();

	bool getIsMuted(int i);
	void toggleMuteState(int i);

	// info
	int getLengthBars();
	int getLengthClocks();

	// has to be called on time
	int tick();

	void setActivePhrase(int i);

	// checkout active phrase to do phrase operations, only allowed when stopped
	Phrase* checkoutActivePhrase();
	void checkinActivePhrase();

private:
	int _clock;
	Phrase _phrases[16];
	bool _mutes[16];
	int _activePhrase;
	int _checkedOutPhrase;

	int _lengthBars;
	int _lengthClocks;

	PatternState _state;

	MidiMessageCollector* _pMessageCollector;
};

#endif /* PATTERN_H_ */
