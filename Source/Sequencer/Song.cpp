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
		_patterns[i] = new Section();
	}
}

void Song::init(HostEventListener* pHostEventListener)
{
	_pHostEventListener = pHostEventListener;

	for (int i=0;i<16;i++)
	{
		_patterns[i]->init(pHostEventListener);
	}

	_state = SONG_STOPPED;

	_currentPattern=0;
	_pCurrentPattern = _patterns[0];
	_nextPattern=0;
	_clock=0;
	_metronomeState=METRONOME_RECORD;

	setNextPattern(0);
}

Song::~Song()
{
	for (int i=0;i<16;i++)
	{
		delete _patterns[i];
	}
}

void Song::record()
{
	_pCurrentPattern->play();
	_state = SONG_RECORDING;
}

void Song::play()
{
	_pCurrentPattern->play();
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

void Song::setMetronomeState(MetronomeState s)
{
	_metronomeState = s;
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

	if (_state==SONG_PLAYING ||
		_state==SONG_RECORDING)
	{
		_pCurrentPattern->tick(pCollector);

		ret = _clock;

		// metronome
		if (_metronomeState!=METRONOME_OFF &&
				( (_metronomeState==METRONOME_RECORD) ? _state==SONG_RECORDING : _state==SONG_PLAYING))
		{
			if (_clock % PHRASE_CLOCKS == 0)
			{
				if (_clock == 0)
				{
					MidiMessage bip(0xf2,0x00,Time::getMillisecondCounterHiRes()/1000.0f);
					pCollector->addMessageToQueue(bip);
				}
				else
				{
					MidiMessage bop(0xf2,0x01,Time::getMillisecondCounterHiRes()/1000.0f);
					pCollector->addMessageToQueue(bop);
				}
			}
		}

		// pulse play light in time with beats
		if (_clock % PHRASE_CLOCKS == 0)
		{
			HostEvent h = HostEventFactory::event(HC_OUT_BEAT);
			_pHostEventListener->onHostEvent(h);
		}

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
	// filter some events...

	// filter stop notes event
	if (m.isControllerOfType(123))
	{
		return;
	}

	if (_state==SONG_RECORDING)
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

int Song::getCurrentPatternLengthClocks()
{
	return _currentPatternLengthClocks;
}


