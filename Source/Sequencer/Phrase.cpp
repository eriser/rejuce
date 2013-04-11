/*
 * phrase.cpp
 *
 *  Created on: 7 Apr 2013
 *      Author: gboxdev
 */

#include "Phrase.h"

Phrase::Phrase(MidiMessageCollector* pCollector,int channel)
{
	_channel = channel;
	_state = PHRASE_STOPPED;
	_pCollector = pCollector;
	_lengthBars = 1;
	_timeSigNumerator = 4;
	_timeSigDenominator = 4;

	_curMessageClock =0;
	_haveEvent = false;
	_clock =0;
	_pseqIter = nullptr;

	setTimeSignature(4,4);
	setLengthBars(1);
}

void Phrase::setLengthBars(int bars)
{
	_lengthBars= bars;

	int beatsPerBar = _timeSigNumerator;
	int clocksPerBeat = ( _timeSigDenominator / 4.0f ) * PHRASE_CLOCKS;

	_lengthClocks = beatsPerBar * clocksPerBeat;

	_scratch.ensureSize(_lengthClocks*sizeof(int));
	_seq.ensureSize(_lengthClocks*sizeof(int));

	if (_pseqIter)
	{
		delete _pseqIter;
	}
	_pseqIter = new MidiBuffer::Iterator(_seq);
}

void Phrase::setTimeSignature(int numerator,int denominator)
{
	_timeSigNumerator = numerator;
	_timeSigDenominator = denominator;

	setLengthBars(_lengthBars);
}

Phrase::~Phrase()
{
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

void Phrase::debug()
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

int Phrase::tick()
{
	if (_state == PHRASE_PLAYING)
	{
		// at least some events?
		if (!_seq.isEmpty())
		{

			//do we have an event leftover from last time?
			if (_haveEvent)
			{
				// is it for now?
				if (_curMessageClock == _clock)
				{
					// get it in the queue
					MidiMessage m(_curMessage);
					_pCollector->addMessageToQueue(m);
					_haveEvent = false;

					_DebugEvent(_clock,m);
				}
			}

			// only get more messages if we dont have one still 'in hand'
			if (!_haveEvent)
			{
				// keep getting events, until we get one that is in the future.
				while (_pseqIter->getNextEvent(_curMessage,_curMessageClock))
				{
					_haveEvent = true;

//printf("have message in loop clock %d, message clock = %d;\n",_clock,_curMessageClock);

					// is it in the future?
					if (_curMessageClock > _clock)
					{
						// yes, so bail
//printf("break;\n");
						break;
					}
					else if (_curMessageClock == _clock)
					{
						// its not in the future, so get it in the queue
						MidiMessage m(_curMessage);
						_pCollector->addMessageToQueue(m);
						_haveEvent = false;

						_DebugEvent(_clock,m);
					}
				}
			}
		}// seq is empty

	}// state==playing

	int ret = _clock;

	// inc clock
	if (_clock < _lengthClocks-1)
		_clock++;
	else
		_clock=0;

	return ret;
}

void Phrase::MergeScratchBuffer()
{
	//Debug();

	if (!_scratch.isEmpty())
	{
		_seq.addEvents(_scratch,0,-1,0);
		_scratch.clear();
	}

	//printf("after merge\n");
	//Debug();
}

void Phrase::play()
{
	_state = PHRASE_PLAYING;
}

void Phrase::stop()
{
	_state = PHRASE_STOPPED;

	_clock =0;
	_haveEvent = false;
	_curMessage =0;
	_curMessageClock =0;


	MergeScratchBuffer();
}

void Phrase::pause()
{
	_state = PHRASE_PAUSED;
}

void Phrase::addEvent(MidiMessage m)
{
	_scratch.addEvent(m,_clock);

	//_DebugEvent(_clock,m);
}
