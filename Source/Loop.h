/*
  ==============================================================================

    Loop.h
    Created: 12 Jun 2020 6:16:16pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#pragma once

#include "Passthrough.h"

class Track;
class Looper;

class LoopProcessor : public PassthroughProcessor
                    , public ChangeBroadcaster
{
public:
    LoopProcessor(Looper* looper, Track* track, int channelCount = 2);

    ~LoopProcessor();

    void SetLoopDuration(int milliseconds);

    int GetLoopDuration() const;
    
    int GetLoopPosition() const;
    
    float GetMagnitude() const;

    float GetThumbnailPoint(int channel, int startSample, int endSample);

    void Reset(bool resetBufferSize);
    
    void PlayFromBeginning();

    void StopLoop();
    
    AudioBuffer<float>* GetLoopBuffer() const { return loopBuffer.get(); };
    
    double GetSampleRate() const { return sampleRate; };

    const String getName() const override;

    void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock) override;

    void releaseResources() override;

    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

private:
    Looper* looper;
    
    Track* track;

    double sampleRate = 44100.0;
    
    int samplesPerBlock = 512;

    int loopDuration = 0;

    std::unique_ptr<AudioBuffer<float>> loopBuffer;

    int loopPosition = 0;
    
    int channelCount;

    CriticalSection mutex;
};
