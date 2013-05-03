

#include "HostProcessor.h"


HostProcessor* JUCE_CALLTYPE createHostProcessor();


//==============================================================================
HostProcessor::HostProcessor()
{
	_metronomeBuffer = nullptr;
	for (int i=0;i<16;i++)
	{
		_synthBuffers[i] = nullptr;
	}

	_metronome = new MetronomeProcessor();
}

HostProcessor::~HostProcessor()
{
	for (int i=0;i<_synths.size();i++)
	{
		delete _synths[i];
	}

	delete _metronome;


	for (int i=0;i<16;i++)
	{
		if (_synthBuffers[i])
		{
			delete _synthBuffers[i];
		}
	}

	if (_metronomeBuffer)
		delete _metronomeBuffer;
}

//==============================================================================
int HostProcessor::getNumParameters()
{
    return 0;
}

float HostProcessor::getParameter (int index)
{
	return 0.0f;
}

void HostProcessor::setParameter (int index, float newValue)
{

}

const String HostProcessor::getParameterName (int index)
{
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
		_synths[channel]->prepareToPlay(sampleRate,samplesPerBlock);
	}

	for (int i=0;i<16;i++)
	{
		if (_synthBuffers[i])
		{
			delete _synthBuffers[i];
		}
		_synthBuffers[i] = new AudioSampleBuffer(getNumOutputChannels(),samplesPerBlock);
	}


	if (_metronomeBuffer)
		delete _metronomeBuffer;
	_metronomeBuffer = new AudioSampleBuffer(getNumOutputChannels(),samplesPerBlock);

	_metronome->setPlayConfigDetails (0, 2,getSampleRate(), getBlockSize());
	_metronome->prepareToPlay(getSampleRate(),getBlockSize());
}

void HostProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

	for (int channel=0;channel<_synths.size();channel++)
	{
		_synths[channel]->releaseResources();
	}

	_metronome->releaseResources();
}

void HostProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.

	for (int channel=0;channel<_synths.size();channel++)
	{
		_synths[channel]->reset();
	}

	_metronome->reset();

}

void HostProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	for (int i=0;i<16;i++)
	{
		_synthBuffers[i]->clear();
	}

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
				printf("c %d %d %d at %d\n",message.getRawData()[0],message.getRawData()[1],message.getRawData()[2],pos);
				channelBuffer.addEvent(message,pos);
			}
		}

		_synths[channel]->processBlock(*(_synthBuffers[channel]),channelBuffer);
	}

	// convert special metronome bip and bop (0xf2 00 and 0xf2 01) into note on/off messages
	{
		MidiBuffer metronomeMidiBuffer;
		MidiBuffer::Iterator iter(midiMessages);
		MidiMessage message;
		int pos;
		while (iter.getNextEvent(message,pos))
		{
			if (message.getRawDataSize()==2 && message.isSongPositionPointer())
			{
				char raw[4] = {0,0,0,0};

				char* data = (char*)message.getRawData();
				if (data[1]==0)
				{
					// bip
					MidiMessage b = MidiMessage::noteOn(1,80,64.0f);
					memcpy(raw,b.getRawData(),4);
				}
				else
				{
					// bop
					MidiMessage b = MidiMessage::noteOn(1,60,64.0f);
					memcpy(raw,b.getRawData(),4);
				}

				if (raw[0])
				{
					MidiMessage m(raw[0],raw[1],raw[2]);
					printf("m %d %d %d at %d\n",m.getRawData()[0],m.getRawData()[1],m.getRawData()[2],pos);

					// TODO: make it so we dont need a noteoff, to remove hack below
					// metronome is one audio sample ahead so we can easily do a noteoff
					metronomeMidiBuffer.addEvent(m,pos==0?1:pos);


//					MidiMessage off;
//					off.allNotesOff(1);
//					metronomeMidiBuffer.addEvent(off,pos);
				}
			}
		}

		_metronomeBuffer->clear();
		_metronome->processBlock(*_metronomeBuffer,metronomeMidiBuffer);
	}

	// mix
	buffer.clear();
	for (int i=0;i<16;i++)
	{
		buffer.addFrom(0,0,_synthBuffers[i]->getSampleData(0),buffer.getNumSamples(),1.0f);
		buffer.addFrom(1,0,_synthBuffers[i]->getSampleData(1),buffer.getNumSamples(),1.0f);
	}
	buffer.addFrom(0,0,_metronomeBuffer->getSampleData(0),buffer.getNumSamples(),1.0f);
	buffer.addFrom(1,0,_metronomeBuffer->getSampleData(1),buffer.getNumSamples(),1.0f);


	// clear all messages before we return
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

	synth->setPlayConfigDetails (0, 2,getSampleRate(), getBlockSize());
	synth->prepareToPlay(getSampleRate(),getBlockSize());

	_synths.add(synth);
}

const AudioProcessor* HostProcessor::hostGetSynth(int i)
{
	if (i>=0 && i<_synths.size())
	{
		return _synths.getReference(i);
	}

	return nullptr;
}

//==============================================================================
// This creates new instances of the plugin..
HostProcessor* JUCE_CALLTYPE createHostProcessor()
{
    return new HostProcessor();
}
