/*
  ============================================================================
 
    Passthrough.h
    Created: 12 Jun 2020 7:09:53pm
    Author:  Filip Jurcak
 
  ===========================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LooperDefs.h"

class PassthroughProcessor : public AudioProcessor
{
public:
    PassthroughProcessor(int inputChannelCount = 1, int outputChannelCount=0);

    ~PassthroughProcessor();

    const String getName() const;

    int GetInputChannelCount() const { return inputChannelCount; }

    int GetOutputChannelCount() const { return outputChannelCount; }

    void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock);

    void releaseResources();

    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages);

    const String getInputChannelName(int channelIndex) const;

    const String getOutputChannelName(int channelIndex) const;

    bool isInputChannelStereoPair(int index) const;

    bool isOutputChannelStereoPair(int index) const;

    bool silenceInProducesSilenceOut() const;

    bool acceptsMidi() const;

    bool producesMidi() const;

    AudioProcessorEditor* createEditor();

    bool hasEditor() const;

    int getNumParameters();

    const String getParameterName(int parameterIndex);

    float getParameter(int parameterIndex);

    const String getParameterText(int parameterIndex);

    void setParameter(int parameterIndex, float newValue);

    int getNumPrograms();

    int getCurrentProgram();

    void setCurrentProgram(int index);

    const String getProgramName(int index);

    void changeProgramName(int index, const String& newName);

    void getStateInformation(juce::MemoryBlock& destData);

    void setStateInformation(const void* data, int sizeInBytes);
    
    bool supportDoublePrecisionProcessing() const;
    
    double getTailLengthSeconds() const;

protected:
    int inputChannelCount;

    int outputChannelCount;

private:
    JUCE_DECLARE_NON_COPYABLE(PassthroughProcessor);
};
