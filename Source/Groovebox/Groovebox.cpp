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

	for (int i=0;i<GC_SIZE;i++) {_controlState[i]=GE_INVALID;_controlValue[i]=0;}


	_transportState = TRANSPORT_STOPPED;
	_currentChannel=0;
	_transposeOffset=0;
	_keyboardMode = KEYBOARD_KB;
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
	// always store the control's state
	_controlState[event.control] = event.event;
	_controlValue[event.control] = event.argv;

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

				case GC_BUTTON_OCTDOWN:
					if (_transposeOffset-12 >= (0-24))
					{
						_transposeOffset-=12;
						// TODO: send message back of transpose value
					}
					break;
				case GC_BUTTON_OCTUP:
					if (_transposeOffset+12 <= (24))
					{
						_transposeOffset+=12;
						// TODO: send message back of transpose value
					}
					break;

				case GC_BUTTON_MUTE:
				case GC_BUTTON_SECTION:
					handleKeboardModeButton(event.control);
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

void Groovebox::handleKeboardModeButton(GrooveControlName control)
{
	// save old led values, so we know if we need to send messages later
	int oldLedStateMute = _controlValue[GCL_MUTE];
	int oldLedStateSection = _controlValue[GCL_SECTION];

	// act depending on which button was pressed, and update keyboard mode
	switch (control)
	{
	case GC_BUTTON_MUTE:
		if (_keyboardMode==KEYBOARD_MUTE)
			_keyboardMode=KEYBOARD_KB;
		else
			_keyboardMode=KEYBOARD_MUTE;
		break;
	case GC_BUTTON_SECTION:
		if (_keyboardMode==KEYBOARD_SECTION)
			_keyboardMode=KEYBOARD_KB;
		else
			_keyboardMode=KEYBOARD_SECTION;
		break;
	default:
		break;
	}

	// update lights depending on keyboard mode
	_controlValue[GCL_MUTE] = 0;
	_controlValue[GCL_SECTION] = 0;
	switch (_keyboardMode)
	{
	case KEYBOARD_MUTE:
		_controlValue[GCL_MUTE] = 3;
		break;
	case KEYBOARD_SECTION:
		_controlValue[GCL_SECTION] = 3;
		break;
	}

	// if values are different, output values
	if (oldLedStateMute != _controlValue[GCL_MUTE])
	{
		GrooveEvent ge = GrooveEventFactory::event(GE_LEDSET,GCL_MUTE,_controlValue[GCL_MUTE]);
		_interface->onGrooveEvent(ge);
	}
	if (oldLedStateSection != _controlValue[GCL_SECTION])
	{
		GrooveEvent ge = GrooveEventFactory::event(GE_LEDSET,GCL_SECTION,_controlValue[GCL_SECTION]);
		_interface->onGrooveEvent(ge);
	}
}

void Groovebox::handleKeyboardButton(bool bDown,GrooveControlName control)
{
	switch (_keyboardMode)
	{
	case KEYBOARD_KB:
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
		break;
	}
	case KEYBOARD_MUTE:
	case KEYBOARD_SECTION:
	{
		int n=-1;
		switch (control)
		{
		case GC_BUTTON_WHITE0: n=0; break;
		case GC_BUTTON_WHITE1: n=1; break;
		case GC_BUTTON_WHITE2: n=2; break;
		case GC_BUTTON_WHITE3: n=3; break;
		case GC_BUTTON_WHITE4: n=4; break;
		case GC_BUTTON_WHITE5: n=5; break;
		case GC_BUTTON_WHITE6: n=6; break;
		case GC_BUTTON_WHITE7: n=7; break;
		case GC_BUTTON_WHITE8: n=8; break;
		case GC_BUTTON_WHITE9: n=9; break;
		case GC_BUTTON_WHITE10: n=10; break;
		case GC_BUTTON_WHITE11: n=11; break;
		case GC_BUTTON_WHITE12: n=12; break;
		case GC_BUTTON_WHITE13: n=13; break;
		case GC_BUTTON_WHITE14: n=14; break;
		case GC_BUTTON_WHITE15: n=15; break;
		}

		if (n>=0)
		{
			if (_keyboardMode==KEYBOARD_MUTE)
			{
				HostEvent h = HostEventFactory::event(HC_PATTERN_SET_NEXT,n);
				_host->event(h);
			} else
			if (_keyboardMode==KEYBOARD_SECTION)
			{
				HostEvent h = HostEventFactory::event(HC_PHRASE_MUTE_TOGGLE,n);
				_host->event(h);
			}
		}

		break;
	}
	}
}

void Groovebox::setTransportLeds()
{
	GrooveEvent ge;

	int oldPlayLedState = _controlValue[GCL_PLAYING];
	int oldRecordLedState = _controlValue[GCL_RECORDING];

	if (_transportState==TRANSPORT_PLAYING)
	{
		_controlValue[GCL_PLAYING]=1;
	}
	if (_transportState==TRANSPORT_RECORDING)
	{
		_controlValue[GCL_RECORDING]=1;
		_controlValue[GCL_PLAYING]=1;
	}

	if (_transportState!=TRANSPORT_PLAYING &&
		_transportState!=TRANSPORT_RECORDING)
	{
		_controlValue[GCL_PLAYING]=0;
		_controlValue[GCL_RECORDING]=0;
	}

	// only send message if changed state
	if (oldPlayLedState!=_controlValue[GCL_PLAYING])
	{
		ge = GrooveEventFactory::event(GE_LEDSET,GCL_PLAYING,_controlValue[GCL_PLAYING]);
		_interface->onGrooveEvent(ge);
	}
	if (oldRecordLedState!=_controlValue[GCL_RECORDING])
	{
		ge = GrooveEventFactory::event(GE_LEDSET,GCL_RECORDING,_controlValue[GCL_RECORDING]);
		_interface->onGrooveEvent(ge);
	}
}

void Groovebox::onHostEvent(HostEvent& event)
{
	switch (event.name)
	{
		case HC_OUT_BEAT:
		{
			GrooveEvent ge = GrooveEventFactory::event(GE_LEDSET,GCL_PLAYING,GE_LED_PULSE);
			_interface->onGrooveEvent(ge);
			break;
		}
		default:
			break;
	}
}

