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

    LooperResult StartProcessing();

    LooperResult StopProcessing();

//   /**
//    * Called when something needs to notify us of a change. Initially,
//    * this is only used when Track objects need to tell us that they
//    * want to be deleted.
//    * @param source object that's notifying us.
//    */
//   void changeListenerCallback(ChangeBroadcaster* source);

   /**
    * Switches the scumbler object between the play and pause states.
    */
    void TogglePlay();
   /**
    *  returns bool indicating whether the Scumbler is processing
    *   audio right now.
    */
   bool IsPlaying() const;

   /**
    * Do we need to just update the time output?
    * @return bool.
    */
   bool UpdateTime();

   /**
    * Do a complete reset on the processor graph.
    * @param addFirstTrack Should we add a single empty track to the scumbler so
    *                      it can start working right away?
    *
    * After removing (and therefore deleting) all of the processor nodes that
    * were added to the graph, creates new input & output processors and adds them.
    * **NOTE** that the Scumbler object should be reset after creation, after the
    * Audio device manager is initialized.
    */
    void Reset(bool addFirstTrack = false);
    
    int GetNumberOfTracks();
    
    LoopStates GetMode() const;
    
    int GetSelectedTrack() const;

    /**
     * Set the scumbler's master output volume.
     * @param volumeInDb dB, probably -96..0
     */
    void SetOutputVolume(float volumeInDb);

    /**
     * Get the current master output volume in dB
     * @return floating point dB value, probably in the range -96.0 .. 0.0
     */
    float GetOutputVolume() const;

    /**
     * Get the number of samples that we've processed while in play mode.
     * @return # of samples, as an unsigned long.
     */
    uint64 GetSampleCount() const;

   /**
     * Connect a source node to a destination node in the graph.
     * @return tk::Result code indicating success or the reason for failure.
     */
    LooperResult Connect(NodeId source, NodeId dest);


    /**
     * Remove a connection between two nodes in the graph. Because we're not
     * allowing any complex topologies now, we follow the same logic that was
     * used to connect
     * @param  source The node that's providing audio
     * @param  dest   The node that's receiving audio.
     * @return tk::Result code indicating success or the reason for failure.
     */
    LooperResult Disconnect(NodeId source, NodeId dest);
    /**
     * Insert 'newNode' in the graph in between 'before' and 'after'.
     * @param  before  The node that 'newNode' should be inserted after. The
     *                 special value Scumbler::kInput can be used to connect
     *                 newNode to the input node.
     * @param  newNode id of the node to connect in the middle. It should
     *                 already have been added to the graph with AddProcessor
     * @param  after   The node that 'newNode' should be inserted before. The
     *                 special value Scumbler::kOutput can be used to connect the
     *                 newNode to the Scumbler's output.
     * @param disconnect Do we need to disconnect before+after before inserting newNode?
     *                   This is useful when we're adding a track -- the input and output nodes
     *                   are no longer directly connected, so there's no reason to disconnect
     *                   them from each other first.
     * @return         tk::Result
     */
    LooperResult InsertBetween(NodeId before, NodeId newNode, NodeId after, bool disconnect=true);

    /**
     * Disconnect the node `nodetoRemove` that's connected between `before` and
     * `after`. The `nodeToRemove` is still owned by the graph and can be reconnected
     * elsewhere, unless the `deleteNode` argument is true.
     * @param  before       The node that must be connected to `nodeToRemove` as its
     *                      input. After this operation, `before` will be connected to
     *                      `after`.
     * @param  nodeToRemove The node that we're trying to remove.
     * @param  after        The node that must be connected to `nodeToRemove`
     *                      as its output. After this operation, `before` will
     *                      be connected to `after`.
     * @param  deleteNode   Should the node be deleted from the graph after it's
     *                      disconnected?
     * @param  reconnect    Should `before` and `after` be reconnected to each other after
     *                      we disconnect `nodeToRemove`? (Useful when deleting a track -- we
     *                      don't want the input and output to be directly connected again.)
     * @return              tk::Result
     */
    LooperResult RemoveBetween(NodeId before, NodeId nodeToRemove, NodeId after, bool deleteNode,
      bool reconnect=true);



    /**
     * Insert the provided AudioProcessor object into the Scumbler's filter
     * graph.  The Scumbler takes ownership of the object, and it should
     * **not** be deleted elsewhere.
     * @param  p The audio processor.
     * @return the identifier of the node.
     */
    NodeId AddProcessor(AudioProcessor* p);


   /**
     * Remove a node from the graph. This should rarely be needed outside the class
     * that implements it. An obvious use is when we load a plugin but aren't able to
     * insert it into the graph. This *shouldn't* happen. But just in case.
     * @param  node Id of the node to delete.
     * @return      Success or Fail.
     */

    LooperResult DeleteNode(NodeId node);


   /**
    * Return a pointer to an editor component for the requested Node in the graph.
    * @param  node       ID of the node we're interested in.
    * @param  useGeneric If true, use JUCE's generic editor component for this.
    * @return            Pointer to the editor component. Calling code is responsible
    *                    for cleaning this up.
    */
   AudioProcessorEditor* GetEditorForNode(NodeId node, bool useGeneric);


   /**
    * Fill a memory block with the current state of the requested node.
    * @param  node id of the node we're interested in
    * @param  m    Memory block to fill
    * @return      success/fail.
    */
   LooperResult GetStateInformationForNode(NodeId node, MemoryBlock& m);

   LooperResult SetStateInformationForNode(NodeId node, MemoryBlock& m);

    /**
     * Reset the loop buffers of all tracks that the scumbler owns, clearing them, and
     * putting all of their loop pointers at the beginning.
     * @return success/fail.
     */
    LooperResult ResetAllTracks();

    /**
     * Reset the loop pointers of all tracks to the specified position (default =
     * beginning), but *don't* do anything to the loop contents   .
     * @param  loopPos Sample#
     * @return         success/fail.
     */
    LooperResult SeekAllTracksAbsolute(int loopPos=0);

    /**
     * Get a pointer to a specific track object that's owned by the Scumbler.
     * @param  index 0-based index of the track to retrieve
     * @return       pointer, nullptr if there's not a Track at that index.
     */
    Track* GetTrack(int index) const;


