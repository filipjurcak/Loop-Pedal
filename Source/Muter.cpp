/*
  ==============================================================================

    Muter.cpp
    Created: 16 Jun 2020 2:57:08am
    Author:  Filip Jurcak

  ==============================================================================
*/

#include "Muter.h"
#include "Track.h"

MuterProcessor::MuterProcessor(int channelCount): PassthroughProcessor(channelCount, channelCount)
{

}

MuterProcessor::~MuterProcessor() {}

void MuterProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    buffer.applyGain(0, buffer.getNumSamples(), 0.0f);
}
