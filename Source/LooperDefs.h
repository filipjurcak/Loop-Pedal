/*
  ==============================================================================

    LooperDefs.h
    Created: 12 Jun 2020 5:30:00pm
    Author:  Filip Jurcak

  ==============================================================================
*/

#pragma once

enum class LooperResult {
    Success,
    Failure
};

// this needs to be in sync with Arduino code to work correctly,
// so if you ever change Arduino code check this
enum class ButtonMidiNotes {
    Recplay = 69,
    Stop = 70,
    Mode = 71,
    Clear = 72,
    Undo = 73,
    Track1 = 74,
    Track2 = 75,
    Track3 = 76,
    Track4 = 77
};

enum class LoopStates { Play, Mute, Record, Overdub, PlayInRecord };

typedef uint32 NodeId;

enum Node: NodeId {
    InvalidNode = 0,
    Input = 0xffffffff,
    Output = 0xfffffffe
};
