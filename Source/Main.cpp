/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================


#include <Windows.h>

//==============================================================================
int main (int argc, char* argv[])
{
	CoInitialize(NULL);

	// setup audio
	AudioDeviceManager adm;	
	for (int n=0;n<adm.getAvailableDeviceTypes().size();n++)
	{
		AudioIODeviceType* dt = adm.getAvailableDeviceTypes().getUnchecked(n);
		
		DBG("audio IO device type "<<dt->getTypeName());
	}




	CoUninitialize();
    return 0;
}
