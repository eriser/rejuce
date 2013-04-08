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

void _DebugEvent(int p,MidiMessage m)
{
	printf("%d: %02x %02x %02x %02x\n",
					p,
					m.getRawData()[0],m.getRawData()[0],m.getRawData()[1],m.getRawData()[2],m.getRawData()[3]);
}

void Phrase::Debug()
{
	std::cout<<"Debug dump phrase, channel "<<_channel<<std::endl;

	_pseqIter->setNextSamplePosition(0);

	MidiMessage m;
	int p;
	while (_pseqIter->getNextEvent(m,p))
	{
		_DebugEvent(p,m);
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
				if (_curMessagePos > pos)
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

					_DebugEvent(0,m);
				}
			}

			// now keep getting events, until we get one that is in the future.
			while (_pseqIter->getNextEvent(_curMessage,_curMessagePos))
			{
				_haveEvent = true;

				// is it in the future?
				if (_curMessagePos > pos)
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

					_DebugEvent(0,m);
				}
			}
		}// seq is empty

	}// state==playing

}

void Phrase::MergeScratchBuffer()
{
	Debug();

	if (!_scratch.isEmpty())
	{
		_seq.addEvents(_scratch,0,-1,0);
		_scratch.clear();
	}

	printf("after merge\n");
	Debug();
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
	m.setTimeStamp(m.getTimeStamp()-_origin);

	_scratch.addEvent(m,m.getTimeStamp());

	_DebugEvent(m.getTimeStamp(),m);
}
