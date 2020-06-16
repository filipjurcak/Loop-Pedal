/*
  ==============================================================================

    Loop.cpp
    Created: 12 Jun 2020 6:16:16pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#include "Loop.h"
#include <math.h>
#include "Track.h"


LoopProcessor::LoopProcessor(Track* track, int channelCount)
:  PassthroughProcessor(channelCount, channelCount)
,  track(track)
,  loopBuffer(new AudioBuffer<float>(channelCount, 0))
,  channelCount(channelCount)
{
    loopBuffer->clear();
//    this->addChangeListener(track);
}

LoopProcessor::~LoopProcessor()
{

}

void LoopProcessor::SetLoopDuration(int milliseconds)
{
    ScopedLock sl(mutex);
    loopDuration = milliseconds;
    int sampleCount = int(sampleRate * milliseconds / 1000.0);
    loopBuffer->setSize(inputChannelCount, sampleCount, true);
    this->sendChangeMessage();
}

int LoopProcessor::GetLoopDuration() const
{
   ScopedLock sl(mutex);
   return loopDuration;
}

int LoopProcessor::GetLoopPosition() const
{
    ScopedLock sl(mutex);
    return loopPosition;
}

float LoopProcessor::GetMagnitude() const
{
    ScopedLock sl(mutex);
    int sampleCount = int(sampleRate * loopDuration / 1000.0);
    return loopBuffer->getMagnitude(loopPosition, std::min(samplesPerBlock, std::max(0, sampleCount - loopPosition)));
}

//bool LoopProcessor::IsPlaying() const
//{
//   bool retval = false;
//   if (fTrack)
//   {
//      retval = fTrack->IsPlaying();
//   }
//   return retval;
//}



//float LoopProcessor::GetThumbnailPoint(int channel, int startSample, int endSample)
//{
//   float retval = 0.f;
//   jassert(endSample > startSample);
//   jassert(channel < fInputChannelCount);
//   if (fLoopBuffer && (channel < fInputChannelCount) && (endSample > startSample))
//   {
//      endSample = mMin(endSample, fLoopBuffer->getNumSamples());
//      int numSamples = endSample - startSample;
//      retval = fLoopBuffer->getMagnitude(channel, startSample, numSamples);
//   }
//   return retval;
//}


void LoopProcessor::Reset(bool resetBufferSize)
{
    ScopedLock sl(mutex);
    loopBuffer->clear();
    if (resetBufferSize)
    {
        loopBuffer->setSize(inputChannelCount, 0);
        loopDuration = 0;
        loopPosition = 0;
    }
    this->sendChangeMessage();
}

void LoopProcessor::PlayFromBeginning()
{
    ScopedLock sl(mutex);
    loopPosition = 0;
    this->sendChangeMessage();
}

void LoopProcessor::StopLoop()
{
    ScopedLock sl(mutex);
    loopPosition = 0;
    this->sendChangeMessage();
}

void LoopProcessor::GetLoopInfo(LoopInfo& info)
{
//   ScopedLock sl(fMutex);
//   info.fSampleRate = fSampleRate;
//   info.fLoopSample = fLoopPosition;
//   if (fLoopBuffer)
//   {
//      info.fLoopLength = fLoopBuffer->getNumSamples();
//   }
//   else
//   {
//      info.fLoopLength = 0;
//   }
//   info.fLoopCount = fLoopCount;
//   info.fIsPlaying = fTrack->IsPlaying();
//   info.fWasReset = fWasReset;
//   fWasReset = false;
}

const String LoopProcessor::getName() const
{
   return "LoopProcessor";
}

void LoopProcessor::prepareToPlay(double sampleRate, int estimatedSamplesPerBlock)
{
    PassthroughProcessor::prepareToPlay(sampleRate, estimatedSamplesPerBlock);
    sampleRate = sampleRate;
//    samplesPerBlock = estimatedSamplesPerBlock;
    // we may need to resize our internal buffers.
    this->SetLoopDuration(loopDuration);

}

void LoopProcessor::releaseResources()
{

}

void LoopProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // Lock down all of the protected code sections.
    ScopedLock sl(mutex);
    int sampleCount = buffer.getNumSamples();
    int loopSampleCount = loopBuffer->getNumSamples();
    LoopStates loopState = track->GetLoopState();
    if (loopState == LoopStates::Record and loopDuration == 0 and loopSampleCount < loopPosition + sampleCount) {
        loopBuffer->setSize(channelCount, loopPosition + sampleCount, true);
        loopSampleCount = loopBuffer->getNumSamples();
    }
    
    if (track->IsSelected() and (loopState != LoopStates::Play and loopState != LoopStates::PlayInRecord))
    {
        for (int channel = 0; channel < inputChannelCount; ++channel)
        {
            // this is easy if we don't need to wrap around the loop
            // buffer when processing this block
            if (loopPosition + sampleCount < loopSampleCount)
            {
                // Add samples from 1 loop ago
                buffer.addFrom(channel, 0, *loopBuffer, channel, loopPosition, sampleCount);
                // ... and copy the mixed samples back into the loop buffer
                // so we can play them back out in one loop's time.
                loopBuffer->copyFrom(channel, loopPosition, buffer, channel, 0, sampleCount);
            }
            else
            {
                // first, process as many samples as we can fit in at the
                // end of the loop buffer.
                int roomAtEnd = loopSampleCount - loopPosition;
                // and we need to put this many samples back at the
                // beginning.
                int wrapped = sampleCount - roomAtEnd;

                // add samples from a loop ago
                // part 1:
                buffer.addFrom(channel, 0, *loopBuffer, channel, loopPosition, roomAtEnd);
                // part 2:
                buffer.addFrom(channel, roomAtEnd, *loopBuffer, channel, 0, wrapped);

                // and now copy the mixed samples back into the loop buffer:
                // part 1:
                loopBuffer->copyFrom(channel, loopPosition, buffer, channel, 0, roomAtEnd);
                // part 2:
                loopBuffer->copyFrom(channel, 0, buffer, channel, roomAtEnd, wrapped);
            }
        }
    } else if (loopDuration != 0) {
        for (int channel = 0; channel < inputChannelCount; ++channel)
        {
            if (loopPosition + sampleCount < loopSampleCount)
            {
                buffer.copyFrom(channel, 0, *loopBuffer, channel, loopPosition, sampleCount);
            }
            else
            {
                int roomAtEnd = loopSampleCount - loopPosition;
                int wrapped = sampleCount - roomAtEnd;
                buffer.copyFrom(channel, 0, *loopBuffer, channel, loopPosition, roomAtEnd);
                buffer.copyFrom(channel, roomAtEnd, *loopBuffer, channel, 0, wrapped);
            }
        }
    }
    
    if (track && (track->IsMuted() || track->IsGlobalMuteOn()))
    {
        buffer.applyGain(0, buffer.getNumSamples(), 0.0f);
    }

    // set the loop position for the next block of data.
    loopPosition += sampleCount;
    if (loopPosition >= loopSampleCount)
    {
        loopPosition -= loopSampleCount;
    }
    // Notify anyone who's observing this processor that we've gotten new sample data.
    this->sendChangeMessage();
}
