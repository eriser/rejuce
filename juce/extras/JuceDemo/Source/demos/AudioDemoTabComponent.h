/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  21 Sep 2012 12:10:28pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_AUDIODEMOTABCOMPONENT_AUDIODEMOTABCOMPONENT_3A306AC8__
#define __JUCER_HEADER_AUDIODEMOTABCOMPONENT_AUDIODEMOTABCOMPONENT_3A306AC8__

//[Headers]     -- You can add your own extra header files here --
#include "../jucedemo_headers.h"


//==============================================================================
/* This component scrolls a continuous waveform showing the audio that's currently
   coming into the audio input.
*/
class LiveAudioInputDisplayComp  : public Component,
                                   public AudioIODeviceCallback,
                                   public Timer
{
public:
    //==============================================================================
    LiveAudioInputDisplayComp();
    ~LiveAudioInputDisplayComp();

    void paint (Graphics& g);
    void timerCallback();

    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels, int numSamples);
private:
    float samples [1024];
    int nextSample, subSample;
    float accumulator;

    LiveAudioInputDisplayComp (const LiveAudioInputDisplayComp&);
    LiveAudioInputDisplayComp& operator= (const LiveAudioInputDisplayComp&);
};

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    This component creates the set of tabs that hold the various
    audio demo pages..
                                                                    //[/Comments]
*/
class AudioDemoTabComponent  : public Component
{
public:
    //==============================================================================
    AudioDemoTabComponent ();
    ~AudioDemoTabComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AudioDeviceManager deviceManager;
    //[/UserVariables]

    //==============================================================================
    TabbedComponent* tabbedComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioDemoTabComponent)
};


#endif   // __JUCER_HEADER_AUDIODEMOTABCOMPONENT_AUDIODEMOTABCOMPONENT_3A306AC8__
