/*
 * Pattern.cpp
 *
 *  Created on: 12 Apr 2013
 *      Author: gnuvebox
 */

#include "Pattern.h"

Pattern::Pattern(MidiMessageCollector* pMessageCollector)
{
	_activePhrase=0;
	_checkedOutPhrase=-1;
	_pMessageCollector=pMessageCollector;

	for (int i=0;i<16;i++)
	{
		_phrases[i].init(pMessageCollector,i);
	}
}

Pattern::~Pattern()
{

}

void Pattern::play()
{

}

void Pattern::stop()
{

}

void Pattern::pause()
{

}

int Pattern::getLengthBars(int bars)
{
	return _lengthBars;
}

int Pattern::getLengthClocks()
{
	return _lengthClocks;
}

void Pattern::addEvent(MidiMessage m)
{

}

void Pattern::clear()
{

}

// has to be called on time
int Pattern::tick()
{

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



	_checkedOutPhrase = -1;
}

