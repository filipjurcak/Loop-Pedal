/*
  ==============================================================================

    Looper.cpp
    Created: 12 Jun 2020 4:09:42pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#include "Looper.h"
#include "LooperDefs.h"
#include "Track.h"
#include "Muter.h"
#include "Passthrough.h"

using Connection = AudioProcessorGraph::Connection;
using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
using Node = AudioProcessorGraph::Node;

Looper::Looper(AudioDeviceManager& deviceManager, int numOfTracks): graph(new AudioProcessorGraph()), deviceManager(deviceManager)
{
    graph->enableAllBuses();
    graph->clear();
    deviceManager.addAudioCallback(&player);

    // Create and add new input/output processor nodes.
    inputNode = graph->addNode(std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioInputNode));
    outputNode = graph->addNode(std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioOutputNode));
    this->Connect(this->inputNode, this->outputNode);
    
    muterNode = graph->addNode(std::make_unique<MuterProcessor>(2));
    this->InsertBetween(inputNode, muterNode, outputNode);
    // for now, as we start in PlayInRecord mode, bypass the muter
    muterNode->setBypassed(true);
    
    player.setProcessor(graph.get());

    for (int i = 0; i < numOfTracks; i++) {
        this->tracks.add(new Track(this, this->trackNames[i], this->selectedTrack == i));
    }
    auto midiInputs = MidiInput::getAvailableDevices();
    for (auto midiInput: midiInputs) {
        if (midiInput.name.startsWith("Arduino")) {
            this->setMidiInput(midiInputs.indexOf(midiInput));
        }
    }
}

Looper::~Looper()
{
    this->removeAllChangeListeners();
    this->deviceManager.removeAudioCallback(&player);
    this->player.setProcessor(nullptr);
    this->graph->clear();
    this->deviceManager.removeMidiInputDeviceCallback (MidiInput::getAvailableDevices()[lastMidiInputIndex].identifier, this);
}

int Looper::GetNumberOfTracks()
{
    return tracks.size();
}

LoopStates Looper::GetMode() const
{
    return this->mode;
}

bool Looper::IsGlobalMuteOn() const
{
    return globalMute;
}

RecordState Looper::GetRecordState() const
{
    return recordState;
}

Track* Looper::GetSelectedTrack() const
{
    return this->tracks[this->selectedTrack];
}

void Looper::Connect(Node::Ptr source, Node::Ptr dest)
{
   this->HandleConnection(source, dest, true);
}

void Looper::Disconnect(Node::Ptr source, Node::Ptr dest)
{
    this->HandleConnection(source, dest, false);
}

void Looper::HandleConnection(Node::Ptr source, Node::Ptr dest, bool connecting)
{
    // only proceed if those nodes exist.
    if (source && dest)
    {
        if (connecting)
        {
            // if they're already connected, there's nothing to do.
            if (graph->isConnected(source->nodeID, dest->nodeID))
            {
                return;
            }
            // verify that we can at least connect the lower channels of these nodes.
            if (!graph->canConnect({ { source->nodeID, 0 }, { dest->nodeID, 0 } }))
            {
                Logger::writeToLog("Nodes with IDs " + String(source->nodeID.uid) + " and " + String(dest->nodeID.uid) + " can't be connected");
                exit(1);
            }
        }
        else
        {
            // bail out early if the two nodes aren't connected.
            if (!graph->isConnected(source->nodeID, dest->nodeID))
            {
                Logger::writeToLog("Nodes with IDs " + String(source->nodeID.uid) + " and " + String(dest->nodeID.uid) + " are not connected");
                exit(1);
            }
        }

        AudioProcessor* srcFilter = source->getProcessor();
        int numSrcChannels = srcFilter->getTotalNumOutputChannels();
        AudioProcessor* destFilter = dest->getProcessor();
        int numDestChannels = destFilter->getTotalNumInputChannels();
        // in our immediate situation, we're only interested in 2 channels at most.
        // Future versions might be interested in more.
        if (numSrcChannels == numDestChannels)
        {
            for (int index = 0; index < numSrcChannels; index++)
            {
                Connection connection = { { source->nodeID, index }, { dest->nodeID, index } };
                if (connecting) {
                    graph->addConnection(connection);
                } else {
                    graph->removeConnection(connection);
                }
            }
        }
        else if (numSrcChannels < numDestChannels)
        {
            // connect the single input to both pins of the destination filter
            for (int index = 0; index < numDestChannels; index++) {
                Connection connection = { { source->nodeID, 0 }, { dest->nodeID, index } };
                if (connecting) {
                    graph->addConnection(connection);
                } else {
                    graph->removeConnection(connection);
                }
            }
        }
        else // numSrcChannels > numDestChannels
        {
            // connect both source pins to the single pin of the dest filter.
            for (int index = 0; index < numSrcChannels; index++) {
                Connection connection = { { source->nodeID, index }, { dest->nodeID, 0 } };
                if (connecting) {
                    graph->addConnection(connection);
                } else {
                    graph->removeConnection(connection);
                }
            }
        }
    }
    else
    {
        Logger::writeToLog("Something went wrong when (dis)connecting nodes with IDs " + String(source->nodeID.uid) + " and " + String(dest->nodeID.uid));
        exit(1);
    }
}

void Looper::InsertBetween(Node::Ptr before, Node::Ptr newNode, Node::Ptr after)
{
    // the new connections both need to be legal before we start messing with things.
    if (!graph->canConnect({ { before->nodeID, 0 }, { newNode->nodeID, 0 } }) ||
        !graph->canConnect({ { newNode->nodeID, 0 }, { after->nodeID, 0 } }))
    {
        Logger::writeToLog("I can't insert node with ID " + String(newNode->nodeID.uid) + " between nodes with IDs " + String(before->nodeID.uid) + " and " + String(after->nodeID.uid));
        exit(1);
    }
    
    // disconnect the two nodes if they are already connected
    if (graph->isConnected(before->nodeID, after->nodeID))
    {
        this->Disconnect(before, after);
    }

    this->Connect(before, newNode);
    this->Connect(newNode, after);
}

Node::Ptr Looper::GetInputNode() const
{
    return inputNode;
}

Node::Ptr Looper::GetOutputNode() const
{
    return outputNode;
}

Node::Ptr Looper::AddProcessor(std::unique_ptr<AudioProcessor> processor) {
    return graph->addNode(std::move(processor));
}

void Looper::ChangeMode(LoopStates mode)
{
    ScopedLock sl(this->mutex);
    this->mode = mode;
    muterNode->setBypassed(this->mode == LoopStates::Play or this->mode == LoopStates::PlayInRecord);
    this->sendChangeMessage();
}

void Looper::ChangeSelectedTrack(int index)
{
    ScopedLock sl(this->mutex);
    this->tracks[this->selectedTrack]->UnselectTrack();
    this->selectedTrack = index;
    this->tracks[this->selectedTrack]->SelectTrack();
    this->sendChangeMessage();
}

void Looper::ResetAllTracks()
{
    for (int i = 0; i < this->tracks.size(); i++)
    {
       this->tracks[i]->ResetLoop(true);
    }
    this->ChangeSelectedTrack(0);
    startLoopingTime = loopTime = -1.0f;
    recordState = RecordState::NotRecorded;
    
    this->ChangeMode(LoopStates::PlayInRecord);
}

void Looper::PlayAllTracks()
{
    for (auto track: this->tracks)
    {
        globalMute = false;
        track->PlayFromBeginning();
    }
}

void Looper::StopAllTracks()
{
    globalMute = true;
    this->sendChangeMessage();
}

Track* Looper::GetTrack(int index) const
{
   // if index is out of range (or if the array is legitimately holding a nullptr), this will return nullptr;
   return tracks[index];
}

void Looper::setMidiInput(int index) {
    auto list = MidiInput::getAvailableDevices();
    deviceManager.removeMidiInputDeviceCallback(list[lastMidiInputIndex].identifier, this);
    auto newInput = list[index];
    if (! deviceManager.isMidiInputDeviceEnabled (newInput.identifier)) {
        deviceManager.setMidiInputDeviceEnabled (newInput.identifier, true);
    }

    deviceManager.addMidiInputDeviceCallback (newInput.identifier, this);
    lastMidiInputIndex = index;
}

void Looper::handleIncomingMidiMessage(MidiInput *source, const MidiMessage& message) {
    if (message.isNoteOn()) {
        int noteNumber = message.getNoteNumber();

        if (noteNumber == ButtonMidiNotes::Undo)
        {
            // TODO
            Logger::writeToLog("Undo on track " + String(this->selectedTrack + 1));
        }

        if (this->mode == LoopStates::Play)
        {
            if (noteNumber == ButtonMidiNotes::Recplay)
            {
                this->PlayAllTracks();
            }
            else if (noteNumber == ButtonMidiNotes::Stop)
            {
                this->StopAllTracks();
            }
            else if (noteNumber == ButtonMidiNotes::Mode)
            {
                this->ChangeMode(LoopStates::PlayInRecord);
            }
            else if (noteNumber == ButtonMidiNotes::Clear)
            {
                this->ResetAllTracks();
            }
            else if (ButtonMidiNotes::Track1 <= noteNumber && noteNumber <= ButtonMidiNotes::Track4) {  // one of the tracks pressed
                int firstTrackIndex = ButtonMidiNotes::Track1;
                int trackIndex = noteNumber - firstTrackIndex;
                if (this->tracks[trackIndex]->IsMuted())
                {
                    this->tracks[trackIndex]->Mute(false);
                }
                else
                {
                    this->tracks[trackIndex]->Mute(true);
                }
            }
        }
        else
        {
            if (noteNumber == ButtonMidiNotes::Recplay)
            {
                if (this->mode == LoopStates::PlayInRecord)
                {
                    if (recordState == RecordState::NotRecorded)
                    {
                        this->startLoopingTime = Time::getMillisecondCounterHiRes() * 0.001;
                        recordState = RecordState::StartedRecording;
                        this->ChangeMode(LoopStates::Record);
                    }
                    else
                    {
                        this->ChangeMode(LoopStates::Overdub);
                    }
                }
                else if (this->mode == LoopStates::Record)
                {
                    if (recordState == RecordState::StartedRecording)
                    {
                        this->loopTime = Time::getMillisecondCounterHiRes() * 0.001 - startLoopingTime;
                        recordState = RecordState::Recorded;
                        this->ChangeMode(LoopStates::Overdub);
                        for (int i = 0; i < this->tracks.size(); i++)
                        {
                            this->tracks[i]->SetLoopDuration(int(this->loopTime * 1000));
                        }
                    }
                }
                else if (this->mode == LoopStates::Overdub)
                {
                    this->ChangeMode(LoopStates::PlayInRecord);
                }
            }
            else if (noteNumber == ButtonMidiNotes::Mode)
            {
                this->ChangeMode(LoopStates::Play);
            }
            else if (noteNumber == ButtonMidiNotes::Stop)
            {
                if (this->tracks[this->selectedTrack]->IsMuted())
                {
                    this->tracks[this->selectedTrack]->Mute(false);
                }
                else
                {
                    this->tracks[this->selectedTrack]->Mute(true);
                }
            }
            else if (noteNumber == ButtonMidiNotes::Clear)
            {
                this->tracks[this->selectedTrack]->ResetLoop(false);
            }
            else if (ButtonMidiNotes::Track1 <= noteNumber && noteNumber <= ButtonMidiNotes::Track4) {  // one of the tracks pressed
                int previousSelectedTrack = this->selectedTrack;
                int firstTrackIndex = ButtonMidiNotes::Track1;
                int selectedTrack = noteNumber - firstTrackIndex;
                this->ChangeSelectedTrack(selectedTrack);
                if (this->mode == LoopStates::Record)
                {
                    if (recordState == RecordState::StartedRecording)
                    {
                        this->loopTime = Time::getMillisecondCounterHiRes() * 0.001 - startLoopingTime;
                        recordState = RecordState::Recorded;
                        for (int i = 0; i < this->tracks.size(); i++)
                        {
                            this->tracks[i]->SetLoopDuration(int(this->loopTime * 1000));
                        }
                    }
                    this->ChangeMode(LoopStates::Overdub);
                }
                else if (this->mode == LoopStates::Overdub)
                {
                    if (previousSelectedTrack == this->selectedTrack)
                    {
                        this->ChangeMode(LoopStates::PlayInRecord);
                    }
                }
            }
        }
    }
    else
    {
        Logger::writeToLog("Unsupported MIDI note command with note " + String(message.getNoteNumber()));
        exit(1);
    }
}
