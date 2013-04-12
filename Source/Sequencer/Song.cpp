/*
 * Song.cpp
 *
 *  Created on: 12 Apr 2013
 *      Author: gboxdev
 */

#include "Song.h"

Song::Song(MidiMessageCollector* pMessageCollector)
{
	for (int i=0;i<16;i++)
	{
		_patterns[i].init(pMessageCollector);
	}

	_state = SONG_STOPPED;

	_currentPattern=0;
	_nextPattern=0;
	_clock=0;

	setNextPattern(0);
}

Song::~Song()
{

}

void Song::play()
{
	_state = SONG_PLAYING;
}

void Song::stop()
{
	_pCurrentPattern->stop();
	_clock =0;

	_state = SONG_STOPPED;
}

void Song::pause()
{
	_pCurrentPattern->pause();

	_state = SONG_PAUSED;
}

void Song::setNextPattern(int i)
{
	_nextPattern = i;

	// if we are stopped, we can set the pattern immediately
	if (_state == SONG_STOPPED)
	{
		_pCurrentPattern->stop();

		_currentPattern = i;
		_pCurrentPattern = &_patterns[i];

		_currentPatternLengthClocks = _pCurrentPattern->getLengthClocks();
	}
}

void Song::setBpm(float bpm)
{
	_bpm = bpm;
}

float Song::getBpm()
{
	return _bpm;
}


int Song::tick()
{
	int ret = _clock;



	return ret;
}


