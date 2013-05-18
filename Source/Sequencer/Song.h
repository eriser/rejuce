/*
 * Song.h
 *
 *  Created on: 12 Apr 2013
 *      Author: gboxdev
 */

#ifndef SONG_H_
#define SONG_H_

#include "Pattern.h"
#include "../Host/HostEventListener.h"
#include "../Host/HostEvent.h"

enum SongState
{
	SONG_STOPPED =0,
	SONG_PAUSED,
	SONG_PLAYING,
	SONG_RECORDING,
};

enum MetronomeState
{
	METRONOME_OFF =0,
	METRONOME_RECORD,
	METRONOME_PLAY,
};

class Song {
public:

	Song();
	void init(HostEventListener* pHostEventListener);
	virtual ~Song();

	// live operations
	void play();
	void record();
	void stop();
	void pause();

	void setNextPattern(int i);
	void setMetronomeState(MetronomeState s);

	void setBpm(float bpm);
	float getBpm();

	int tick(MidiMessageCollector* pCollector);

	void addEvent(MidiMessage m); // new events merged on next Play or Stop
	void clear();

	int getCurrentPatternLengthClocks();

private:
	Pattern* _patterns[16];
	int _currentPattern;
	Pattern* _pCurrentPattern;
	int _nextPattern;
	int _clock;
	float _bpm;

	SongState _state;
	MetronomeState _metronomeState;

	HostEventListener* _pHostEventListener;

	int _currentPatternLengthClocks;
};

#endif /* SONG_H_ */
