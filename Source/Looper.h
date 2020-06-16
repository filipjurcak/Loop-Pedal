/*
  ==============================================================================

    Looper.h
    Created: 12 Jun 2020 4:09:42pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Muter.h"
#include "LooperDefs.h"

using Node = AudioProcessorGraph::Node;

class Track;

class Looper: public MidiInputCallback, public ChangeBroadcaster
{
public:
    friend class Track;

    Looper(AudioDeviceManager& deviceManager, int numOfTracks = 4);

    ~Looper();
    
    int GetNumberOfTracks();

    LoopStates GetMode() const;
    
    bool IsGlobalMuteOn() const;
    
    RecordState GetRecordState() const;

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
    
    Node::Ptr muterNode = nullptr;

    OwnedArray<Track> tracks;

    Array<String> trackNames = { "GUITAR", "DRUM", "RC30", "VOX" };

    LoopStates mode = LoopStates::PlayInRecord;

    RecordState recordState = RecordState::NotRecorded;
    
    bool globalMute = false;

    double startLoopingTime = -1.0f;

    double loopTime = -1.0f;

    int selectedTrack = 0;

    int lastMidiInputIndex = 0;

    CriticalSection mutex;
};
