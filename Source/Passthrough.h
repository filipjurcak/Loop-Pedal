/*
  =============================================================================
 
    Passthrough
    Created: 12 Jun 2020 7:09:50
    Author:  Filip Jurca
 
  ============================================================================
*/


#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LooperDefs.h"

/**
 * @class PassthroughProcessor
 *
 * No-op audio processor that we're really only using for test. You can create an
 * instance with 1 or 2 input/output channels. Whatever comes in goes out unaffected.
 */
class PassthroughProcessor : public AudioProcessor
{
public:
    /**
     *  @param inputChannelCount number of desired input channels
     *  @param outputChannelCount number of desired output channels (if ==0, will match
     *                            the setting of the input channels.)
    */
    PassthroughProcessor(int inputChannelCount = 1, int outputChannelCount=0);

    /**
    * Destructor.
    */
    ~PassthroughProcessor();
   
    /**
     * get the name of this processor.
     * @return String filled with the name.
     */
    const String getName() const;

    int GetInputChannelCount() const { return inputChannelCount; };
   
    int GetOutputChannelCount() const { return outputChannelCount; };
 
    /**
     * Called before playback starts to get things ready.
     * @param sampleRate               samples per second.
     * @param estimatedSamplesPerBlock
     */
    void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock);

    void releaseResources();

    void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

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


protected:
    int inputChannelCount;
    int outputChannelCount;

private:
   JUCE_DECLARE_NON_COPYABLE(PassthroughProcessor);
};
