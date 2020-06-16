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
    Node::Ptr inputNode = looper->GetInputNode();
    Node::Ptr outputNode = looper->GetOutputNode();

    // create & insert the loop processor
    std::unique_ptr<AudioProcessor> loopProcessor = std::make_unique<LoopProcessor>(looper, this, 2);
    auto loopNode = looper->AddProcessor(std::move(loopProcessor));
    // this is ugly, but I don't know how to do it better yet
    loop = dynamic_cast<LoopProcessor*>(loopNode->getProcessor());
    looper->InsertBetween(inputNode, loopNode, outputNode);
    
    std::unique_ptr<AudioProcessor> muteProcessor = std::make_unique<MuterProcessor>(2);
    muterNode = looper->AddProcessor(std::move(muteProcessor));
    looper->InsertBetween(inputNode, muterNode, loopNode);

    looper->addChangeListener(this);
}

Track::~Track()
{
    
}

void Track::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == looper)
    {
        if (looper->IsGlobalMuteOn())
            this->sendChangeMessage();
        LoopStates mode = looper->GetMode();
        this->setMuterBypass((mode == LoopStates::Record or mode == LoopStates::Overdub) and this->IsSelected());
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

void Track::setMuterBypass(bool shouldBypass)
{
    if (muterNode)
    {
        muterNode->setBypassed(shouldBypass);
    }
}
