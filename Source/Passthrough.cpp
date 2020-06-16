/*
  ============================================================================
    Passthrough.cpp
    Created: 12 Jun 2020 7:09:53pm
    Author:  Filip Jurcak
  ===========================================================================
*/

#include "Passthrough.h"

PassthroughProcessor::PassthroughProcessor(int inputChannelCount, int outputChannelCount)
:  inputChannelCount(inputChannelCount)
,  outputChannelCount(outputChannelCount)
{
   jassert(inputChannelCount >= 1);
   jassert(inputChannelCount <= 2);
   jassert(outputChannelCount >= 0);
   jassert(outputChannelCount <= 2);

   if (outputChannelCount == 0)
   {
      outputChannelCount = inputChannelCount;
   }

   this->setLatencySamples(0);
   this->setPlayConfigDetails(inputChannelCount, outputChannelCount, 0, 0);
}

PassthroughProcessor::~PassthroughProcessor()
{
   
}

const String PassthroughProcessor::getName() const
{
   return "PassthroughProcessor";
}

void PassthroughProcessor::prepareToPlay(double sampleRate, int estimatedSamplesPerBlock)
{
   this->setPlayConfigDetails(inputChannelCount, outputChannelCount, sampleRate,
      estimatedSamplesPerBlock);

}

void PassthroughProcessor::releaseResources()
{

}

void PassthroughProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

}

const String PassthroughProcessor::getInputChannelName(int channelIndex) const
{
   return "channel " + String(channelIndex);
}

const String PassthroughProcessor::getOutputChannelName(int channelIndex) const
{
   return "channel " + String(channelIndex);
}

bool PassthroughProcessor::isInputChannelStereoPair(int index) const
{
   return (2 == inputChannelCount);
}

bool PassthroughProcessor::isOutputChannelStereoPair(int index) const
{
   return (2 == outputChannelCount);
}

bool PassthroughProcessor::silenceInProducesSilenceOut() const
{
   return true;
}

bool PassthroughProcessor::acceptsMidi() const
{
   return false;
}

bool PassthroughProcessor::producesMidi() const
{
   return false;
}

AudioProcessorEditor* PassthroughProcessor::createEditor()
{
   return nullptr;
}

bool PassthroughProcessor::hasEditor() const
{
   return false;
}

int PassthroughProcessor::getNumParameters()
{
   return 0;
}

const String PassthroughProcessor::getParameterName(int parameterIndex)
{
   return "parameter " + String(parameterIndex);
}

float PassthroughProcessor::getParameter(int parameterIndex)
{
   return 0.f;
}

const String PassthroughProcessor::getParameterText(int parameterIndex)
{
   return "0.0";
}

void PassthroughProcessor::setParameter(int parameterIndex, float newValue)
{

}


int PassthroughProcessor::getNumPrograms()
{
   return 0;
}

int PassthroughProcessor::getCurrentProgram()
{
   return 0;
}


void PassthroughProcessor::setCurrentProgram(int index)
{

}

const String PassthroughProcessor::getProgramName(int index)
{
   return "program #" + String(index);
}

void PassthroughProcessor::changeProgramName(int index, const String& newName)
{

}

void PassthroughProcessor::getStateInformation(juce::MemoryBlock& destData)
{

}

void PassthroughProcessor::setStateInformation(const void* data, int sizeInBytes)
{

}

bool PassthroughProcessor::supportDoublePrecisionProcessing() const
{
    return false;
}

double PassthroughProcessor::getTailLengthSeconds() const
{
    return 0;
}
