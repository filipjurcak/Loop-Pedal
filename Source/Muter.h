/*
  ==============================================================================

    Muter.h
    Created: 16 Jun 2020 2:57:08am
    Author:  Filip Jurcak

  ==============================================================================
*/

#pragma once

#include "Passthrough.h"

class MuterProcessor : public PassthroughProcessor
{
public:
   MuterProcessor(int channelCount=1);

   ~MuterProcessor();

   void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages);

   bool silenceInProducesSilenceOut() const { return true; };
};
