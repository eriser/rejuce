/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "Host/Host.h"

//==============================================================================

//#include <Windows.h>
#include <iostream>


//==============================================================================
int main (int argc, char* argv[])
{
	Host* host = new Host();

	if (!host->init("ALSA","Intel 82801AA-ICH","VMPK Output",44100))
	{
		DBG("could not initialize host.");
	}
	else
	{
		// wait
		std::cout <<"q to quit..\n";
		char s[10];s[0]='\0';
		while (s[0]!='q')
		{
			s[0]='\0';
			std::cin >> s;

			if (s[0]=='p')//play
			{
				printf("play...\n");
				host->event(HostEventFactory::event(HC_TRANSPORT_PLAY));
			}

			if (s[0]=='r')//record
			{
				printf("record...\n");
				host->event(HostEventFactory::event(HC_TRANSPORT_RECORD));
			}

			if (s[0]=='s')//stop
			{
				printf("stop...\n");
				host->event(HostEventFactory::event(HC_TRANSPORT_STOP));
			}
		}
	}

	delete host;

//	CoUninitialize();
    return 0;
}
