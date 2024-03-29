/*
 * Groovebox.cpp
 *
 *  Created on: 16 May 2013
 *      Author: gnvbx
 */

#include "Groovebox.h"
#include "../Noisemaker/Engine/Params.h"

Groovebox::Groovebox(Host* pHost,GrooveboxInterface* pInterface) :GrooveEventListener(), HostEventListener()
{
	_host = pHost;
	_interface = pInterface;
	_host->setHostEventListener(this);
	_interface->setScreenManager(&_screenManager);

	for (int i=0;i<GC_SIZE;i++) {_controlState[i]=GE_INVALID;_controlValue[i]=0;}

	_transportState = TRANSPORT_STOPPED;
	_currentTrack=0;
	_transposeOffset=0;
	_octaveOffset=0;
	_keyboardMode = KEYBOARD_KB;

	_shiftTrack = false;
	_shiftTranspose = false;
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
	// some keys are shift keys. when they are pressed we only allow the keys they operate
	// on to be pressed as well, using a not particularly elegant mechanism

	// TODO: device hopefully not terrible mechanism (for the above)

	// if we are being asked to store something
	if (event.getEvent()!= GE_REQUESTCONTROLSTATE &&
		event.getEvent()!= GE_REQUESTCONTROLVALUE )
	{
		// store the control's state
		_controlState[event.getControl()] = event.getEvent();
		_controlValue[event.getControl()] = event.getInt();
	}

	// events coming in from interface. update local state or fire host event
	switch (event.getEvent())
	{
		case GE_REQUESTCONTROLSTATE:
		{
			GrooveEvent ge = GrooveEvent(GE_REQUESTCONTROLSTATE,event.getControl(),_controlState[event.getControl()]);
			_interface->onGrooveEvent(ge);
			break;
		}
		case GE_REQUESTCONTROLVALUE:
		{
			GrooveEvent ge = GrooveEvent(GE_REQUESTCONTROLSTATE,event.getControl(),_controlValue[event.getControl()]);
			_interface->onGrooveEvent(ge);
			break;
		}
		case GE_PARAM:
		{
			switch (event.getControl())
			{
				case GCP_REC_METSTATE:
					_host->event(HostEventFactory::event(HC_REC_METSTATE,event.getInt()));
					break;
				case GCP_REC_METRONOME:
					_host->event(HostEventFactory::event(HC_REC_METRONOME,event.getInt()));
					break;
				case GCP_REC_AUTOQUANT:
					_host->event(HostEventFactory::event(HC_SONG_AUTOQUANTISE,event.getInt()));
					break;
				case GCP_PHRASE_LENGTH:
					_host->event(HostEventFactory::event(HC_PHRASE_LENGTH,event.getInt()));
					break;
				case GCP_PHASE_QUANTISENUM:
					_host->event(HostEventFactory::event(HC_SONG_QUANTISENUMERATOR,event.getInt()));
				case GCP_PHASE_QUANTISEDIV:
					_host->event(HostEventFactory::event(HC_SONG_QUANTISEDIVISOR,event.getInt()));
				default:
					break;
			}
			break;
		}
		case GE_BUTTON_DOWN:
		{
			switch (event.getControl())
			{
				case GCB_PHASE_QUANTISE:
					_host->event(HostEventFactory::event(HC_PHASE_QUANTISE));
					break;
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
					if (_octaveOffset-1 >= (0-3))
					{
						_octaveOffset--;
						// TODO: send message back of transpose value
					}
					break;
				case GC_BUTTON_OCTUP:
					if (_octaveOffset+1 <= (3))
					{
						_octaveOffset++;
						// TODO: send message back of transpose value
					}
					break;

				case GC_BUTTON_MUTE:
				case GC_BUTTON_SECTION:
					handleKeboardModeButton(event.getControl());
					break;

				case GC_BUTTON_TRACK:
				case GC_BUTTON_TRANSPOSE:
					if (event.getControl() == GC_BUTTON_TRACK)
						_keyboardMode = KEYBOARD_TRACK;
					else
						_keyboardMode = KEYBOARD_TRANSPOSE;
					handleTrackTransposeButton(true,event.getControl());
					break;

				default:
					// notes
					if (event.getControl()>=GC_BUTTON_WHITE0 && event.getControl()<=GC_BUTTON_WHITE15)
						handleKeyboardButton(true,event.getControl());
					break;
			}
			break;
		}

		case GE_BUTTON_UP:
		{
			switch (event.getControl())
			{
			case GC_BUTTON_TRACK:
			case GC_BUTTON_TRANSPOSE:
				_keyboardMode = KEYBOARD_KB;
				handleTrackTransposeButton(false,event.getControl());
				break;
			default:
				handleKeyboardButton(false,event.getControl());
				break;
			}
			break;
		}

		case GE_KNOB:
		{
			int controller = 0;

			switch (event.getControl())
			{
			case GCK_FILTER_TYPE:		controller = FILTERTYPE;	break;
			case GCK_FILTER_CUTOFF:		controller = CUTOFF;		break;
			case GCK_FILTER_RES:		controller = RESONANCE;		break;
			case GCK_FILTER_KEYFOLLOW:	controller = KEYFOLLOW;		break;
			case GCK_FILTER_CONTOUR:	controller = FILTERCONTOUR;	break;
			case GCK_FILTER_A:			controller = FILTERATTACK;	break;
			case GCK_FILTER_D:			controller = FILTERDECAY;	break;
			case GCK_FILTER_S:			controller = FILTERSUSTAIN;	break;
			case GCK_FILTER_R:			controller = FILTERRELEASE;	break;
			break;
			}

			if (controller)
			{
				int val = event.getInt();
				if (val<0) val=0;
				if (val>127) val=127;

				MidiMessage m = MidiMessage::controllerEvent(_currentTrack+1,controller,val);

				m.setTimeStamp(Time::getMillisecondCounterHiRes()/1000.0f);
				HostEvent h;
				h = HostEventFactory::event(m);
				_host->event(h);
			}
			break;
		}

		default:
			break;

	}
}

