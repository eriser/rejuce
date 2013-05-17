/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "Host/Host.h"
#include "Groovebox/Groovebox.h"
#include "WebInterface/WebInterface.h"

//==============================================================================

//#include <Windows.h>
#include <iostream>


//==============================================================================
int main (int argc, char* argv[])
{
	Host host;
	WebInterface interface;
	Groovebox groovebox(&host,&interface);
	interface.setOutListener(&groovebox);

	if (!host.init("ALSA","Intel 82801AA-ICH","VMPK Output",44100))
	//if (!host->init("ALSA","HDA Intel (1)","VMPK Output",44100))
	{
		DBG("could not initialize host.");
	}
	else
	{
		std::cout <<"start Groovebox\n";
		groovebox.start();

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
				host.event(HostEventFactory::event(HC_TRANSPORT_PLAY));
			}

			if (s[0]=='r')//record
			{
				printf("record...\n");
				host.event(HostEventFactory::event(HC_TRANSPORT_RECORD));
			}

			if (s[0]=='s')//stop
			{
				printf("stop...\n");
				host.event(HostEventFactory::event(HC_TRANSPORT_STOP));
			}
		}

		std::cout <<"stop Groovebox\n";

		groovebox.stop();

	}

//	CoUninitialize();

	DBG("exiting OK\n");
    return 0;
}
