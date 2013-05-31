/*
 * Groovebox.cpp
 *
 *  Created on: 16 May 2013
 *      Author: gnvbx
 */

#include "Groovebox.h"

Groovebox::Groovebox(Host* pHost,GrooveboxInterface* pInterface) :GrooveEventListener(), HostEventListener()
{
	_host = pHost;
	_interface = pInterface;
	_host->setHostEventListener(this);

	for (int i=0;i<GC_SIZE;i++) _controlState[i]=0;


	_transportState = TRANSPORT_STOPPED;
	_currentChannel=0;
	_transposeOffset=0;
}

Groovebox::~Groovebox()
{
	delete _interface;
}

void Groovebox::start()
{
	_interface->start();
}

void Groovebox::stop()
{
	_interface->stop();
}

void Groovebox::onGrooveEvent(GrooveEvent& event)
{
	// events coming in from interface. update local state or fire host event
	switch (event.event)
	{
		case GE_BUTTON_DOWN:
		{
			switch (event.control)
			{
				case GC_BUTTON_STOP:
					_host->event(HostEventFactory::event(HC_TRANSPORT_STOP));
					_transportState = TRANSPORT_STOPPED;
					setTransportLeds();
					break;
				case GC_BUTTON_PLAY:
					_host->event(HostEventFactory::event(HC_TRANSPORT_PLAY));
					_transportState = TRANSPORT_PLAYING;
					setTransportLeds();
					break;
				case GC_BUTTON_PAUSE:
					_host->event(HostEventFactory::event(HC_TRANSPORT_PAUSE));
					_transportState = TRANSPORT_PAUSED;
					setTransportLeds();
					break;
				case GC_BUTTON_BACK:
					_host->event(HostEventFactory::event(HC_TRANSPORT_REWIND));
					break;
				case GC_BUTTON_RECORD:
					_host->event(HostEventFactory::event(HC_TRANSPORT_RECORD));
					_transportState = TRANSPORT_RECORDING;
					setTransportLeds();
					break;

				default:
					// notes
					if (event.control>=GC_BUTTON_WHITE0 && event.control<=GC_BUTTON_WHITE15)
						handleKeyboardButton(true,event.control);
					break;
			}
			break;
		}

		case GE_BUTTON_UP:
		{
			switch (event.control)
			{
			default:
				handleKeyboardButton(false,event.control);
				break;
			}
			break;
		}

		default:
			break;

	}
}

void Groovebox::handleKeyboardButton(bool bDown,GrooveControlName control)
{
	int note=0;
	int velocity = 100;	// TODO: we could take this value from the interface in argv
	MidiMessage m;

	// 58 is midi note of lowest note on keyboard
	// GC_BUTTON_WHITE0 is first key, and keys are in contiguous ascending order
	note = 58 + _transposeOffset + control - GC_BUTTON_WHITE0;

	if (bDown)
	{
		m = MidiMessage::noteOn(_currentChannel+1,note,(unsigned char)velocity);
	}
	else
	{
		m = MidiMessage::noteOff(_currentChannel+1,note,(unsigned char)velocity);
	}

	m.setTimeStamp(Time::getMillisecondCounterHiRes()/1000.0f);

	_host->event(HostEventFactory::event(m));
}

void Groovebox::setTransportLeds()
{
	GrooveEvent ge;

	int oldPlayLedState = _controlState[GCL_PLAYING];
	int oldRecordLedState = _controlState[GCL_RECORDING];

	if (_transportState==TRANSPORT_PLAYING)
	{
		_controlState[GCL_PLAYING]=1;
	}
	if (_transportState==TRANSPORT_RECORDING)
	{
		_controlState[GCL_RECORDING]=1;
		_controlState[GCL_PLAYING]=1;
	}

	if (_transportState!=TRANSPORT_PLAYING &&
		_transportState!=TRANSPORT_RECORDING)
	{
		_controlState[GCL_PLAYING]=0;
		_controlState[GCL_RECORDING]=0;
	}

	// only send message if changed state
	if (oldPlayLedState!=_controlState[GCL_PLAYING])
	{
		ge = GrooveEventFactory::event(GE_LEDSET,GCL_PLAYING,_controlState[GCL_PLAYING]);
		_interface->onGrooveEvent(ge);
	}
	if (oldRecordLedState!=_controlState[GCL_RECORDING])
	{
		ge = GrooveEventFactory::event(GE_LEDSET,GCL_RECORDING,_controlState[GCL_RECORDING]);
		_interface->onGrooveEvent(ge);
	}
}

void Groovebox::setSemiLedsOff()
{

}

void Groovebox::setLedPos(HostEvent& event)
{
	// TODO
//	GrooveEvent ge = GrooveEventFactory::event(GCL_,event.argv[0]);
//				_interface->onGrooveEvent(ge);
}

void Groovebox::onHostEvent(HostEvent& event)
{
	switch (event.name)
	{
		case HC_OUT_LEDPOS:
		{
			setLedPos(event);
			break;
		}
		default:
			break;
	}
}

