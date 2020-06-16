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
#include "Gain.h"
#include "Passthrough.h"
#include "LooperDefs.h"
#include "Loop.h"

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

//   /**
//    * Control whether we're looking at left, right, or both inputs.
//    * @param channels Enum indicating what this input should be listening to.
//    */
//   void SetEnabledChannels(tk::ChannelEnable channels);
//
//   /**
//    * Get the currently enabled channel(s)
//    * @return Enum with the channel settings.
//    */
//   tk::ChannelEnable GetEnabledChannels() const;

    void ResetLoop(bool resetingAllTracks);
    
    void PlayFromBeginning();

    void StopTrack();

   /**
    * Connect a ChangeListener object to one of the things we own that can send change notifications.
    * @param  add     If true, add the `listener`, otherwise remove it.
    * @param target   One of the `ListenTo` enum items (kPreFx, kTrack, kPostFx)
    * @param listener Pointer to the object we need to send updates to.
    */
//   void UpdateChangeListeners(bool add, ListenTo target, ChangeListener* listener);

   LoopProcessor* GetLoopProcessor() const { return loop; };

private:
    JUCE_DECLARE_NON_COPYABLE(Track);

    Looper* looper;

    String name;

    bool muted = false;

    bool selected = false;

//    int loopDuration = 0;

    LoopProcessor* loop;

    CriticalSection mutex;
};
