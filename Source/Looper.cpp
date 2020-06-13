/*
  ==============================================================================

    Looper.cpp
    Created: 12 Jun 2020 4:09:42pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#include "Looper.h"
#include <math.h>
#include "LooperDefs.h"
#include "Track.h"

typedef AudioProcessorGraph::NodeID NodeID;

//#include "Gain.h"
//#include "Passthrough.h"
//#include "SampleCounter.h"



float DbToFloat(float db) {
   return pow(10.0, db/20.0);
}

float GainToDb(float gain) {
   return 20.0 * log10(gain);
}


Looper::Looper(AudioDeviceManager& deviceManager, int numOfTracks): deviceManager(deviceManager)
{
    this->StartProcessing();
    this->Reset();
    this->SetOutputVolume(outputVolume);
    for (int i = 0; i < numOfTracks; i++) {
        this->tracks.add(new Track(this, this->trackNames[i], this->selectedTrack == i));
    }
    auto midiInputs = MidiInput::getAvailableDevices();
    for (auto midiInput: midiInputs) {
        if (midiInput.name.startsWith("Arduino")) {
            Logger::writeToLog("Arduino found.");
            this->setMidiInput(midiInputs.indexOf(midiInput));
        }
    }
}

Looper::~Looper()
{
//    this->removeAllChangeListeners();
    this->StopProcessing();
    this->graph.clear();
//    deviceManager.removeMidiInputDeviceCallback (MidiInput::getAvailableDevices()[lastInputIndex].identifier, this);
}


LooperResult Looper::StartProcessing()
{
//   LooperResult retval = LooperResult::kAlreadyStarted;
//   if (!fProcessing)
//   {
//      fPlayer.setProcessor(&fGraph);
//      fDeviceManager.addAudioCallback(&fPlayer);
//      fProcessing = true;
//   }
//   return retval;
    return LooperResult::Success;
}


LooperResult Looper::StopProcessing()
{
//   tk::Result retval = tk::kAlreadyStopped;
//   if (fProcessing)
//   {
//      fDeviceManager.removeAudioCallback(&fPlayer);
//      fPlayer.setProcessor(nullptr);
//      fProcessing = false;
//      retval = tk::kSuccess;
//   }
//   return retval;
    return LooperResult::Success;
}

//void Looper::changeListenerCallback(ChangeBroadcaster* source)
//{
//   // std::cout << "Scumbler::changeListenerCallback()" << std::endl;
////   if (source == sampleCount)
////   {
////      // just notify that we've changed so the time readout can change.
////      timeUpdate = true;
////       //mMsg('.');
////      //this->sendChangeMessage();
////   }
//}

void Looper::TogglePlay()
{
   if (this->playing)
   {
      this->Pause();
   }
   else
   {
      this->Play();
   }
   // std::cout << "Scumbler::TogglePlay->sendChangeMessage" << std::endl;
//   this->sendChangeMessage();

}

bool Looper::IsPlaying() const
{
   return playing;
}

bool Looper::UpdateTime()
{
   bool retval = timeUpdate;
   timeUpdate = false;
   return retval;
}


void Looper::Reset(bool addFirstTrack)
{
    this->Pause();

    // clear out the processor graph
    graph.clear();

    // Create and add new input/output processor nodes.
    // TODO check how to free them correctly
//    AudioProcessorGraph::AudioGraphIOProcessor* in = new AudioProcessorGraph::AudioGraphIOProcessor(
//      AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
//    inputNode = this->AddProcessor(in);
//
//    AudioProcessorGraph::AudioGraphIOProcessor* out = new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
//    outputNode = this->AddProcessor(out);
//
//    this->Connect(inputNode, outputNode);

   // connect a gain processor in the middle:
//   GainProcessor* gain = new GainProcessor(nullptr, 2);
//   NodeId gainNode = this->AddProcessor(gain);
//   if (tk::kSuccess == this->InsertBetween(fInputNode, gainNode, fOutputNode))
//   {
//      fOutputGain = gain;
//      fGainNode = gainNode;

//   }
//   else
//   {
//      outputGain = nullptr;
      gainNode = NodeId(Node::InvalidNode);
//   }

   // Add the processor that counts samples for us
//   fSampleCount = new SampleCounterProcessor(this, 5000);
//   fSampleCount->addChangeListener(this);
//   fSampleCountNode = this->AddProcessor(fSampleCount);
//   this->Connect(fInputNode, fSampleCountNode);


   // Delete any tracks that we have, returning to zero tracks.
//   fTracks.clear();
//
//   if (addFirstTrack)
//   {
//      // ... and then add a single track to start out.
//      this->AddTrack();
//      // (and make sure it's active so it receives input!)
//      this->ActivateTrack(0);
//   }
//   // let anyone listening tk::know that we've changed.
//   // std::cout << "Scumbler::Reset->sendChangeMessage" << std::endl;
//   this->sendChangeMessage();

}

int Looper::GetNumberOfTracks()
{
    return tracks.size();
}

LoopStates Looper::GetMode() const
{
    return this->mode;
}

int Looper::GetSelectedTrack() const
{
    return this->selectedTrack;
}

void Looper::SetOutputVolume(float volumeInDb)
{
   if (volumeInDb != outputVolume)
   {
      outputVolume = volumeInDb;

      //!!! Send the new gain to the audio processor that actually controls
      // the output.
      float gain = DbToFloat(outputVolume);
//      outputGain->SetGain(gain);
   }
}

float Looper::GetOutputVolume() const
{
   return outputVolume;
}

uint64 Looper::GetSampleCount() const
{
//   return fSampleCount->GetSampleCount();
    return 0;
}

LooperResult Looper::Connect(NodeId source, NodeId dest)
{
   return this->HandleConnection(source, dest, true);
}

LooperResult Looper::Disconnect(NodeId source, NodeId dest)
{
   return this->HandleConnection(source, dest, false);
}


LooperResult Looper::InsertBetween(NodeId before, NodeId newNode, NodeId after, bool disconnect)
{
    LooperResult retval = LooperResult::Failure;

    NodeID beforeNode = NodeID(this->HandleSpecialNode(before));
    NodeID afterNode = NodeID(this->HandleSpecialNode(after));
    NodeID newNodeNode = NodeID(newNode);

    if (disconnect)
    {
        // 1: we can't succeed of before and after aren't connected.
        if (!graph.isConnected(beforeNode, afterNode))
        {
            return LooperResult::Failure;
        }
    }
    // the new connections both need to be legal before we start messing with things.
//    if (!graph.canConnect(beforeNode, NodeID(0), newNodeNode, NodeID(0)) ||
//        !graph.canConnect(newNodeNode, NodeID(0), afterNode, NodeID(0)))
//    {
//        return LooperResult::Failure;
//    }
//   //  first, disconnect the two nodes that are already being connected.
//   retval = disconnect ? this->Disconnect(before, after) : tk::kSuccess;
//   if (tk::kSuccess == retval)
//   {
//      retval = this->Connect(before, newNode);
//      if (tk::kSuccess == retval)
//      {
//         retval = this->Connect(newNode, after);
//      }
//   }

   return retval;
}



LooperResult Looper::RemoveBetween(NodeId before, NodeId nodeToRemove,
   NodeId after, bool deleteNode, bool reconnect)
{
   LooperResult retval = LooperResult::Failure;

   // if needed, look up the node ids for the input/output nodes.
//   before = this->HandleSpecialNode(before);
//   after = this->HandleSpecialNode(after);
//
//   // 1. First pre-condition: before must already be connected to nTR, and
//   // nTR must be connected to after.
//   if (! (graph.isConnected(before, nodeToRemove) &&
//          graph.isConnected(nodeToRemove, after)))
//   {
//      return LooperResult::Failure;
//   }
//
//   // 2. Next precondition: The connection between `before` and `after` needs
//   // to be a legal connection. I don't think this one is possible if we assume
//   // that all graphs start out connecting input->output and are after that only
//   // added to with the InsertBetween() method.
//   if (!graph.canConnect(before, 0, after, 0))
//   {
//      return LooperResult::Failure;
//   }
//
//   // 3. Disconnect `nodeToRemove on either side.
//   retval = this->Disconnect(before, nodeToRemove);
//   if (LooperResult::Success == retval)
//   {
//      retval = this->Disconnect(nodeToRemove, after);
//      if (LooperResult::Success == retval)
//      {
//         if (reconnect)
//         {
//            // 4. Re-connect the before and after nodes, as if the nodeToRemove had
//            // never been there.
//            retval = this->Connect(before, after);
//         }
//         if (deleteNode)
//         {
//            graph.removeNode(nodeToRemove);
//         }
//      }
//   }

   return retval;
}


void Looper::Play()
{
   if (!playing)
   {
      playing = true;
   }
}

void Looper::Pause()
{
   if (playing)
   {
      playing = false;
      // !!! TODO
   }
}

void Looper::ChangeMode(LoopStates mode)
{
    this->mode = mode;
    this->sendChangeMessage();
}

LooperResult Looper::ResetAllTracks()
{
//   for (int i = 0; i < this->GetNumTracks(); ++i)
//   {
//      Track* t = this->GetTrack(i);
//      t->ResetLoop();
//   }
//   // set the sample count back to zero.
//   fSampleCount->Reset();
   return LooperResult::Success;

}

LooperResult Looper::SeekAllTracksAbsolute(int loopPos)
{
//   for (int i = 0; i < this->GetNumTracks(); ++i)
//   {
//      Track* t = this->GetTrack(i);
//      t->SeekAbsolute(loopPos);
//   }

   return LooperResult::Success;

}

Track* Looper::GetTrack(int index) const
{
   // if index is out of range (or if the array is legitimately holding a
   // nullptr), this will return nullptr;
   return tracks[index];
}


LooperResult Looper::HandleConnection(NodeId source, NodeId dest, bool connecting)
{
   LooperResult retval = LooperResult::Failure;
   fnPtr op = nullptr;

//   AudioProcessorGraph::Node* srcNode  = graph.getNodeForId(source);
//   AudioProcessorGraph::Node* destNode = graph.getNodeForId(dest);
//   // only proceed if those nodes exist.
//   if (srcNode && destNode)
//   {
//      if (connecting)
//      {
//         op = &AudioProcessorGraph::addConnection;
//         // if they're already connected, there's nothing to do.
//         if (fGraph.isConnected(source, dest))
//         {
//            return tk::kAlreadyConnected;
//         }
//         // verify that we can at least connect the lower channels of these nodes.
//         if (!fGraph.canConnect(source, 0, dest, 0))
//         {
//            return tk::kIllegalConnection;
//         }
//
//      }
//      else
//      {
//         op = &AudioProcessorGraph::removeConnection;
//         // bail out early if the two nodes aren't connected.
//         if (!fGraph.isConnected(source, dest))
//         {
//            return tk::kNotConnected;
//         }
//      }
//
//      retval = tk::kSuccess;
//      AudioProcessor* srcFilter  = srcNode->getProcessor();
//      int numSrcChannels         = srcFilter->getNumOutputChannels();
//      AudioProcessor* destFilter = destNode->getProcessor();
//      int numDestChannels        = destFilter->getNumInputChannels();
//      // in our immediate situation, we're only interested in 2 channels at most.
//      // Future versions might be interested in more.
//      // !!! NOTE that a better way to do this is to instead check whether
//      // isInputChannelStereoPair/isOutputChannelStereoPair() is true and to
//      // hook things up accordingly.
//      if (numSrcChannels == numDestChannels)
//      {
//         for (int index = 0; index < numSrcChannels; ++index)
//         {
//            mCallMemberFn(fGraph, op)(source, index, dest, index);
//         }
//
//      }
//      else if (numSrcChannels < numDestChannels)
//      {
//         // connect the single input to both pins of the destination filter
//         mCallMemberFn(fGraph, op)(source, 0, dest, 0);
//         mCallMemberFn(fGraph, op)(source, 0, dest, 1);
//      }
//      else // numSrcChannels > numDestChannels
//      {
//         // connect both source pins to the single pin of the dest filter.
//         mCallMemberFn(fGraph, op)(source, 0, dest, 0);
//         mCallMemberFn(fGraph, op)(source, 1, dest, 0);
//      }
//   }
//   else
//   {
//      // one or other of the requested nodes aren't present in the graph.
//      if (nullptr == srcNode)
//      {
//         return tk::kNoSourceNode;
//      }
//      if (nullptr == destNode)
//      {
//         return tk::kNoDestNode;
//      }
//   }
   return retval;
}

NodeId Looper::AddProcessor(AudioProcessor* p)
{
   AudioProcessorGraph::Node* node;
//   node = graph.addNode(p);
//   return node->nodeId;
    return NodeId(0);
}

LooperResult Looper::DeleteNode(NodeId node)
{
    LooperResult retval = LooperResult::Failure;
    if (graph.removeNode(NodeID(node)))
    {
        retval = LooperResult::Success;
    }
    return retval;
}


AudioProcessorEditor* Looper::GetEditorForNode(NodeId node, bool useGeneric)
{
   AudioProcessorEditor* retval = nullptr;
   AudioProcessorGraph::Node* plugin = graph.getNodeForId(NodeID(node));
   if (nullptr != plugin)
   {
      if (!useGeneric)
      {
         retval = plugin->getProcessor()->createEditorIfNeeded();
         if (nullptr == retval)
         {
            // we failed to create a native editor, so fall back to the generic.
            useGeneric = true;
         }
      }
      if (useGeneric)
      {
         retval = new GenericAudioProcessorEditor(plugin->getProcessor());
      }

   }
   return retval;
}


LooperResult Looper::GetStateInformationForNode(NodeId nodeId, MemoryBlock& m)
{
   LooperResult retval = LooperResult::Failure;
   AudioProcessorGraph::Node* node = graph.getNodeForId(NodeID(nodeId));
   if (nullptr != node)
   {
      // get the actual processor object behind this node, and have
      // it stuff its state data into the memory block that we've been passed.
      AudioProcessor* processor = node->getProcessor();
      processor->getStateInformation(m);
      retval = LooperResult::Success;
   }

   return retval;
}

LooperResult Looper::SetStateInformationForNode(NodeId nodeId, MemoryBlock& m)
{
   LooperResult retval = LooperResult::Failure;
   AudioProcessorGraph::Node* node = graph.getNodeForId(NodeID(nodeId));
   if (nullptr != node)
   {
      // get the actual processor object behind this node, and have
      // it restore its state from the passed in memory block.
      AudioProcessor* processor = node->getProcessor();
      processor->setStateInformation(m.getData(), (int) m.getSize());
      retval = LooperResult::Success;
   }

   return retval;
}


NodeId Looper::HandleSpecialNode(NodeId node) {
   NodeId retval = node;
   if (Node::Input == node) {
      retval = inputNode;
   } else if (Node::Output == node) {
      // if we have inserted a gain processor before the output, that should
      // be treated as the output; everything goes through it.
      retval = (gainNode == Node::InvalidNode)  ? outputNode : gainNode;
   }

   return retval;
}

void Looper::setMidiInput (int index) {
    auto list = MidiInput::getAvailableDevices();
    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, this);
    auto newInput = list[index];
    if (! deviceManager.isMidiInputDeviceEnabled (newInput.identifier)) {
        deviceManager.setMidiInputDeviceEnabled (newInput.identifier, true);
    }

    deviceManager.addMidiInputDeviceCallback (newInput.identifier, this);

    lastInputIndex = index;
}

void Looper::handleIncomingMidiMessage(MidiInput *source, const MidiMessage& message) {
    if (message.isNoteOn()) {
        int noteNumber = message.getNoteNumber();

        if (noteNumber == int(ButtonMidiNotes::Undo)) {
            Logger::writeToLog("Undo on track " + String(this->selectedTrack + 1));
        }

        if (this->mode == LoopStates::Play) {
            if (noteNumber == int(ButtonMidiNotes::Recplay)) {
                Logger::writeToLog("Play All");
            } else if (noteNumber == int(ButtonMidiNotes::Stop)) {
                Logger::writeToLog("Stop All");
            } else if (noteNumber == int(ButtonMidiNotes::Mode)) {
                this->ChangeMode(LoopStates::PlayInRecord);
                Logger::writeToLog("Record mode - play in record");
            } else if (noteNumber == int(ButtonMidiNotes::Clear)) {
                Logger::writeToLog("Clear All");
            } else if (noteNumber == int(ButtonMidiNotes::Track1)) {
                this->tracks[0]->MuteUnmute();
                if (this->tracks[0]->IsMuted()) {
                    Logger::writeToLog("Mute Track 1");
                } else {
                    Logger::writeToLog("Play Track 1");
                }
            } else if (noteNumber == int(ButtonMidiNotes::Track2)) {
                this->tracks[1]->MuteUnmute();
                if (this->tracks[1]->IsMuted()) {
                    Logger::writeToLog("Mute Track 2");
                } else {
                    Logger::writeToLog("Play Track 2");
                }
            } else if (noteNumber == int(ButtonMidiNotes::Track3)) {
                this->tracks[2]->MuteUnmute();
                if (this->tracks[2]->IsMuted()) {
                    Logger::writeToLog("Mute Track 3");
                } else {
                    Logger::writeToLog("Play Track 3");
                }
            } else if (noteNumber == int(ButtonMidiNotes::Track4)) {
                this->tracks[3]->MuteUnmute();
                if (this->tracks[3]->IsMuted()) {
                    Logger::writeToLog("Mute Track 4");
                } else {
                    Logger::writeToLog("Play Track 4");
                }
            }
        } else {
            if (noteNumber == int(ButtonMidiNotes::Recplay)) {
                if (this->mode == LoopStates::PlayInRecord) {
                    if (!wasRecorded) {
                        this->startLoopingTime = Time::getMillisecondCounterHiRes() * 0.001;
                        Logger::writeToLog("Record on Track " + String(this->selectedTrack + 1));
                        this->ChangeMode(LoopStates::Record);
                    } else {
                        this->ChangeMode(LoopStates::Overdub);
                        Logger::writeToLog("Overdubbing on Track " + String(this->selectedTrack + 1) + " after play in record");
                    }
                } else if (this->mode == LoopStates::Record) {
                    if (!this->wasRecorded) {
                        this->loopTime = Time::getMillisecondCounterHiRes() * 0.001 - startLoopingTime;
                        for (int i = 0; i < this->tracks.size(); i++) {
                            this->tracks[i]->SetLoopDuration(this->loopTime);
                        }
                        Logger::writeToLog("Loop time is " + String(this->loopTime));
                        this->wasRecorded = true;
                        Logger::writeToLog("Overdub after first recording");
                    } else {
                        Logger::writeToLog("Overdub after play in record");
                    }
                    this->ChangeMode(LoopStates::Overdub);
                } else if (this->mode == LoopStates::Overdub) {
                    this->ChangeMode(LoopStates::PlayInRecord);
                    Logger::writeToLog("Play in record");
                }
            } else if (noteNumber == int(ButtonMidiNotes::Mode)) {
                this->ChangeMode(LoopStates::Play);
                Logger::writeToLog("Play Mode");
            } else if (noteNumber == int(ButtonMidiNotes::Stop)) {
                if (this->selectedTrack == 0) {
                    this->tracks[0]->MuteUnmute();
                    if (this->tracks[0]->IsMuted()) {
                        Logger::writeToLog("Muting Track 1 during recording");
                    } else {
                        Logger::writeToLog("Unmuting Track 1 during recording");
                    }
                } else if (selectedTrack == 1) {
                    this->tracks[1]->MuteUnmute();
                    if (this->tracks[1]->IsMuted()) {
                        Logger::writeToLog("Muting Track 2 during recording");
                    } else {
                        Logger::writeToLog("Unmuting Track 2 during recording");
                    }
                } else if (this->selectedTrack == 2) {
                    this->tracks[2]->MuteUnmute();
                    if (tracks[2]->IsMuted()) {
                        Logger::writeToLog("Muting Track 3 during recording");
                    } else {
                        Logger::writeToLog("Unmuting Track 3 during recording");
                    }
                } else if (this->selectedTrack == 3) {
                    this->tracks[3]->MuteUnmute();
                    if (this->tracks[3]->IsMuted()) {
                        Logger::writeToLog("Muting Track 4 during recording");
                    } else {
                        Logger::writeToLog("Unmuting Track 4 during recording");
                    }
                }

            } else if (noteNumber == int(ButtonMidiNotes::Clear)) {
                Logger::writeToLog("Clear track " + String(this->selectedTrack + 1));
//                this->tracks[this->selectedTrack]->sendChangeMessage();
            } else if (noteNumber == int(ButtonMidiNotes::Track1)) {
                int previousSelectedTrack = this->selectedTrack;
                this->tracks[previousSelectedTrack]->UnselectTrack();
                this->selectedTrack = 0;
                this->tracks[0]->SelectTrack();
                if (this->mode == LoopStates::Record) {
                    this->ChangeMode(LoopStates::Overdub);
                    if (previousSelectedTrack != 0) {
                        Logger::writeToLog("Changing record to overdub from track " + String(previousSelectedTrack + 1) + " to Track 1");
                    }
                    Logger::writeToLog("Changing record to overdub");
                } else if (this->mode == LoopStates::Overdub) {
                    if (previousSelectedTrack == 0) {
                        this->ChangeMode(LoopStates::PlayInRecord);
                        Logger::writeToLog("Changing to Play In Record on Track 1");
                    } else {
                        Logger::writeToLog("Overdub to Track 1 from Track " + String(previousSelectedTrack + 1));
                    }
                } else {
                    Logger::writeToLog("Selected Track 1");
                }
            } else if (noteNumber == int(ButtonMidiNotes::Track2)) {
                int previousSelectedTrack = this->selectedTrack;
                this->tracks[previousSelectedTrack]->UnselectTrack();
                this->selectedTrack = 1;
                this->tracks[1]->SelectTrack();
                if (this->mode == LoopStates::Record) {
                    this->ChangeMode(LoopStates::Overdub);
                    if (previousSelectedTrack != 1) {
                        Logger::writeToLog("Changing record to overdub from track " + String(previousSelectedTrack + 1) + " to Track 2");
                    }
                    Logger::writeToLog("Changing record to overdub");
                } else if (this->mode == LoopStates::Overdub) {
                    if (previousSelectedTrack == 1) {
                        this->ChangeMode(LoopStates::PlayInRecord);
                        Logger::writeToLog("Changing to Play In Record on Track 2");
                    } else {
                        Logger::writeToLog("Overdub to Track 2 from Track " + String(previousSelectedTrack + 1));
                    }
                } else {
                    Logger::writeToLog("Selected Track 2");
                }
            } else if (noteNumber == int(ButtonMidiNotes::Track3)) {
                int previousSelectedTrack = this->selectedTrack;
                this->tracks[previousSelectedTrack]->UnselectTrack();
                this->selectedTrack = 2;
                this->tracks[2]->SelectTrack();
                if (this->mode == LoopStates::Record) {
                    this->ChangeMode(LoopStates::Overdub);
                    if (previousSelectedTrack != 2) {
                        Logger::writeToLog("Changing record to overdub from track " + String(previousSelectedTrack + 1) + " to Track 3");
                    }
                    Logger::writeToLog("Changing record to overdub");
                } else if (this->mode == LoopStates::Overdub) {
                    if (previousSelectedTrack == 2) {
                        this->mode = LoopStates::PlayInRecord;
                        Logger::writeToLog("Changing to Play In Record on Track 3");
                    } else {
                        Logger::writeToLog("Overdub to Track 3 from Track " + String(previousSelectedTrack + 1));
                    }
                } else {
                    Logger::writeToLog("Selected Track 3");
                }
            } else if (noteNumber == int(ButtonMidiNotes::Track4)) {
                int previousSelectedTrack = this->selectedTrack;
                this->tracks[previousSelectedTrack]->UnselectTrack();
                this->selectedTrack = 3;
                this->tracks[3]->SelectTrack();
                if (this->mode == LoopStates::Record) {
                    this->ChangeMode(LoopStates::Overdub);
                    if (previousSelectedTrack != 3) {
                        Logger::writeToLog("Changing record to overdub from track " + String(previousSelectedTrack + 1) + " to Track 4");
                    }
                    Logger::writeToLog("Changing record to overdub");
                } else if (this->mode == LoopStates::Overdub) {
                    if (previousSelectedTrack == 3) {
                        this->ChangeMode(LoopStates::PlayInRecord);
                        Logger::writeToLog("Changing to Play In Record on Track 4");
                    } else {
                        Logger::writeToLog("Overdub to Track 4 from Track " + String(previousSelectedTrack + 1));
                    }
                } else {
                    Logger::writeToLog("Selected Track 4");
                }
            }
        }
    } else {
        Logger::writeToLog("Unsupported MIDI note command with note " + String(message.getNoteNumber()));
    }
}
