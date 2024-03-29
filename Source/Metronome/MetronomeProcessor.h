/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __METRONOMEPROCESSOR_H_526ED7A9__
#define __METRONOMEPROCESSOR_H_526ED7A9__

#include "../../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class MetronomeProcessor  : public AudioProcessor
{
public:
    //==============================================================================
	MetronomeProcessor();
    ~MetronomeProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    void reset();

    //==============================================================================
    bool hasEditor() const                  { return true; }
    void* createEditor();

    //==============================================================================
    const String getName() const            { return "Metronome"; }

    int getNumParameters();
    float getParameter (int index);
    void setParameter (int index, float newValue);
    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms()                                                { return 0; }
    int getCurrentProgram()                                             { return 0; }
    void setCurrentProgram (int /*index*/)                              { }
    const String getProgramName (int /*index*/)                         { return String::empty; }
    void changeProgramName (int /*index*/, const String& /*newName*/)   { }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);

    //==============================================================================
    enum Parameters
    {
        volumeParam = 0,

        totalNumParams
    };

    float currentVolume;

private:

    // the synth!
    Synthesiser synth;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MetronomeProcessor)
};

#endif  // __METRONOMEPROCESSOR_H_526ED7A9__
