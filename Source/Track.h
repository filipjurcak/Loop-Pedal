/*
  ==============================================================================

    Track.h
    Created: 12 Jun 2020 4:12:38pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Looper.h"
#include "Passthrough.h"
#include "LooperDefs.h"
#include "Loop.h"
#include "Muter.h"

using Node = AudioProcessorGraph::Node;

class Track : public ChangeBroadcaster, public ChangeListener
{
public:
    Track(Looper* looper, const String& name = String(), bool selected = false);

    ~Track();
    
    void changeListenerCallback(ChangeBroadcaster* source) override;

    String GetName() const;

    void Mute(bool mute);

    bool IsMuted() const;
    
    bool IsGlobalMuteOn() const;
    
    bool IsSelected() const;
    
    void SelectTrack();
    
    void UnselectTrack();
    
    void SetLoopDuration (int duration);
    
    LoopStates GetLoopState();

    void ResetLoop(bool resetingAllTracks);
    
    void PlayFromBeginning();

    void StopTrack();

    LoopProcessor* GetLoopProcessor() const { return loop; };
    
    void setMuterBypass(bool shouldBypass);

private:
    JUCE_DECLARE_NON_COPYABLE(Track);

    Looper* looper;

    String name;

    bool muted = false;

    bool selected = false;

    LoopProcessor* loop;
    
    Node::Ptr muterNode = nullptr;

    CriticalSection mutex;
};
