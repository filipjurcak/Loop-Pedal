/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "Loop.h"
#include "Looper.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ThumbnailComponent  : public Component,
                            private ChangeListener
{
public:
    //==============================================================================
    ThumbnailComponent (int sourceSamplesPerThumbnailSample, AudioFormatManager& formatManager, AudioThumbnailCache& cache, Looper* looper);
    ~ThumbnailComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void fillThumbnail();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AudioThumbnail thumbnail;

    Looper* looper;

    Track* track;

    LoopProcessor* loopProcessor;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

