/*
 * Pattern.h
 *
 *  Created on: 12 Apr 2013
 *      Author: gnuvebox
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include "../Host/HostEvent.h"
#include "../Host/HostEventListener.h"
#include "Phrase.h"
class Phrase;

enum SectionState
{
	PATTERN_STOPPED =0,
	PATTERN_PAUSED,
	PATTERN_PLAYING
};

class Song;

class Section {
public:
	Section();
	void init(HostEventListener* pHostEventListener);
	virtual ~Section();

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
	int tick(MidiMessageCollector* pCollector);

	void setActivePhrase(int i);

	// checkout active phrase to do phrase operations, only allowed when stopped
	Phrase* checkoutActivePhrase();
	void checkinActivePhrase();

private:
	int _clock;
	Phrase* _phrases[16];
	bool _mutes[16];
	int _activePhrase;
	int _checkedOutPhrase;

	int _lengthBars;
	int _lengthClocks;

	int _ledPos;

	SectionState _state;

	HostEventListener* _pHostEventListener;

};

#endif /* PATTERN_H_ */
