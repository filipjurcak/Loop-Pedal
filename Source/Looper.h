/*
  ==============================================================================

    Looper.h
    Created: 12 Jun 2020 4:09:42pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LooperDefs.h"

using Node = AudioProcessorGraph::Node;

//class GainProcessor;
//class SampleCounterProcessor;
class Track;


/**
 * Convert a decibel value into the equivalent floating-point gain.
 * @param  db decibels, probably <= 0 (but not necessarily  )
 * @return    the corresponding floating-point amplitude gain value (e.g., -6.0 ~= 0.5)
 */
float DbToFloat(float db);

/**
 * convert a gain value from 0.0 .. 1.0 into the equivalent dB value
 * @param  gain Gain to convert
 * @return      decibel value
 */
float GainToDb(float gain);


class Looper: public MidiInputCallback, public ChangeBroadcaster //, public ChangeListener
{
public:
    friend class Track;

    Looper(AudioDeviceManager& deviceManager, int numOfTracks = 4);

    ~Looper();

//   /**
//    * Called when something needs to notify us of a change. Initially,
//    * this is only used when Track objects need to tell us that they
//    * want to be deleted.
//    * @param source object that's notifying us.
//    */
//   void changeListenerCallback(ChangeBroadcaster* source);
    
    int GetNumberOfTracks();

    LoopStates GetMode() const;
    
    bool IsGlobalMuteOn() const;

    Track* GetSelectedTrack() const;

    void Connect(Node::Ptr source, Node::Ptr dest);

    void Disconnect(Node::Ptr source, Node::Ptr dest);

    void InsertBetween(Node::Ptr before, Node::Ptr newNode, Node::Ptr after);

    Node::Ptr GetInputNode() const;

    Node::Ptr GetOutputNode() const;

    Node::Ptr AddProcessor(std::unique_ptr<AudioProcessor> processor);

    void ResetAllTracks();
    
    void PlayAllTracks();

    void StopAllTracks();

    Track* GetTrack(int index) const;

protected:
    void ChangeMode(LoopStates mode);

    void ChangeSelectedTrack(int index);

    void HandleConnection(Node::Ptr source, Node::Ptr dest, bool connecting);

private:
    JUCE_DECLARE_NON_COPYABLE(Looper);
    
    void setMidiInput (int index);

    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message);

    AudioProcessorPlayer player;

    std::unique_ptr<AudioProcessorGraph> graph;

    AudioDeviceManager& deviceManager;

    Node::Ptr inputNode = nullptr;

    Node::Ptr outputNode = nullptr;

    OwnedArray<Track> tracks;

    Array<String> trackNames = { "GUITAR", "DRUM", "RC30", "VOX" };

    LoopStates mode = LoopStates::PlayInRecord;

    bool wasRecorded = false;
    
    bool globalMute = false;

    double startLoopingTime;

    double loopTime;

    int selectedTrack = 0;

    int lastMidiInputIndex = 0;

    CriticalSection mutex;
};
