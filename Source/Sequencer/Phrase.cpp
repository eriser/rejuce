/*
 * phrase.cpp
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#include "Phrase.h"

Phrase::Phrase(MidiMessageCollector* pCollector,int channel)
{
	_origin = 0.0f;
	_channel = channel;
	_state = PHRASE_STOPPED;
	_pCollector = pCollector;
	_lengthBars = 1;
	_timeSigNumerator = 4;
	_timeSigDenominator = 4;
	_pseqIter = new MidiBuffer::Iterator(_seq);

	_curMessagePos =0;
	_haveEvent = false;
}

Phrase::~Phrase() {
	if (_pseqIter)
	{
		delete _pseqIter;
	}
}

void Phrase::Debug()
{
	std::cout<<"Debug dump phrase, channel "<<_channel<<std::endl;

	_pseqIter->setNextSamplePosition(0);

	MidiMessage m;
	int p;
	while (_pseqIter->getNextEvent(m,p))
	{
		std::cout<<p<<" : "<<m.getRawData()[0]<<m.getRawData()[1]<<m.getRawData()[2]<<m.getRawData()[3]<<std::endl;
	}

	_pseqIter->setNextSamplePosition(0);

}

void Phrase::Tick(double time)
{

	if (_state == PHRASE_PLAYING)
	{
		// at least some events?
		if (!_seq.isEmpty())
		{
			double pos = time - _origin;

			//do we have an event leftover from last time?
			if (_haveEvent)
			{
				// is it in the future?
				if (_curMessage.getTimeStamp() > pos)
				{
					// yes, so bail
					return;
				}
				else
				{
					// its not in the future, so get it in the queue
					MidiMessage m(_curMessage);
					m.setTimeStamp(time);
					_pCollector->addMessageToQueue(m);
					_haveEvent = false;
				}
			}

			// now keep getting events, until we get one that is in the future.
			while (_pseqIter->getNextEvent(_curMessage,_curMessagePos))
			{
				_haveEvent = true;

				// is it in the future?
				if (_curMessage.getTimeStamp() > pos)
				{
					// yes, so bail
					return;
				}
				else
				{
					// its not in the future, so get it in the queue
					MidiMessage m(_curMessage);
					m.setTimeStamp(time);
					_pCollector->addMessageToQueue(m);
					_haveEvent = false;
				}
			}
		}// seq is empty

	}// state==playing

}

void Phrase::MergeScratchBuffer()
{
	if (!_scratch.isEmpty())
	{
		_seq.addEvents(_scratch,0,-1,0);
		_scratch.clear();
	}
}

void Phrase::Play(double time)
{
	// are we coming from stopped, or pause?
	if (_state == PHRASE_STOPPED)
	{
		// reset origin to now
		_origin = time;

		// clear record buffer
		_scratch.clear();
	}
	else if (_state == PHRASE_PLAYING)
	{
		MergeScratchBuffer();
	}

	_state = PHRASE_PLAYING;
}

void Phrase::Stop()
{
	_state = PHRASE_STOPPED;

	MergeScratchBuffer();
}

void Phrase::Pause()
{
	_state = PHRASE_PAUSED;
}

void Phrase::AddEvent(MidiMessage m)
{
	_scratch.addEvent(m,_scratch.getNumEvents()+1);
}
