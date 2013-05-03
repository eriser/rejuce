

#include "../JuceLibraryCode/JuceHeader.h"
#include "GenericSynth.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...

//==============================================================================
/** Our demo synth sound is just a basic sine wave..
*/
class SineWaveSound : public SynthesiserSound
{
public:
    SineWaveSound()
    {
    }

    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};


//==============================================================================
/** Our demo synth voice just plays a sine wave..
*/
class SineWaveVoice  : public SynthesiserVoice
{
public:
    SineWaveVoice()
        : angleDelta (0.0),
          tailOff (0.0)
    {
    }

    bool canPlaySound (SynthesiserSound* sound)
    {
        return dynamic_cast <SineWaveSound*> (sound) != 0;
    }

    void startNote (const int midiNoteNumber, const float velocity,
                    SynthesiserSound* /*sound*/, const int /*currentPitchWheelPosition*/)
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;

        double cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        double cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * double_Pi;
    }

    void stopNote (const bool allowTailOff)
    {
        if (allowTailOff)
        {
            // start a tail-off by setting this flag. The render callback will pick up on
            // this and do a fade out, calling clearCurrentNote() when it's finished.

            if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
                                // stopNote method could be called more than once.
                tailOff = 1.0;
        }
        else
        {
            // we're being told to stop playing immediately, so reset everything..

            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    void pitchWheelMoved (const int /*newValue*/)
    {
        // can't be bothered implementing this for the demo!
    }

    void controllerMoved (const int /*controllerNumber*/, const int /*newValue*/)
    {
        // not interested in controllers in this case.
    }

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0)
            {
                while (--numSamples >= 0)
                {
                    const float currentSample = (float) (sin (currentAngle) * level * tailOff);

                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;

                    currentAngle += angleDelta;
                    ++startSample;

                    tailOff *= 0.99;

                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();

                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    const float currentSample = (float) (sin (currentAngle) * level);

                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;

                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    }

private:
    double currentAngle, angleDelta, level, tailOff;
};


///////////////////////////////////////////////////////////////////////////////////////////////////

//==============================================================================

//==============================================================================
GenericSynth::GenericSynth()
{
    gain = 1.0f;
    zeromem (&lastPosInfo, sizeof (lastPosInfo));
    lastPosInfo.timeSigNumerator = 4;
    lastPosInfo.timeSigDenominator = 4;
    lastPosInfo.bpm = 120;

	// add some voices to our synth, to play the sounds..
    for (int i = 4; --i >= 0;)
    {
        synth.addVoice (new SineWaveVoice()); 
    }
    synth.clearSounds();
    synth.addSound (new SineWaveSound());
}

GenericSynth::~GenericSynth()
{
}

//========================================================================================================================
const String GenericSynth::getName() const
{
    return "Metronome Synth";
}

int GenericSynth::getNumParameters()
{
    return 1;
}

float GenericSynth::getParameter (int index)
{
    return (index == 0) ? gain
                        : 0.0f;
}

void GenericSynth::setParameter (int index, float newValue)
{
    if (index == 0)
    {
        if (gain != newValue)
        {
            gain = newValue;
        }
    }
}

const String GenericSynth::getParameterName (int index)
{
    if (index == 0)
        return "gain";

    return String::empty;
}

const String GenericSynth::getParameterText (int index)
{
    if (index == 0)
        return String (gain, 2);

    return String::empty;
}

const String GenericSynth::getInputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

const String GenericSynth::getOutputChannelName (const int channelIndex) const
{
    return String (channelIndex + 1);
}

bool GenericSynth::isInputChannelStereoPair (int index) const
{
    return false;
}

bool GenericSynth::isOutputChannelStereoPair (int index) const
{
    return false;
}

bool GenericSynth::acceptsMidi() const
{
    return true;
}

bool GenericSynth::producesMidi() const
{
    return false;
}

//==============================================================================
void GenericSynth::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
}

void GenericSynth::releaseResources()
{
    // when playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void GenericSynth::processBlock (AudioSampleBuffer& buffer,
                                   MidiBuffer& midiMessages)
{
    buffer.clear();

	if (midiMessages.getNumEvents())
	{
		DBG("events "<<midiMessages.getNumEvents());
	}

    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
void* GenericSynth::createEditor()
{
    return nullptr;
}

//==============================================================================
void GenericSynth::getStateInformation (MemoryBlock& destData)
{
    
}

void GenericSynth::setStateInformation (const void* data, int sizeInBytes)
{
  
}
