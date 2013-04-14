/*
 * Song.h
 *
 *  Created on: 12 Apr 2013
 *      Author: gboxdev
 */

#ifndef SONG_H_
#define SONG_H_

#include "Pattern.h"

enum SongState
{
	SONG_STOPPED =0,
	SONG_PAUSED,
	SONG_PLAYING,
	SONG_RECORDING
};

class Song {
public:

	Song();
	Song(MidiMessageCollector* pMessageCollector);
	void init(MidiMessageCollector* pMessageCollector);
	virtual ~Song();

	// live operations
	void play();
	void stop();
	void pause();

	void setNextPattern(int i);

	void setBpm(float bpm);
	float getBpm();

	int tick();

	void addEvent(MidiMessage m); // new events merged on next Play or Stop
	void clear();

private:
	Pattern _patterns[16];
	int _currentPattern;
	Pattern* _pCurrentPattern;
	int _nextPattern;
	int _clock;
	float _bpm;

	SongState _state;

	int _currentPatternLengthClocks;
};

#endif /* SONG_H_ */