protected:
  /**
    * Start audio through our processor graph.
    */
  void Play();

  /**
   * Stop audio playback (but keep the loop pointers where they are)
   */
  void Pause();
    
    
    void ChangeMode(LoopStates mode);

  /**
   * Typedef for a pointer-to-member-function that's exposed by the
   * AudioProcessorGraph used to both connect and disconnect a single channel pair
   * of two nodes.
   * @param  uint32 node ID of source node
   * @param  int    source channel
   * @param  uint32 node ID of destination node
   * @param  int    destination channel index
   * @return        success/failure of the operation.
   */
  typedef bool (AudioProcessorGraph::*fnPtr)(uint32, int, uint32, int);


  /**
   * Internal function that takes care of either connecting or disconnecting two nodes in an audio processor graph.
   * @param  source     node id of the  source node.
   * @param  dest       node id of the destination node.
   * @param  connecting If true, we are connecting the nodes (else disconnecting)
   * @return            tk::Result code indicating success or the reason for failure.
   */
  LooperResult HandleConnection(NodeId source, NodeId dest, bool connecting);

  /**
   * Use this to convert one of the special enum values `Scumbler::kInput` or
   * `Scumbler::kOutput` into the actual node ids that
   * @param  node value that may either be a real node id (< 0xfffffffe) or one of the
   *              special values for the Scumbler's pre-existing in/out nodes.
   * @return      an actual node id (possibly the same value that was passed in.)
   */
  NodeId HandleSpecialNode(NodeId node);

private:
    JUCE_DECLARE_NON_COPYABLE(Looper);
    
    void setMidiInput (int index);
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message);
//    class IncomingMessageCallback : public CallbackMessage {
//        public:
//            IncomingMessageCallback (Looper* o, const MidiMessage& m, const String& s)
//                : owner (o), message (m), source (s) {}
//
//                void messageCallback() override {
//                    if (owner != nullptr)
//                    owner->handleIncomingMidiCommand(message);
//                }
//
//            Component::SafePointer<Looper> owner;
//            MidiMessage message;
//            String source;
//        };

    AudioProcessorPlayer player;
    AudioProcessorGraph graph;
    AudioDeviceManager& deviceManager;
    bool processing = false;
    bool playing = false;
    bool timeUpdate = false;
    NodeId inputNode = Node::InvalidNode;
    NodeId outputNode = Node::InvalidNode;
//   SampleCounterProcessor* sampleCount = nullptr;
    NodeId sampleCountNode;
    OwnedArray<Track> tracks;
    Array<String> trackNames = { "GUITAR", "DRUM", "RC30", "VOX" };
    LoopStates mode = LoopStates::PlayInRecord;
    bool wasRecorded = false;
    double startLoopingTime;
    double loopTime;
    int selectedTrack = 0;
    float outputVolume = 1.0f;
    int lastInputIndex = 0;
//   GainProcessor* outputGain;
    NodeId gainNode = NodeId(Node::InvalidNode);
};
