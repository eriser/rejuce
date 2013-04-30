

#include "HostProcessor.h"


HostProcessor* JUCE_CALLTYPE createHostProcessor();


//==============================================================================
HostProcessor::HostProcessor()
{
}

HostProcessor::~HostProcessor()
{
	for (int i=0;i<_synths.size();i++)
	{
		delete _synths[i];
	}

}

//==============================================================================
int HostProcessor::getNumParameters()
{
    return totalNumParams;
}

float HostProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:     return gain;
        case delayParam:    return delay;
        default:            return 0.0f;
    }
}

void HostProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:     gain = newValue;  break;
        case delayParam:    delay = newValue;  break;
        default:            break;
    }
}

const String HostProcessor::getParameterName (int index)
{
    switch (index)
    {
        case gainParam:     return "gain";
        case delayParam:    return "delay";
        default:            break;
    }

    return String::empty;
}

const String HostProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

//==============================================================================
void HostProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	for (int channel=0;channel<_synths.size();channel++)
	{
		_synths.getReference(channel)->prepareToPlay(sampleRate,samplesPerBlock);
	}

}

void HostProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

	for (int channel=0;channel<_synths.size();channel++)
	{
		_synths[channel]->releaseResources();
	}

}

void HostProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.

	for (int channel=0;channel<_synths.size();channel++)
	{
		_synths[channel]->reset();
	}

}

void HostProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	for (int channel=0;channel<_synths.size();channel++)
	{
		// copy only messages for this channel
		MidiBuffer channelBuffer;
		MidiBuffer::Iterator iter(midiMessages);
		MidiMessage message;
		int pos;
		while (iter.getNextEvent(message,pos))
		{
			if (message.getChannel() == channel+1)
			{
				channelBuffer.addEvent(message,pos);
			}
		}

		_synths[channel]->processBlock(buffer,channelBuffer);
	}

	midiMessages.clear();

}

//==============================================================================
void* HostProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void HostProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:

    // Create an outer XML element..
    /*XmlElement xml ("MYPLUGINSETTINGS");

    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    xml.setAttribute ("gain", gain);
    xml.setAttribute ("delay", delay);

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);*/
}

void HostProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    /*ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);

            gain  = (float) xmlState->getDoubleAttribute ("gain", gain);
            delay = (float) xmlState->getDoubleAttribute ("delay", delay);
        }
    }
    */
}

const String HostProcessor::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String HostProcessor::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool HostProcessor::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool HostProcessor::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool HostProcessor::acceptsMidi() const
{
	return true;
}

bool HostProcessor::producesMidi() const
{
	return false;
}

bool HostProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double HostProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

void HostProcessor::hostAddSynth(AudioProcessor* synth)
{
	_synths.add(synth);
}

const AudioProcessor* HostProcessor::hostGetSynth(int i)
{
	if (i>=0 && i<_synths.size())
	{
		return _synths.getReference(i);
	}
}

//==============================================================================
// This creates new instances of the plugin..
HostProcessor* JUCE_CALLTYPE createHostProcessor()
{
    return new HostProcessor();
}
