/*
  ==============================================================================

    Gain.cpp
    Created: 12 Jun 2020 4:18:30pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#include "Gain.h"
#include "Track.h"

GainProcessor::GainProcessor(Track* track, int channelCount, float gain)
:  PassthroughProcessor(channelCount, channelCount)
,  track(track)
,  gain(gain)
{

}

GainProcessor::~GainProcessor() {}

void GainProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    float gain = this->GetGain();
//    std::cout << "Gain na tomto node je " << gain << "\n";
    if (track && (track->IsMuted() || track->IsGlobalMuteOn()))
    {
        gain = 0.0f;
    }
    buffer.applyGain(0, buffer.getNumSamples(), gain);
}

void GainProcessor::SetGain(float gain)
{
    const ScopedLock sl(mutex);
    this->gain = gain;
    this->sendChangeMessage();
}

float GainProcessor::GetGain() const
{
    const ScopedLock sl(mutex);
    return gain;
}
