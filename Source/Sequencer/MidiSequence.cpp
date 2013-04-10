/*
 * MidiSequence.cpp
 *
 *  Created on: 10 Apr 2013
 *      Author: gboxdev
 */

#include "MidiSequence.h"

MidiSequence::MidiSequence() {
	_events = nullptr;
}

MidiSequence::~MidiSequence() {
	if (_events)
		delete[] _events;
}

void MidiSequence::reset(int length)
{
	if (_events)
		delete[] _events;

	_events = new Array<int>[length];
	_length = length;
}

void MidiSequence::addEvent(int pos,int rawMidi)
{
	Array<int>* posEvents = &_events[pos];
	posEvents->add(rawMidi);
}

int MidiSequence::getEventCountAt(int pos)
{
	Array<int>* posEvents = &_events[pos];
	return posEvents->size();
}

int MidiSequence::getEventAt(int pos,int which)
{
	Array<int>* posEvents = &_events[pos];
	return posEvents->operator [](which);
}