void Groovebox::handleTrackTransposeButton(bool bDown,GrooveControlName control)
{
	for (int i=0;i<16;i++)
	{
		int led = GE_LED_OFF;

		if (_keyboardMode == KEYBOARD_TRACK)
		{
			if (i == _currentTrack)
				led = bDown ? GE_LED_ON : GE_LED_OFF;
		}
		if (_keyboardMode == KEYBOARD_TRANSPOSE)
		{
			if (i == _transposeOffset+8)
				led = bDown ? GE_LED_ON : GE_LED_OFF;
		}

		if (_controlValue[(GrooveControlName)(GCL_SEMI_0 + i)]!=led)
		{
			_controlValue[(GrooveControlName)(GCL_SEMI_0 + i)]=led;
			GrooveEvent ge = GrooveEvent(GE_LEDSET,(GrooveControlName)(GCL_SEMI_0 + i),led);
			_interface->onGrooveEvent(ge);
		}
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
		{
			// enter mute mode, set out led and heads-up to the host
			_controlValue[GCL_MUTE] = 3;
			break;
		}
		case KEYBOARD_SECTION:
		{
			// enter section mode, set led and heads-up to host
			_controlValue[GCL_SECTION] = 3;
			break;
		}
	}

	// if values are different, output values
	if (oldLedStateMute != _controlValue[GCL_MUTE])
	{
		GrooveEvent ge = GrooveEvent(GE_LEDSET,GCL_MUTE,_controlValue[GCL_MUTE]);
		_interface->onGrooveEvent(ge);
	}
	if (oldLedStateSection != _controlValue[GCL_SECTION])
	{
		GrooveEvent ge = GrooveEvent(GE_LEDSET,GCL_SECTION,_controlValue[GCL_SECTION]);
		_interface->onGrooveEvent(ge);
	}

	// loop over keyboard leds to set them to represent the mute or section status
	Song* song = _host->getSequencer()->getSong();
	for (int i=0;i<16;i++)
	{
		int led = GE_LED_OFF;

		if (_keyboardMode == KEYBOARD_MUTE)
		{
			led = song->getPhraseMuteState(i) ? GE_LED_OFF : GE_LED_ON;
		}
		if (_keyboardMode == KEYBOARD_SECTION)
		{
			led = GE_LED_OFF;
			if (i==song->getCurrentSection())
			{
				led = GE_LED_ON;
			}
			else if (i==song->getNextSection())
			{
				led = GE_LED_FLASH;
			}
			else
			{
				led = GE_LED_OFF;
			}
		}

		_controlValue[(GrooveControlName)(GCL_SEMI_0 + i)]=led;
		GrooveEvent ge = GrooveEvent(GE_LEDSET,(GrooveControlName)(GCL_SEMI_0 + i),led);
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
		note = 58 + _transposeOffset + (12*_octaveOffset) + control - GC_BUTTON_WHITE0;

		if (bDown)
		{
			m = MidiMessage::noteOn(_currentTrack+1,note,(unsigned char)velocity);
		}
		else
		{
			m = MidiMessage::noteOff(_currentTrack+1,note,(unsigned char)velocity);
		}

		m.setTimeStamp(Time::getMillisecondCounterHiRes()/1000.0f);

		_host->event(HostEventFactory::event(m));
		break;
	}
	case KEYBOARD_MUTE:
	case KEYBOARD_SECTION:
	case KEYBOARD_TRACK:
	case KEYBOARD_TRANSPOSE:
	{
		if (!bDown)
		{
			// no one cares
		}
		else
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
				switch (_keyboardMode)
				{
					case KEYBOARD_SECTION:
					{
						HostEvent h = HostEventFactory::event(HC_SECTION_SET_NEXT,n);
						_host->event(h);

						// also set the led (to flash, until the section actually changes)
						_controlValue[(GrooveControlName)(GCL_SEMI_0 + n)] = GE_LED_FLASH;
						GrooveEvent ge = GrooveEvent(GE_LEDSET,(GrooveControlName)(GCL_SEMI_0 + n),GE_LED_FLASH);
						_interface->onGrooveEvent(ge);
						break;
					}
					case KEYBOARD_MUTE:
					{
						HostEvent h = HostEventFactory::event(HC_PHRASE_MUTE_TOGGLE,n);
						_host->event(h);

						printf("set %d led\n",n);

						// also toggle the led
						int led = _controlValue[(GrooveControlName)(GCL_SEMI_0 + n)];
						if (led == 0)
							led = 1;
						else if (led== 1)
							led =0;
						_controlValue[(GrooveControlName)(GCL_SEMI_0 + n)] = led;
						GrooveEvent ge = GrooveEvent(GE_LEDSET,(GrooveControlName)(GCL_SEMI_0 + n),led);
						_interface->onGrooveEvent(ge);
						break;
					}
					case KEYBOARD_TRACK:
					{
						GrooveEvent ge;

						// off old led
						ge = GrooveEvent(GE_LEDSET,
														(GrooveControlName)(GCL_SEMI_0 + _currentTrack),
														GE_LED_OFF);
						_interface->onGrooveEvent(ge);

						// set value
						_currentTrack = n;
						HostEvent h = HostEventFactory::event(HC_CURRENT_PHRASE,n);
						_host->event(h);

						// on new led
						_controlValue[(GrooveControlName)(GCL_SEMI_0 + _currentTrack)]=GE_LED_ON;
						ge = GrooveEvent(GE_LEDSET,
														(GrooveControlName)(GCL_SEMI_0 + _currentTrack),
														GE_LED_ON);
						_interface->onGrooveEvent(ge);
						break;
					}
					case KEYBOARD_TRANSPOSE:
					{
						// TODO: fix this!!!
						break;
					}
				}
			}

			break;
		}
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
		ge = GrooveEvent(GE_LEDSET,GCL_PLAYING,_controlValue[GCL_PLAYING]);
		_interface->onGrooveEvent(ge);
	}
	if (oldRecordLedState!=_controlValue[GCL_RECORDING])
	{
		ge = GrooveEvent(GE_LEDSET,GCL_RECORDING,_controlValue[GCL_RECORDING]);
		_interface->onGrooveEvent(ge);
	}
}

