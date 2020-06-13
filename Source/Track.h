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

//#include "Gain.h"
//#include "Input.h"
//#include "Passthrough.h"
//#include "LooperDefs.h"
//class Looper;


class Track : public ChangeBroadcaster
{
public:
   Track(Looper* looper, const String& name = String(), bool selected = false);

   ~Track();

    /**
     * Return the name of this track
     * @return string, track name.
    */
    String GetName() const;

    /**
     * Mute/unmute this track.
     * @return       Success/fail.
    */
    void MuteUnmute();

    /**
     * Is this track currently muted?
     * @return bool, current mute state.
     */
    bool IsMuted() const;
    
    bool IsSelected() const;
    
    void SelectTrack();
    
    void UnselectTrack();
    
    void SetLoopDuration (float duration);

   /**
    * Set the input gain to be applied to this track before going into the pre-effects
    * block. default = 0.0.
    * @param gainInDb gain in dB, probably -96..+6 (?)
    */
//   void SetInputGain(float gainInDb);

   /**
    * Return the current gain in dB applied to the input of this track
    * @return Gain value.
    */
//   float GetInputGain() const;
//
//   /**
//    * Set the pan value to be applied to the input to this track.
//    * NOTE that this only has an effect when the input node is a single channel and
//    * the InputProcessor is set to generate a stereo pair.
//    * @param  pan 0.0 = fully left, 1.0 == fully right. Default = 0.5
//    * @return     success/fail.
//    */
//   LooperResult SetInputPan(float pan);
//
//   /**
//    * Get the pan setting applied to the input of this channel (0 <= pan <= 1)
//    * @return float.
//    */
//   float GetInputPan() const;




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


   /**
    * Reset the contents of the loop. Zero out all samples & return the loop
    * read/write position to zero.
    */
   void ResetLoop();

   /**
    * Set the loop pointer to the specified sample position inside the loop.
    * If loopPos < 0 or > loopLength, we clamp the new position to those legal bounds.
    * @param loopPos sample # (0..loopLength);
    */
   void SeekAbsolute(int loopPos = 0);

    /**
     * Set the track's output volume.
     * @param volumeInDb dB, probably -96..0
     */
//    void SetOutputVolume(float volumeInDb);

    /**
     * Get the current track output volume in dB
     * @return floating point dB value, probably in the range -96.0 .. 0.0
     */
//    float GetOutputVolume() const;

   /**
    * Connect a ChangeListener object to one of the things we own that can send change notifications.
    * @param  add     If true, add the `listener`, otherwise remove it.
    * @param target   One of the `ListenTo` enum items (kPreFx, kTrack, kPostFx)
    * @param listener Pointer to the object we need to send updates to.
    */
//   void UpdateChangeListeners(bool add, ListenTo target, ChangeListener* listener);


   /**
    * Get  pointer to the LoopProcessor for this track.
    * @return pointer.
    */
//   LoopProcessor* GetLoop() const { return fLoop; };

private:
    JUCE_DECLARE_NON_COPYABLE(Track);
   
    /**
     * The looper object that owns us. We do not own this pointer, so
     * don't delete it.
     */
    Looper* looper;

    /**
     * We can give each track a name for display in the UI.
     */
    String name;

    /**
     * Is this track currently muted?
     */
    bool muted = false;
    bool selected = false;
    float loopDuration = 0.0f;

   /**
    * dB of gain to apply to the input to this track.
    */
//   float fInputGain;

   /**
    * Where should a mono input signal be panned?
    */
//   float fPan;

   /**
    * A non-owning pointer to the input processor for this track
    */
//   InputProcessor* fInputProcessor;
//
//   /**
//    * the node Id of the input processor
//    */
//   NodeId fInputId;
//
//   /**
//    * A non-owning pointer to the loop processor for this track.
//    */
//   LoopProcessor* fLoop;
//
//   /**
//    * node id of the loop.
//    */
//   NodeId fLoopId;
//
//   /**
//    * A non-owning pointer to the output volume processor for this track.
//    */
//   GainProcessor*   fOutputGain;
//   NodeId fVolumeId;
//
//   /**
//    * output volume in dB
//    */
//   float fOutputVolume;
//
   CriticalSection mutex;
};
