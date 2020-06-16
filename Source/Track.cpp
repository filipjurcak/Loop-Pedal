/*
  ==============================================================================

    Track.cpp
    Created: 12 Jun 2020 4:12:38pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#include "Track.h"
using Node = AudioProcessorGraph::Node;

Track::Track(Looper* looper, const String& name, bool selected)
: looper(looper)
, name(name)
, selected(selected)
{
    // we need the input and output nodes that the Scumbler controls.
    Node::Ptr inputNode = looper->GetInputNode();
    Node::Ptr outputNode = looper->GetOutputNode();

    // create & insert the loop processor
    std::unique_ptr<AudioProcessor> loopProcessor = std::make_unique<LoopProcessor>(this, 2);
    auto loopNode = looper->AddProcessor(std::move(loopProcessor));
    // this is ugly, but I don't know how to do it better yet
    loop = dynamic_cast<LoopProcessor*>(loopNode->getProcessor());
    looper->InsertBetween(inputNode, loopNode, outputNode);
    
//    auto inputGainNode = looper->AddProcessor(std::make_unique<GainProcessor>(this, 2, 0.0f));
//    std::cout << "NodeID inputGainNode-u je " << inputGainNode->nodeID.uid << "\n";
//    looper->InsertBetween(inputNode, inputGainNode, loopNode);

    looper->addChangeListener(this);
}

Track::~Track()
{
    
}

void Track::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == looper and looper->IsGlobalMuteOn())
    {
        this->sendChangeMessage();
    }
}


String Track::GetName() const
{
   return this->name;
}

void Track::Mute(bool mute)
{
    ScopedLock sl(this->mutex);
    this->muted = mute;
    this->sendChangeMessage();
}

bool Track::IsMuted() const
{
   ScopedLock sl(this->mutex);
   return this->muted;
}

bool Track::IsGlobalMuteOn() const
{
    ScopedLock sl(this->mutex);
    return looper->IsGlobalMuteOn();
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

void Track::SetLoopDuration(int duration)
{
    ScopedLock sl(this->mutex);
    loop->SetLoopDuration(duration);
    this->sendChangeMessage();
}

LoopStates Track::GetLoopState()
{
    return looper->GetMode();
}

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

void Track::ResetLoop(bool resetingAllTracks)
{
    if (resetingAllTracks)
    {
        this->UnselectTrack();
    }
    loop->Reset(resetingAllTracks);
    this->Mute(false);
}

void Track::PlayFromBeginning()
{
    loop->PlayFromBeginning();
    this->sendChangeMessage();
}

void Track::StopTrack()
{
    loop->StopLoop();
}


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
