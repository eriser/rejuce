/*
 * Pattern.cpp
 *
 *  Created on: 12 Apr 2013
 *      Author: gnuvebox
 */

#include "Pattern.h"

Pattern::Pattern()
{
	init(nullptr);
}

Pattern::Pattern(MidiMessageCollector* pMessageCollector)
{
	init(pMessageCollector);
}

void Pattern::init(MidiMessageCollector* pMessageCollector)
{
	_activePhrase=0;
	_checkedOutPhrase=-1;
	_pMessageCollector=pMessageCollector;
	_clock=0;
	_state = PATTERN_STOPPED;

	for (int i=0;i<16;i++)
	{
		setActivePhrase(i);
		Phrase* phrase = checkoutActivePhrase();

		phrase->init(pMessageCollector,i);

		checkinActivePhrase();

		_mutes[16]=false;
	}
}

Pattern::~Pattern()
{

}

void Pattern::play()
{
	for (int i=0;i<16;i++)
	{
		_phrases[i].play();
	}

	_state = PATTERN_PLAYING;
}

void Pattern::stop()
{
	for (int i=0;i<16;i++)
	{
		_phrases[i].stop();
	}

	_state = PATTERN_STOPPED;
	_clock =0;
}

void Pattern::pause()
{
	for (int i=0;i<16;i++)
	{
		_phrases[i].pause();
	}

	_state = PATTERN_PAUSED;
}

int Pattern::getLengthBars()
{
	return _lengthBars;
}

int Pattern::getLengthClocks()
{
	return _lengthClocks;
}

void Pattern::addEvent(MidiMessage m)
{
	if (_state==PATTERN_PLAYING)
	{
		_phrases[_activePhrase].addEvent(m);
	}
}

void Pattern::clear()
{
	if (_state==PATTERN_STOPPED)
	{
		for (int i=0;i<16;i++)
		{
			_phrases[i].clear();
		}
	}
}

// has to be called on time
int Pattern::tick()
{
	int ret = _clock;

	if (_state==PATTERN_PLAYING)
	{
		for (int i=0;i<16;i++)
		{
			_phrases[i].tick();
		}

		ret = _clock;

		// inc clock
		if (_clock < _lengthClocks-1)
			_clock++;
		else
			_clock=0;
	}

	return ret;
}

void Pattern::setActivePhrase(int i)
{
	// only allowed if we are stopped, and no phrase is checked out
	if (_state==PATTERN_STOPPED && _checkedOutPhrase==-1)
	{
		_activePhrase = i;
	}
}

// checkout active phrase to do phrase operations, only allowed when stopped
Phrase* Pattern::checkoutActivePhrase()
{
	if (_state!=PATTERN_STOPPED)
		return nullptr;

	_checkedOutPhrase = _activePhrase;

	// return the phrase so the caller can dick with it
	return &_phrases[_activePhrase];
}

void Pattern::checkinActivePhrase()
{
	// the phrase will have been dicked with, so recalculate stuff
	Phrase* phrase = &_phrases[_checkedOutPhrase];

	// recalculate pattern length
	int maxLengthBars = 0;
	int maxLengthClocks =0;
	for (int i=0;i<16;i++)
	{
		int lengthBars = _phrases[i].getLengthBars();
		int lengthClocks = _phrases[i].getLengthBars();

		if (lengthBars > maxLengthBars)
			maxLengthBars = lengthBars;

		if (lengthClocks > maxLengthClocks)
			maxLengthClocks = lengthClocks;
	}

	_checkedOutPhrase = -1;
}

bool Pattern::getIsMuted(int i)
{
	return _mutes[i];
}

void Pattern::toggleMuteState(int i)
{
	_mutes[i]=!_mutes[i];
}
