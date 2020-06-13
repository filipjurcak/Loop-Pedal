/*
  ==============================================================================

    Track.cpp
    Created: 12 Jun 2020 4:12:38pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#include "Track.h"

Track::Track(Looper* looper, const String& name, bool selected)
: looper(looper)
, name(name)
, selected(selected)
//,  fInputActive(false)
//,  fInputGain(0.f)
//,  fPan(0.5)
//,  fPreEffectCount(preFxCount)
//,  fPreEffects(nullptr)
//,  fPostEffectCount(postFxCount)
//,  fPostEffects(nullptr)
//,  fLoop(nullptr)
//,  fLoopId(tk::kInvalidNode)
//,  fOutputGain(nullptr)
//,  fVolumeId(tk::kInvalidNode)
//,  fOutputVolume(0.0)
{
   // we need the input and output nodes that the Scumbler controls.
   NodeId input = looper->HandleSpecialNode(Node::Input);
   NodeId output = looper->HandleSpecialNode(Node::Output);

   // create an insert the input processor
//   fInputProcessor = new InputProcessor(this, 2);
//   this->SetInputGain(fInputGain);
//   this->SetInputPan(fPan);
//   fInputId = fScumbler->AddProcessor(fInputProcessor);
//
//   bool disconnectInputAndOutput = (0 == Track::sTrackCount++);
//
//   fScumbler->InsertBetween(input, fInputId, output, disconnectInputAndOutput);


   // create and insert the gain processor.
//   fOutputGain = new GainProcessor(this, 2);
//   fVolumeId = fScumbler->AddProcessor(fOutputGain);
//   fScumbler->InsertBetween(fInputId, fVolumeId, output);
//
//   // create & insert the loop processor
//   fLoop = new LoopProcessor(this, 2);
//   fLoopId = fScumbler->AddProcessor(fLoop);
//   fScumbler->InsertBetween(fInputId, fLoopId, fVolumeId);
//
//   this->addChangeListener(fScumbler);


}

Track::~Track()
{
   NodeId input = looper->HandleSpecialNode(Node::Input);
   NodeId output = looper->HandleSpecialNode(Node::Output);

   // remove the loop & delete it
//   looper->RemoveBetween(fInputId, fLoopId, fVolumeId, true);
//   // remove the output gain node & delete it.
//   fScumbler->RemoveBetween(fInputId, fVolumeId, output, true);
//   // remove the input processor & delete it.
//
//   bool reconnectInputAndOutput = (0 == --Track::sTrackCount);
//
//   fScumbler->RemoveBetween(input, fInputId, output, true, reconnectInputAndOutput);
//
//   this->removeChangeListener(fScumbler);
}

String Track::GetName() const
{
   return this->name;
}

void Track::MuteUnmute()
{
    ScopedLock sl(this->mutex);
    this->muted = !this->muted;
    this->sendChangeMessage();
}

bool Track::IsMuted() const
{
   ScopedLock sl(this->mutex);
   return this->muted;
}

void Track::SelectTrack()
{
    ScopedLock sl(this->mutex);
    this->selected = true;
    this->sendChangeMessage();
}

void Track::UnselectTrack()
{
    ScopedLock sl(this->mutex);
    this->selected = false;
    this->sendChangeMessage();
}

bool Track::IsSelected() const
{
    ScopedLock sl(this->mutex);
    return this->selected;
}

void Track::SetLoopDuration(float duration)
{
    ScopedLock sl(this->mutex);
    this->loopDuration = duration;
    this->sendChangeMessage();
}

//void Track::SetInputGain(float gainInDb)
//{
//   if (gainInDb != fInputGain)
//   {
//      fInputGain = gainInDb;
//
//      float gain = DbToFloat(fInputGain);
//      fInputProcessor->SetGain(gain);
//
//      // update our observers.
//      // std::cout << "Track::SetInputGain->sendChangeMessage" << std::endl;
//      this->sendChangeMessage();
//   }
//}

//float Track::GetInputGain() const
//{
//   return fInputGain;
//}
//
//tk::Result Track::SetInputPan(float pan)
//{
//   if (fPan != pan)
//   {
//      fInputProcessor->SetPan(pan);
//      fPan = pan;
//      // std::cout << "Track::SetInputPan->sendChangeMessage" << std::endl;
//      fScumbler->SetDirty();
//      this->sendChangeMessage();
//   }
//   return tk::kSuccess;
//
//}
//
//float Track::GetInputPan() const
//{
//   return fPan;
//}
//
//void Track::SetEnabledChannels(tk::ChannelEnable channels)
//{
//   if (this->GetEnabledChannels() != channels)
//   {
//      fInputProcessor->SetEnabledChannels(channels);
//      std::cout << "Track::SetEnabledChannels->sendChangeMessage" << std::endl;
//      fScumbler->SetDirty();
//      this->sendChangeMessage();
//   }
//}
//
//tk::ChannelEnable Track::GetEnabledChannels() const
//{
//   return fInputProcessor->GetEnabledChannels();
//}

void Track::ResetLoop()
{
//   fLoop->Reset();
}

void Track::SeekAbsolute(int loopPos)
{
//   if (fLoop)
//   {
//      fLoop->SeekAbsolute(loopPos);
//   }

}

//void Track::SetOutputVolume(float volumeInDb)
//{
//   if (volumeInDb != fOutputVolume)
//   {
//      fOutputVolume = volumeInDb;
//
//      float gain = DbToFloat(fOutputVolume);
//      fOutputGain->SetGain(gain);
//
//      // update our observers.
//      //std::cout << "Track::SetOutputVolume->sendChangeMessage" << std::endl;
//      fScumbler->SetDirty();
//      this->sendChangeMessage();
//   }
//
//}
//
//float Track::GetOutputVolume() const
//{
//   return fOutputVolume;
//}


//void Track::UpdateChangeListeners(bool add, ListenTo target, ChangeListener* listener)
//{
//   ChangeBroadcaster* sender;
//   switch (target)
//   {
//      case kPreFx: sender = fPreEffects; break;
//
//      case kTrack: sender = this; break;
//
//      case kPostFx: sender = fPostEffects; break;
//
//      case kLoop: sender = fLoop; break;
//
//      // assert on an error.
//      default: jassert(false); break;
//   }
//
//   if (add)
//   {
//      sender->addChangeListener(listener);
//   }
//   else
//   {
//      sender->removeChangeListener(listener);
//   }
//}
