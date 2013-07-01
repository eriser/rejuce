/*
 * Pattern.cpp
 *
 *  Created on: 12 Apr 2013
 *      Author: gnuvebox
 */

#include "Section.h"

Section::Section()
{
	for (int i=0;i<16;i++)
	{
		_phrases[i] = new Phrase();
	}
	init(nullptr);
}

void Section::init(HostEventListener* pHostEventListener)
{
	_ledPos =0;
	_activePhrase=0;
	_checkedOutPhrase=-1;
	_clock=0;
	_state = PATTERN_STOPPED;
	_pHostEventListener = pHostEventListener;

	for (int i=0;i<16;i++)
	{
		setActivePhrase(i);
		Phrase* phrase = checkoutActivePhrase();

		phrase->init(i);

		checkinActivePhrase();

		_mutes[i]=false;
	}
	setActivePhrase(0);
}

Section::~Section()
{
	for (int i=0;i<16;i++)
	{
		if (_phrases[i])
			delete _phrases[i];
	}

}

void Section::play()
{
	for (int i=0;i<16;i++)
	{
		_phrases[i]->play();
	}

	_state = PATTERN_PLAYING;
}

void Section::stop()
{
	for (int i=0;i<16;i++)
	{
		Phrase* phrase = _phrases[i];
		phrase->stop();
	}

	_state = PATTERN_STOPPED;
	_clock =0;
}

void Section::pause()
{
	for (int i=0;i<16;i++)
	{
		_phrases[i]->pause();
	}

	_state = PATTERN_PAUSED;
}

int Section::getLengthBars()
{
	return _lengthBars;
}

int Section::getLengthClocks()
{
	return _lengthClocks;
}

void Section::addEvent(MidiMessage m)
{
	if (_state==PATTERN_PLAYING)
	{
		//_phrases[_activePhrase]->addEvent(m);
		if ((m.getChannel()))
		{
			_phrases[m.getChannel()-1]->addEvent(m);
		}
	}
}

void Section::clear()
{
	if (_state==PATTERN_STOPPED)
	{
		for (int i=0;i<16;i++)
		{
			_phrases[i]->clear();
		}
	}
}

// has to be called on time
int Section::tick(MidiMessageCollector* pCollector)
{
	int ret = _clock;

	if (_state==PATTERN_PLAYING)
	{
		for (int i=0;i<16;i++)
		{
			_phrases[i]->tick(pCollector);
		}

		ret = _clock;

		// inc clock
		if (_clock < _lengthClocks-1)
			_clock++;
		else
		{
			_clock=0;
		}
	}

	return ret;
}

void Section::setActivePhrase(int i)
{
	// only allowed if we are stopped, and no phrase is checked out
	if (_state==PATTERN_STOPPED && _checkedOutPhrase==-1)
	{
		_activePhrase = i;
	}
}

// checkout active phrase to do phrase operations, only allowed when stopped
Phrase* Section::checkoutActivePhrase()
{
	if (_state!=PATTERN_STOPPED)
		return nullptr;

	_checkedOutPhrase = _activePhrase;

	// return the phrase so the caller can dick with it
	return _phrases[_activePhrase];
}

void Section::checkinActivePhrase()
{
	// the phrase will have been dicked with, so recalculate stuff
	Phrase* phrase = _phrases[_checkedOutPhrase];

	// recalculate pattern length
	int maxLengthBars = 0;
	int maxLengthClocks =0;
	for (int i=0;i<16;i++)
	{
		int lengthBars = _phrases[i]->getLengthBars();
		int lengthClocks = _phrases[i]->getLengthClocks();

		if (lengthBars > maxLengthBars)
			maxLengthBars = lengthBars;

		if (lengthClocks > maxLengthClocks)
			maxLengthClocks = lengthClocks;
	}

	_lengthBars = maxLengthBars;
	_lengthClocks = maxLengthClocks;

	printf("new phrase length is %d, section length %d\n",phrase->getLengthBars(),_lengthClocks);

	_checkedOutPhrase = -1;
}

bool Section::getIsMuted(int i)
{
	return _mutes[i];
}

void Section::toggleMuteState(int i)
{
	_mutes[i]=!_mutes[i];
}

Phrase* Section::getPhrase(int i)
{
	return _phrases[i];
}