void Groovebox::onHostEvent(HostEvent& event)
{
	switch (event.name)
	{
		case HC_OUT_BEAT:
		{
			_controlValue[GCL_PLAYING] = GE_LED_PULSE;
			GrooveEvent ge = GrooveEvent(GE_LEDSET,GCL_PLAYING,GE_LED_PULSE);
			_interface->onGrooveEvent(ge);
			break;
		}
		case HC_OUT_SECTION_CHANGE:
		{
			if (_keyboardMode == KEYBOARD_SECTION)
			{
				// loop over keyboard leds to set them to represent the mute or section status
				Song* song = _host->getSequencer()->getSong();
				for (int i=0;i<16;i++)
				{
					int led = GE_LED_OFF;

					led = GE_LED_OFF;
					if (i==song->getCurrentSection())
					{
						led = GE_LED_ON;
					}
					else if (i==song->getNextSection())
					{
						led = GE_LED_FLASH;
					}
					else
					{
						led = GE_LED_OFF;
					}

					if (_controlValue[(GrooveControlName)(GCL_SEMI_0 + i)] != led)
					{
						_controlValue[(GrooveControlName)(GCL_SEMI_0 + i)]=led;
						GrooveEvent ge = GrooveEvent(GE_LEDSET,(GrooveControlName)(GCL_SEMI_0 + i),led);
						_interface->onGrooveEvent(ge);
					}
				}
			}
			break;
		}
		default:
			break;
	}
}

