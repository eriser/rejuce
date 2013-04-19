/*
 * Song.cpp
 *
 *  Created on: 12 Apr 2013
 *      Author: gboxdev
 */

#include "Song.h"

Song::Song()
{
	for (int i=0;i<16;i++)
	{
		_patterns[i] = new Pattern();
	}
}

void Song::init()
{
	for (int i=0;i<16;i++)
	{
		_patterns[i]->init();
	}

	_state = SONG_STOPPED;

	_currentPattern=0;
	_pCurrentPattern = _patterns[0];
	_nextPattern=0;
	_clock=0;

	setNextPattern(0);
}

Song::~Song()
{
	for (int i=0;i<16;i++)
	{
		delete _patterns[i];
	}
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
		_pCurrentPattern = _patterns[i];

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


int Song::tick(MidiMessageCollector* pCollector)
{
	int ret = _clock;

	if (_state==SONG_PLAYING)
	{
		_pCurrentPattern->tick(pCollector);

		ret = _clock;

		// inc clock
		if (_clock < _currentPatternLengthClocks-1)
		{
			_clock++;
		}
		else
		{
			// we might need to change pattern.
			if (_currentPattern != _nextPattern)
			{
				_pCurrentPattern->stop();

				_currentPattern = _nextPattern;
				_pCurrentPattern = _patterns[_nextPattern];

				_currentPatternLengthClocks = _pCurrentPattern->getLengthClocks();
			}

			_clock=0;
		}

	}

	return ret;
}

void Song::addEvent(MidiMessage m)
{
	if (_state==SONG_PLAYING)
	{
		_patterns[_currentPattern]->addEvent(m);
	}
}

void Song::clear()
{
	if (_state == SONG_STOPPED)
	{
		for (int i=0;i<16;i++)
		{
			_patterns[i]->clear();
		}
	}
}


