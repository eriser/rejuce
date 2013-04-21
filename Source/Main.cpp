/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "Sequencer/Host.h"

//==============================================================================

//#include <Windows.h>
#include <iostream>


//==============================================================================
int main (int argc, char* argv[])
{
	Host host;

	if (!host.init("ALSA","Intel 82801AA-ICH","VMPK Output",44100))
	{
		DBG("could not initialize host.");
		exit(1);
	}
	else
	{
		host.start();
	}

	// wait
	std::cout <<"q to quit..\n";
	char s[10];s[0]='\0';
	while (s[0]!='q')
	{
		std::cin >> s;

		if (s[0]=='p')//play
		{
			printf("play...\n");
			host.command(HostCommandFactory::command(HC_TRANSPORT_PLAY));
		}

		if (s[0]=='r')//record
		{
			printf("record...\n");
			host.command(HostCommandFactory::command(HC_TRANSPORT_RECORD));
		}

		if (s[0]=='s')//stop
		{
			printf("stop...\n");
			host.command(HostCommandFactory::command(HC_TRANSPORT_STOP));
		}

	}

	host.stop();

//	CoUninitialize();
    return 0;
}
