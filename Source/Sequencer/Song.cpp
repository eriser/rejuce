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
		_sections[i] = new Section();
	}
}

void Song::init(HostEventListener* pHostEventListener)
{
	_pHostEventListener = pHostEventListener;

	for (int i=0;i<16;i++)
	{
		_sections[i]->init(pHostEventListener);
	}

	_state = SONG_STOPPED;

	_currentSection=0;
	_pCurrentSection = _sections[0];
	_nextSection=0;
	_clock=0;
	_metronomeState=METRONOME_RECORD;

	setNextSection(0);
}

Song::~Song()
{
	for (int i=0;i<16;i++)
	{
		delete _sections[i];
	}
}

void Song::record()
{
	_pCurrentSection->play();
	_state = SONG_RECORDING;
}

void Song::play()
{
	_pCurrentSection->play();
	_state = SONG_PLAYING;
}

void Song::stop()
{
	_pCurrentSection->stop();
	_clock =0;

	_state = SONG_STOPPED;
}

void Song::pause()
{
	_pCurrentSection->pause();

	_state = SONG_PAUSED;
}

int Song::getCurrentSection()
{
	return _currentSection;
}

int Song::getNextSection()
{
	return _nextSection;
}

void Song::togglePhraseMute(int i)
{
	_pCurrentSection->toggleMuteState(i);
}

bool Song::getPhraseMuteState(int i)
{
	return _pCurrentSection->getIsMuted(i);
}

void Song::setNextSection(int i)
{
	_nextSection = i;

	// if we are stopped, we can set the pattern immediately
	if (_state == SONG_STOPPED)
	{
		_pCurrentSection->stop();

		_currentSection = i;
		_pCurrentSection = _sections[i];

		_currentPatternLengthClocks = _pCurrentSection->getLengthClocks();
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
		_pCurrentSection->tick(pCollector);

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
			if (_currentSection != _nextSection)
			{
				_pCurrentSection->stop();

				_currentSection = _nextSection;
				_pCurrentSection = _sections[_nextSection];

				_currentPatternLengthClocks = _pCurrentSection->getLengthClocks();
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
		_sections[_currentSection]->addEvent(m);
	}
}

void Song::clear()
{
	if (_state == SONG_STOPPED)
	{
		for (int i=0;i<16;i++)
		{
			_sections[i]->clear();
		}
	}
}

int Song::getCurrentPatternLengthClocks()
{
	return _currentPatternLengthClocks;
}


